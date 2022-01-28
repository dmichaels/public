#!/bin/bash
# --------------------------------------------------------------------------------------------------
  CWD="$(cd "$(dirname ${BASH_SOURCE[0]})" > /dev/null 2>&1 && pwd)"
# --------------------------------------------------------------------------------------------------
# Script to load a MySQL or Vertica table from a CSV file.
# Supports simple load to a (presumably empty) table or upsert to a (presumably populated) table.
# The CSV file is assumed to be in a TAB-separated format and is assumed to have a column header.
# See: https://kiosc.cartera.com/display/MP/TABLELOAD
# See: git/.../stats/scripts/backoffice/vertica/replicateReceiveIncremental.php
# --------------------------------------------------------------------------------------------------

HOST=localhost
PORT=
USERNAME=
PASSWORD=
PASSWORD_OBFUSCATED=
MYSQL=
VERTICA=
VSQL_CLI=vsql
MYSQL_CLI=mysql

TABLE=
COLUMNLIST=
NOCOLUMNLIST=
INPUT=/dev/stdin
UNZIP=
NOLOCAL=
SET_GLOBAL_LOCAL=
UPSERT=
VERTICA_UPSERT_TMPDB=
VERTICA_UPSERT_TMPDB_INCLUDING_PROJECTIONS=
VERTICA_UPSERT_TMPDB_SIMPLE=
VERTICA_UPSERT_DELETE=
VERTICA_UPSERT_MERGE=1
VERBOSE=
VERBOSE_WITH_TIMESTAMPS=
DEBUG=
NODROP=
PING=
LOGFILE="/tmp/tableload-`date +'%Y%m%d-%H%M%S'`-$$.log"

RETURN_VALUE=0

if [ -t 0 ]                      ; then IS_INPUT_TERMINAL=1 ; fi
if [ -p /dev/stdin ]             ; then IS_INPUT_PIPE=1     ; fi
if [ ! -t 0 -a ! -p /dev/stdin ] ; then IS_INPUT_REDIRECT=1 ; fi

function usage() {
    if [ "$1" ] ; then echo "$1" 1>&2 ; fi
    echo "USAGE: `basename $0` { mysql | vertica } --host host [--port port] --username username --password password --table database.table [--input file] [--gunzip] [--upsert] [--verbose]" 1>&2
    exit 1
}

function help() {
    usage "For further details please see: https://kiosc.cartera.com/display/MP/TABLELOAD" 
}

function log() {
    if [ $DEBUG ] ; then 
        echo "`date +'%Y-%m-%d %H:%M:%S'`: $1" | tee -a $LOGFILE 1>&2
    elif [ $VERBOSE_WITH_TIMESTAMPS ] ; then 
        echo "`date +'%Y-%m-%d %H:%M:%S'`: $1" 1>&2
    elif [ $VERBOSE ] ; then
        echo "$1" 1>&2
    else
        echo "$1" >> $LOGFILE
    fi
}

function debug() {
    if [ $DEBUG ] ; then 
        echo "`date +'%Y-%m-%d %H:%M:%S'`: $1" | tee -a $LOGFILE 1>&2
    fi
}

function set_duration() {
    t=${1#-}
    values=($(($t % 60)) $(($t / 60 % 60)) $(($t / 3600 % 24)) $(($t / 86400)))
    seconds=${values[0]} ; minutes=${values[1]} ; hours=${values[2]}
    ss=`printf %02d $seconds`;  mm=`printf %02d $minutes` ; hh=${hours} ;  duration=$hh:$mm:$ss
}

function generate_guid() {
    guid=`uuidgen | tr -d '-' | tr '[:lower:]' '[:upper:]'`
}

if [ $# -lt 1 ] ; then
    usage
fi

ORIGINAL_ARGS=("$@")
FIRST_ARG=`echo $1 | tr '[:upper:]' '[:lower:]'`

if [ $FIRST_ARG == "mysql" ] ; then
    MYSQL=1 ; PORT=3306 ; shift
elif [ $FIRST_ARG == "vertica" ] ; then
    VERTICA=1 ; PORT=5433 ; shift
elif [ $FIRST_ARG == "--help" ] ; then
    help
else
    usage "First argument must be one of: mysql vertica"
fi

for DUMMY in "$@"
do
    case $1 in
        -h|--host|--hostname|--server)
            if [ $# -le 1 ] ; then usage ; fi
            HOST=$2
            shift ; shift ;;
        -h*)
            HOST=`echo $1 | cut -c3-`
            if [ -z "$HOST" ] ; then usage "Invalid --host option" ; fi
            shift ;;
        -P|--port)
            #
            # Defaults to 3306 for MySQL and 5433 for Vertica (below).
            #
            if [ $# -le 1 ] ; then usage ; fi
            PORT=$2
            shift ; shift ;;
        -P*)
            PORT=`echo $1 | cut -c3-`
            if [ -z "$PORT" ] ; then usage "Invalid --port option" ; fi
            shift ;;
        -u|--user|--username)
            if [ $# -le 1 ] ; then usage ; fi
            USERNAME=$2
            shift ; shift ;;
        -u*)
            USERNAME=`echo $1 | cut -c3-`
            if [ -z "$USERNAME" ] ; then usage "Invalid --username option" ; fi
            shift ;;
        -p|-w|--password)
            if [ $# -le 1 ] ; then usage ; fi
            PASSWORD=$2
            PASSWORD_OBFUSCATED="`echo $PASSWORD | cut -c1-1`xxxxxxx"
            shift ; shift ;;
        -p*)
            PASSWORD=`echo $1 | cut -c3-`
            PASSWORD_OBFUSCATED="`echo $PASSWORD | cut -c1-1`xxxxxxx"
            if [ -z "$PASSWORD" ] ; then usage "Invalid --password option" ; fi
            shift ;;
        -t|--table)
            if [ $# -le 1 ] ; then usage ; fi
            TABLE=$2
            read -r -a TABLE_ARRAY <<< `echo $TABLE | tr '.' ' '`
            if [ ${#TABLE_ARRAY[@]} -ne 2 ] ; then
                usage "The --table option requires a qualified database.table value ..."
            fi
            DATABASE=${TABLE_ARRAY[0]}
            UNQUALIFIED_TABLE=${TABLE_ARRAY[1]}
            shift ; shift ;;
        --columns)
            #
            # By default we explicitly specify the columns being loaded,
            # on the import SQL command (i.e. on the LOAD for MySQL and the
            # COPY for Vertica) from the column header of the input CSV file.
            # Use the --columns option to specify this (comma-separated) list
            # on the command line instead; not recommended.
            #
            if [ $# -le 1 ] ; then usage ; fi
            if [ $NOCOLUMNLIST ] ; then
                usage "Both --columns and --nocolumns can not be specified ..."
            fi
            COLUMNLIST=$2
            shift ; shift ;;
        --nocolumns)
            #
            # By default we explicitly specify the columns being loaded,
            # on the import SQL command (i.e. on the LOAD for MySQL and the
            # COPY for Vertica) from the column header of the input CSV file.
            # Use the --nocolumns option to suppress this; not recommended.
            #
            if [ "$COLUMNLIST" ] ; then
                usage "Both --nocolumns and --columns can not be specified ..."
            fi
            NOCOLUMNLIST=1
            shift ;;
        --primary-key|--primary-keys)
            #
            # Primary key(s) to use for Vertica (not MySQL) can be specified explicitly
            # using this option with a (comma-separated) list of table column names.
            #
            if [ $# -le 1 ] ; then usage ; fi
            PRIMARY_KEYS=`echo $2 | tr ',' ' '`
            shift ; shift ;;
        -i|--input)
            #
            # We ASSUME that the CSV file is in TAB-separated
            # format AND that it has a column header.
            #
            if [ $# -le 1 ] ; then usage ; fi
            INPUT=$2
            if [ "$INPUT" = "-" ] ; then
                INPUT=/dev/stdin
            fi
            FIRST_CHARACTER_OF_INPUT_SPECIFIER=`echo $INPUT | cut -c1-1`
            if [ $FIRST_CHARACTER_OF_INPUT_SPECIFIER != "/" ] ; then
                INPUT="`pwd`/$INPUT"
            fi
            if [ "$INPUT" != "/dev/stdin" -a ! -f $INPUT ] ; then
                usage "Cannot open file: $INPUT"
            fi
            IS_INPUT_TERMINAL=
            IS_INPUT_PIPE=
            IS_INPUT_REDIRECT=
            IS_INPUT_FILE=1
            shift ; shift ;;
        --unzip|--gunzip|--decompress|--uncompress)
            UNZIP=1
            shift ;;
        --nolocal)
            NOLOCAL=1
            shift ;;
        --set-global-local)
            #
            # On Mac OS X MySQL (at least) got an error on LOAD for which
            # the resolution was to do: SET GLOBAL LOCAL_INFILE = 1;
            # which is a wonky, as it does this for the MySQL instance. 
            #
            SET_GLOBAL_LOCAL=1
            shift ;;
        --upsert)
            UPSERT=1
            shift ;;
        --vertica-upsert-tmpdb)
            #
            # Use this to specify a different database/schema for the temporary table
            # used for the upsert than the database/schema of the target table.
            #
            if [ $# -le 1 ] ; then usage ; fi
            VERTICA_UPSERT_TMPDB=$2
            shift ; shift ;;
        --vertica-upsert-tmpdb-including-projections)
            #
            # Use this to specify that the temporary table used for the upsert
            # be created using CREATE TABLE T like S INCLUDING PROJECTIONS.
            # Otherwise this will be used:
            # CREATE TABLE T as (select * from S limit 0) unsegmented all nodes
            #
            VERTICA_UPSERT_TMPDB_INCLUDING_PROJECTIONS=1
            shift ;;
        --vertica-upsert-tmpdb-simple)
            #
            # Use this to specify that the temporary table used for the upsert
            # be created using CREATE TABLE T like S.
            # Otherwise this will be used:
            # CREATE TABLE T as (select * from S limit 0) unsegmented all nodes
            #
            VERTICA_UPSERT_TMPDB_SIMPLE=1
            shift ;;
        --vertica-upsert-delete|--vertica-upsert-delete-insert)
            #
            # Use this to specify that the upsert be done by first deleting records in
            # the target table which are in the source/temporary data/table, and then
            # inserting into the target table from the source/temporary data/table.
            #
            VERTICA_UPSERT_DELETE=1
            VERTICA_UPSERT_MERGE=
            shift ;;
        -v|--verbose)
            VERBOSE=1
            shift ;;
        --verboset)
            VERBOSE=1
            VERBOSE_WITH_TIMESTAMPS=1
            shift ;;
        --debug)
            DEBUG=1
            shift ;;
        --nodrop)
            NODROP=1
            shift ;;
        --mysql-cli|--mysql)
            if [ $# -le 1 ] ; then usage ; fi
            MYSQL_CLI=$2
            shift ; shift ;;
        --vsql-cli|--vsql)
            if [ $# -le 1 ] ; then usage ; fi
            VSQL_CLI=$2
            shift ; shift ;;
        --ping)
            PING=1
            shift ;;
        --help)
            help
            shift ;;
        --*)
            usage "Unrecognized option: $1"
            shift ;;
        *)
            if [ $1 ] ; then usage "Unknown argument: $1" ; fi
            shift ;;
    esac
done

if [ -z "$TABLE" -a -z "$PING" ] ; then
    usage "Missing --table option ..."
fi

if [ -z "$HOST" -o -z "$USERNAME" -o -z "$PASSWORD" ] ; then usage ; fi

# Get the list of column names to load ...
#
if [ -z "$NOCOLUMNLIST" -a -z "$PING" ] ; then
    if [ ! -z "$COLUMNLIST" ] ; then
        #
        # Here the list of columns to load was
        # explicitly specified on the command-line.
        #
        COLUMNLIST="($COLUMNLIST)"
    else
        #
        # We need to get the column header from the actual CSV input in order
        # to properly construct the SQL to load the data (via LOAD for MySQL
        # and via COPY for Vertica). This is not that so straightforward when
        # reading from stdin.
        #
        if [ "$INPUT" = "/dev/stdin" ] ; then
            if [ $UNZIP ] ; then
                #
                # Not sure how to do this ...
                # Need to do a non-destructive read of the first line from the gzipped stdin.
                #
                ARGS_WITHOUT_UNZIP=() ; for ARG in "${ORIGINAL_ARGS[@]}" ; do case $ARG in --unzip|--gunzip|--decompress) shift ;; -p|--password) ARGS_WITHOUT_UNZIP+=($ARG) ; shift ;  shift ;; *) ARGS_WITHOUT_UNZIP+=($ARG) ; shift ;; esac done
                
                log "RUNNING: exec gunzip -c | $0 `echo ${ARGS_WITHOUT_UNZIP[@]}`"
                gunzip -c | $0 ${ARGS_WITHOUT_UNZIP[@]}
                exit 0
            else
                #
                # A read of the first (column header) line of stdin using the read command
                # is destructive, so that that line is now no longer part of the stdin,
                # so no need to skip past it on load (below); but this is the case only
                # if the stdin is from pipe; in the case of stdin from a redirect,
                # the read is non-destructive, so we need to skip past it (below).
                #
               read COLUMNLIST
               COLUMNLIST="(`echo $COLUMNLIST | tr ' ' ','`)"
            fi
        else
            if [ $UNZIP ] ; then
                COLUMNLIST="(`gunzip -c $INPUT | awk 'NR==1 {print; exit}' | tr '\t' ','`)"
            else
                COLUMNLIST="(`head -1 $INPUT | tr '\t' ','`)"
            fi
        fi
    fi
    if [ "$COLUMNLIST" = "()" ] ; then
        usage "Column list is empty ..."
    fi
fi

log "CARTERA MYSQL/VERTICA TABLE IMPORT UTILITY"

debug "LOG: $LOGFILE"

if [ $VERTICA ] ; then

    start_time=`date +%s`

    VSQL_COMMAND="$VSQL_CLI -h $HOST -p $PORT -U $USERNAME -w $PASSWORD -A -t"
    VSQL_COMMAND_OBFUSCATED="$VSQL_CLI -h $HOST -p $PORT -U $USERNAME -w $PASSWORD_OBFUSCATED -A -t"

    if [ $PING ] ; then
        PINGR=`echo "select 'prufrock'" | $VSQL_COMMAND -t 2>&1`
        if [ "$PINGR" = "prufrock" ] ; then echo "PING OK" ; exit 0 ; else echo "PING FAILED" ; echo $PINGR ; exit 1 ; fi
    fi

    if [ $UPSERT ] ; then VERB="Upserting"
    else                  VERB="Inserting" ; fi

    if   [ $IS_INPUT_TERMINAL ] ; then log "VERTICA: $VERB into $TABLE from $INPUT (terminal)"
    elif [ $IS_INPUT_PIPE ]     ; then log "VERTICA: $VERB into $TABLE from $INPUT (pipe)"
    elif [ $IS_INPUT_REDIRECT ] ; then log "VERTICA: $VERB into $TABLE from $INPUT (redirect)"
    elif [ $IS_INPUT_FILE ]     ; then log "VERTICA: $VERB into $TABLE from $INPUT (file)"
    else                               log "VERTICA: $VERB into $TABLE from $INPUT" ; fi

    log "COMMAND: $VSQL_COMMAND_OBFUSCATED"

    if [ $UPSERT ] ; then
        #
        # For upsert into Vertica we will load the CSV into a temporary (source) table
        # and the MERGE into the target (destination) table using SQL that looks like:
        #
        # MERGE
        #  INTO destination d
        # USING source s
        #    ON s.brand_fk  = d.brand_fk
        #   AND s.member_id = d.member_id
        #  WHEN MATCHED THEN UPDATE
        #   SET email = s.email,
        #       flags = s.flags
        #  WHEN NOT MATCHED THEN INSERT
        #     ( brand_fk,
        #       member_id,
        #       email,
        #       flags )
        #     VALUES
        #     ( s.brand_fk,
        #       s.member_id,
        #       s.email,
        #       s.flags );
        #
        if [ -z "$PRIMARY_KEYS" ] ; then
            PRIMARY_KEYS_SQL="select column_name from primary_keys where table_schema = '$DATABASE' and table_name = '$UNQUALIFIED_TABLE' order by column_name"
            debug "PRIMARY KEYS SQL: $PRIMARY_KEYS_SQL"
            PRIMARY_KEYS=`$VSQL_COMMAND -c "$PRIMARY_KEYS_SQL" | tr '\n' ' '`
            debug "PRIMARY KEYS: $PRIMARY_KEYS"
        fi
        read -r -a PRIMARY_KEYS_ARRAY <<< $PRIMARY_KEYS
        if [ ${#PRIMARY_KEYS_ARRAY[@]} -eq 0 ] ; then
            usage "The --upsert option for Vertica requires that the table ($TABLE) have a primary key defined (and it does not) ..."
        fi
        COLUMNS_SQL="select column_name from columns where table_schema = '$DATABASE' and table_name = '$UNQUALIFIED_TABLE' order by column_name"
        debug "COLUMNS SQL: $COLUMNS_SQL"
        COLUMNS=`$VSQL_COMMAND -c "$COLUMNS_SQL" | tr '\n' ' '`
        read -r -a COLUMNS_ARRAY <<< $COLUMNS
        if [ ${#COLUMNS_ARRAY[@]} -eq 0 ] ; then
            usage "The --upsert option for Vertica requires that the table ($TABLE) have a least one column defined (and it does not) ..."
        fi
        debug "COLUMNS: $COLUMNS"
        generate_guid ; TEMPORARY_TABLE_NAME="${UNQUALIFIED_TABLE}_$guid"
        if [ ${#TEMPORARY_TABLE_NAME} -gt 20 ] ; then
            #
            # Temporary table name is too long; just use GUID.
            #
            TEMPORARY_TABLE="T$guid"
        fi
        if [ -z "$VERTICA_TMPDB" ] ; then TEMPORARY_TABLE="$DATABASE.$TEMPORARY_TABLE_NAME" ; else TEMPORARY_TABLE="$VERTICA_TMPDB.$TEMPORARY_TABLE_NAME" ; fi 
        ORIGINAL_TABLE=$TABLE
        TABLE=$TEMPORARY_TABLE

        USING_CLAUSE_SQL=
        for PRIMARY_KEY in "${PRIMARY_KEYS_ARRAY[@]}" ; do
            #
            # The USING_CLAUSE_SQL looks like this:
            #
            #       s.brand_fk  = d.brand_fk
            #   AND s.member_id = d.member_id
            #
            if [ ${#USING_CLAUSE_SQL} -gt 0 ] ; then
                USING_CLAUSE_SQL="$USING_CLAUSE_SQL and s.$PRIMARY_KEY = d.$PRIMARY_KEY"
            else
                USING_CLAUSE_SQL="s.$PRIMARY_KEY = d.$PRIMARY_KEY"
            fi
        done

        MATCHED_CLAUSE_SQL=
        for COLUMN in "${COLUMNS_ARRAY[@]}" ; do
            #
            # The MATCHED_CLAUSE_SQL looks like this:
            #
            #   email = s.email,
            #   flags = s.flags
            #
            IS_PRIMARY_KEY=
            for PRIMARY_KEY in "${PRIMARY_KEYS_ARRAY[@]}" ; do
                if [ $COLUMN = $PRIMARY_KEY ] ; then IS_PRIMARY_KEY=1 ; break ; fi
            done
            if [ -z $IS_PRIMARY_KEY ] ; then
                if [ ${#MATCHED_CLAUSE_SQL} -gt 0 ] ; then
                    MATCHED_CLAUSE_SQL="$MATCHED_CLAUSE_SQL, $COLUMN = s.$COLUMN"
                else
                    MATCHED_CLAUSE_SQL="$COLUMN = s.$COLUMN"
                fi
            fi
        done

        NOT_MATCHED_COLUMNS_CLAUSE_SQL=
        for COLUMN in "${COLUMNS_ARRAY[@]}" ; do
            #
            # The NOT_MATCHED_COLUMNS_CLAUSE_SQL looks like this:
            #
            #     brand_fk,
            #     member_id,
            #     email,
            #     flags
            #
            if [ ${#NOT_MATCHED_COLUMNS_CLAUSE_SQL} -gt 0 ] ; then
                NOT_MATCHED_COLUMNS_CLAUSE_SQL="$NOT_MATCHED_COLUMNS_CLAUSE_SQL, $COLUMN"
            else
                NOT_MATCHED_COLUMNS_CLAUSE_SQL="$COLUMN"
            fi
        done

        NOT_MATCHED_VALUES_CLAUSE_SQL=
        for COLUMN in "${COLUMNS_ARRAY[@]}" ; do
            #
            # The NOT_MATCHED_VALUES_CLAUSE_SQL looks like this:
            #
            #     s.brand_fk,
            #     s.member_id,
            #     s.email,
            #     s.flags
            #
            if [ ${#NOT_MATCHED_VALUES_CLAUSE_SQL} -gt 0 ] ; then
                NOT_MATCHED_VALUES_CLAUSE_SQL="$NOT_MATCHED_VALUES_CLAUSE_SQL, s.$COLUMN"
            else
                NOT_MATCHED_VALUES_CLAUSE_SQL="s.$COLUMN"
            fi
        done

        if [ $VERTICA_UPSERT_MERGE ] ; then
            MERGE_SQL="merge into $ORIGINAL_TABLE d using $TEMPORARY_TABLE s on $USING_CLAUSE_SQL"
            if [ ! -z "$MATCHED_CLAUSE_SQL" ] ; then
                MERGE_SQL="$MERGE_SQL when matched then update set $MATCHED_CLAUSE_SQL"
            fi
            MERGE_SQL="$MERGE_SQL when not matched then insert ($NOT_MATCHED_COLUMNS_CLAUSE_SQL) values ($NOT_MATCHED_VALUES_CLAUSE_SQL)"
        elif [ $VERTICA_UPSERT_DELETE ] ; then
            VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL=
            for PRIMARY_KEY in "${PRIMARY_KEYS_ARRAY[@]}" ; do
                #
                # The looks VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL like this:
                #
                #       brand_fk  = target_table.brand_fk
                #   AND member_id = target_table.member_id
                #
                if [ ${#VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL} -gt 0 ] ; then
                    VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL="$VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL and $PRIMARY_KEY = $ORIGINAL_TABLE.$PRIMARY_KEY"
                else
                    VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL="$PRIMARY_KEY = $ORIGINAL_TABLE.$PRIMARY_KEY"
                fi
            done
            VERTICA_UPSERT_DELETE_SQL="delete from $ORIGINAL_TABLE where exists (select 1 from $TEMPORARY_TABLE where $VERTICA_UPSERT_DELETE_KEY_CLAUSE_SQL)"
            VERTICA_UPSERT_INSERT_SQL="insert into $ORIGINAL_TABLE select * from $TEMPORARY_TABLE"
        else
            usage "Should not happen. Invalid Vertica upsert strategy (MERGE or DELETE/INSERT) specified."
        fi

        # Not using Vertica CREATE TEMPORARY TABLE
        # because it is not supported with use of LIKE.
        #
        if [ "$VERTICA_UPSERT_TMPDB_INCLUDING_PROJECTIONS" ] ; then
            CREATE_TEMPORARY_TABLE_SQL="create table $TEMPORARY_TABLE like $ORIGINAL_TABLE including projections"
        elif [ "$VERTICA_UPSERT_TMPDB_SIMPLE" ] ; then
            CREATE_TEMPORARY_TABLE_SQL="create table $TEMPORARY_TABLE like $ORIGINAL_TABLE"
        else
            #
            # This method of creating the temporary table for Vertica has been (so far) deemed the best.
            # For more info see:
            # - https://tickets.cartera.com/browse/DIG-1739
            # - https://tickets.cartera.com/browse/STATS-404
            # - https://github.com/CarteraCommerce/dig-promotions/blob/master/src/main/java/com/cartera/dig/promotions/utils/database/DatabaseInsert.java#L590
            #
            CREATE_TEMPORARY_TABLE_SQL="create table $TEMPORARY_TABLE as (select * from $ORIGINAL_TABLE limit 0) unsegmented all nodes"
        fi
        DROP_TEMPORARY_TABLE_SQL="drop table $TEMPORARY_TABLE cascade"
    fi

    if [ -z $NOLOCAL ] ; then
        LOCAL=local
    fi

    if [ $UNZIP ] ; then
        UNZIP=gzip
    fi

    if [ "$INPUT" = "/dev/stdin" ] ; then
        if [ $IS_INPUT_REDIRECT ] ; then
            SKIP="skip 1"
        else
            SKIP=
        fi
    else
        SKIP="skip 1"
    fi

    IMPORT_SQL="copy $TABLE $COLUMNLIST from $LOCAL '$INPUT' $UNZIP delimiter E'\t' record terminator E'\n' null as '\N' $SKIP abort on error"

    if [ $UPSERT ] ; then
        log "CREATE TMP TABLE SQL: $CREATE_TEMPORARY_TABLE_SQL"
        $VSQL_COMMAND -c "$CREATE_TEMPORARY_TABLE_SQL" > $LOGFILE 2>&1
        CREATE_TEMPORARY_TABLE_STATUS=$?
        if [ $CREATE_TEMPORARY_TABLE_STATUS != 0 ] ; then
            usage "ERROR: Cannot create temporary table ($TEMPORARY_TABLE); need sufficient privileges to create a temporary table in the '$DATABASE' schema."
        fi
    fi

    log "IMPORT SQL: $IMPORT_SQL"

    import_start_time=`date +%s`

    NROWS_IMPORTED=`$VSQL_COMMAND -c "$IMPORT_SQL"`

    stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $import_start_time` ; set_duration $duration_seconds

    log "ROWS IMPORTED: $NROWS_IMPORTED (DURATION: $duration)"

    if [ $UPSERT ] ; then

        if [ $VERTICA_UPSERT_MERGE ] ; then

            # Here, for Vertica, we will upsert by using the MERGE statement.

            log "UPSERT STRATEGY: MERGE"
            log "MERGE SQL: $MERGE_SQL"

            merge_start_time=`date +%s`

            # Use the echo pipe to vsql rather than -c just so we can get the row count,
            # which is not printed using the latter way where just get the COMMIT output.
            #
            # NROWS_UPSERTED=`$VSQL_COMMAND -c "$MERGE_SQL ; commit"`
            #
            NROWS_UPSERTED=`echo "$MERGE_SQL ; commit" | $VSQL_COMMAND | sed -e 's/\s*COMMIT\s*//'`

            stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $merge_start_time` ; set_duration $duration_seconds

            log "ROWS MERGED: $NROWS_UPSERTED (DURATION: $duration)"

        elif [ $VERTICA_UPSERT_DELETE ] ; then

            log "UPSERT STRATEGY: DELETE/INSERT"

            # Here, for Vertica, we will upsert by first doing a DELETE of records in the
            # target table which are in the source/temporary table by primary key(s), and
            # then doing an INSERT from the source/temporary table into the target table.

            log "DELETE SQL: $VERTICA_UPSERT_DELETE_SQL"
            delete_start_time=`date +%s`
            NROWS_DELETED=`echo "$VERTICA_UPSERT_DELETE_SQL ; commit" | $VSQL_COMMAND | sed -e 's/\s*COMMIT\s*//'`
            stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $delete_start_time` ; set_duration $duration_seconds
            log "ROWS DELETED: $NROWS_DELETED (DURATION: $duration)"

            log "INSERT SQL: $VERTICA_UPSERT_INSERT_SQL"
            insert_start_time=`date +%s`
            NROWS_INSERTED=`echo "$VERTICA_UPSERT_INSERT_SQL ; commit" | $VSQL_COMMAND | sed -e 's/\s*COMMIT\s*//'`
            stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $insert_start_time` ; set_duration $duration_seconds
            log "ROWS INSERTED: $NROWS_INSERTED (DURATION: $duration)"

        else
            usage "Should not happen. Invalid Vertica upsert strategy (MERGE or DELETE/INSERT) specified."
        fi

        if [ $NODROP ] ; then
            log "DROP TMP TABLE SQL (BUT NOT DROPPING): $DROP_TEMPORARY_TABLE_SQL"
        else
            log "DROP TMP TABLE SQL: $DROP_TEMPORARY_TABLE_SQL"
            $VSQL_COMMAND -c "$DROP_TEMPORARY_TABLE_SQL" > $LOGFILE 2>&1
        fi
    fi

    stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $start_time` ; set_duration $duration_seconds
    log "DURATION: $duration"

elif [ $MYSQL ] ; then

    if [ $PRIMARY_KEYS ] ; then
        usage "The --primary-keys option may only be used with Vertica (not MySQL) ..."
    fi

    start_time=`date +%s`

    ANNOYING_WARNING="Using a password on the command line interface can be insecure"

    MYSQL_COMMAND="$MYSQL_CLI -h$HOST -P$PORT -u$USERNAME -p$PASSWORD"
    MYSQL_COMMAND_OBFUSCATED="$MYSQL_CLI -h$HOST -P$PORT -u$USERNAME -p$PASSWORD_OBFUSCATED"

    if [ $PING ] ; then
        PINGR=`echo "select 'prufrock'" | $MYSQL_COMMAND --skip-column-names 2>&1| fgrep -v "$ANNOYING_WARNING"`
        if [ "$PINGR" = "prufrock" ] ; then echo "PING OK" ; exit 0 ; else echo "PING FAILED" ; echo $PINGR ; exit 1 ; fi
    fi

    if [ -z $NOLOCAL ] ; then
        MYSQL_COMMAND="$MYSQL_COMMAND --local-infile"
        MYSQL_COMMAND_OBFUSCATED="$MYSQL_COMMAND_OBFUSCATED --local-infile"
        LOCAL=local
    fi

    ORIGINAL_INPUT=$INPUT
    if [ $UNZIP ] ; then
        #
        # Unzipping to a named pipe and streaming from that with the LOAD;
        # does not seem to actually be substantively faster than just unziping the file
        # and then loading, but it does at least leave the specified zipped file intact.
        #
        NAMED_PIPE=/tmp/$$.pipe
        mkfifo -m 0600 $NAMED_PIPE
        INPUT=$NAMED_PIPE
    fi

    if [ $UPSERT ] ; then
        REPLACE=replace
    else
        REPLACE=
    fi

    if [ "$INPUT" = "/dev/stdin" ] ; then
        #
        # If input from stdin then we already read the
        # header line above using the bash 'read' command.
        #
        if [ $IS_INPUT_REDIRECT ] ; then
            IGNORELINES="ignore 1 lines"
        else
            IGNORELINES=
        fi
    else
        IGNORELINES="ignore 1 lines"
    fi

    IMPORT_SQL="load data $LOCAL infile '$INPUT' $REPLACE into table $TABLE fields terminated by '\t' $IGNORELINES $COLUMNLIST"

     if [ $SET_GLOBAL_LOCAL ] ; then
         IMPORT_SQL="set global local_infile = 1 ; $IMPORT_SQL"
     fi

    if [ $UPSERT ] ; then VERB="Upserting"
    else                  VERB="Inserting" ; fi

    if   [ $IS_INPUT_TERMINAL ] ; then log "MYSQL: $VERB into $TABLE from $ORIGINAL_INPUT (terminal)"
    elif [ $IS_INPUT_PIPE ]     ; then log "MYSQL: $VERB into $TABLE from $ORIGINAL_INPUT (pipe)"
    elif [ $IS_INPUT_REDIRECT ] ; then log "MYSQL: $VERB into $TABLE from $ORIGINAL_INPUT (redirect)"
    elif [ $IS_INPUT_FILE ]     ; then log "MYSQL: $VERB into $TABLE from $ORIGINAL_INPUT (file)"
    else                               log "MYSQL: $VERB into $TABLE from $ORIGINAL_INPUT" ; fi

    log "COMMAND: $MYSQL_COMMAND_OBFUSCATED"
    log "IMPORT SQL: $IMPORT_SQL"

    if [ $UNZIP ] ; then
        gunzip --stdout $ORIGINAL_INPUT > $NAMED_PIPE &
        GUNZIP_PID=$!
        #
        # TODO: Do we need to wait for this to finish below, after the load is (hopefully) done?
        #
    fi

      $MYSQL_COMMAND -e "$IMPORT_SQL" \
          2>&1| fgrep -v "$ANNOYING_WARNING"

    # This PIPESTATUS trick from Mike Smith on 2020-11-11 ...
    #
    for r in ${PIPESTATUS[@]}; do if [ $r -ne 0 ]; then RETURN_VALUE=$r; fi; done

    if [ "$NAMED_PIPE" ] ; then
        rm -f $NAMED_PIPE
    fi

    stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $start_time` ; set_duration $duration_seconds
    log "DURATION: $duration"

else
    usage "First argument must be one of: mysql vertica"
fi

exit $RETURN_VALUE
