#!/bin/bash
# --------------------------------------------------------------------------------------------------
  CWD="$(cd "$(dirname ${BASH_SOURCE[0]})" > /dev/null 2>&1 && pwd)"
# --------------------------------------------------------------------------------------------------
# Script to dump a MySQL or Vertica table to a CSV file.
# The CSV file will be in a TAB-separated format and will have a column header.
# See: https://kiosc.cartera.com/display/MP/TABLEDUMP
# See: git/.../stats/scripts/backoffice/vertica/replicateSendIncremental.php
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
CC_CLI=cc

TABLE=
WHERE=
LIMIT=
SKIP=
ORDER=
COLUMNS="*"
SELECT=
OUTPUT=/dev/stdout
NOCONVERT=
SCRUB=
ZIP=
VERBOSE=
VERBOSE_WITH_TIMESTAMPS=
DEBUG=
PING=
LOGFILE="/tmp/tabledump-`date +'%Y%m%d-%H%M%S'`-$$.log"

RETURN_VALUE=0

if [ -t 1 ]                       ; then IS_OUTPUT_TERMINAL=1 ; fi
if [ -p /dev/stdout ]             ; then IS_OUTPUT_PIPE=1     ; fi
if [ ! -t 1 -a ! -p /dev/stdout ] ; then IS_OUTPUT_REDIRECT=1 ; fi

function usage() {
    if [ "$1" ] ; then echo "$1" 1>&2 ; fi
    echo "USAGE: `basename $0` { mysql | vertica } --host host [--port port] --username username --password password --table database.table [--output file] [--gzip] [--where clause] [--order clause] [--count N] [--offset N] [--select { SQL | @file }] [--scrub] [--verbose]" 1>&2
    exit 1
}

function help() {
    usage "For further details please see: https://kiosc.cartera.com/display/MP/TABLEDUMP" 
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

# When exporting from MySQL (via mysql) null field values are exported as the string 'NULL' which
# are unfriendly to Vertica imports, so we convert them to '\N' with this bit of C code below,
# compiled on-the-fly. This code also converts field values with the exact values of '0000-00-00'
# or '0000-00-00 00:00:00', which (almost certainly) represent zero date values, and which are
# also unfriendly to Vertica imports, to '\N'. Note that the '\\\' in th C code below gets
# translated to '\\' by bash into the C file.
#
# P.S. Yes, this could be done using sed but would be much much slower.
# P.S. This exact kind of thing - using C code to do this same translation - is used by
#      Dave Andre's suite of replication PHP/bash scripts; but not compiled on-the-fly.
#
function compile_null_convert_utility() {
    NULL_CONVERT_UTILITY=/tmp/tabledump-convert-$$
    cat << ____EOCODE > $NULL_CONVERT_UTILITY.c
    #include <stdio.h>
    int main() {
        const int N = 102400; char input[N], output[N];
        while (fgets(input, N, stdin)) {
            char *q = output; int bof = 1;
            for (char *p = input ; *p != '\0' ; p++) {
                if (bof) {
                    if (p[0] == 'N' && p[1] == 'U' && p[2] == 'L' && p[3] == 'L') {
                        if ((p[4] == '\t') || (p[4] == '\n')) q[0] = '\\\', q[1] = 'N', q[2] = p[4], q += 3, p += 4; else *q++ = *p;
                    } else if (p[0] == '0' && p[1] == '0' && p[2] == '0' && p[3] == '0' && p[4] == '-' && p[5] == '0' && p[6] == '0' && p[7] == '-' && p[8] == '0' && p[9] == '0') {
                        if ((p[10] == '\t') || (p[10] == '\n')) q[0] = '\\\', q[1] = 'N', q[2] = p[10], q += 3, p += 10;
                        else if (p[10] == ' ' && p[11] == '0' && p[12] == '0' && p[13] == ':' && p[14] == '0' && p[15] == '0' && p[16] == ':' && p[17] == '0' && p[18] == '0') {
                            if ((p[19] == '\t') || (p[19] == '\n')) q[0] = '\\\', q[1] = 'N', q[2] = p[19], q += 3, p += 19; else *q++ = *p;
                        } else *q++ = *p;
                    } else *q++ = *p;
                } else *q++ = *p;
                bof = (*p == '\t');
            }
            *q = '\0'; fputs(output, stdout);
        } }
____EOCODE
    $CC_CLI -O -std=c99 -o $NULL_CONVERT_UTILITY $NULL_CONVERT_UTILITY.c
}

# When exporting from Vertica (via vsql) if fields contain embedded tabs or newlines or whatnot,
# they are not escaped or encoded, and will cause problems when subsequently are imported.
# So what we do is, if the --scrub option is used, to export (via vsql) with a field separator
# of 0x03 (ETX) rather than tab (via the -F option), and a record separator of 0x04 (EOT) rather
# than newline (via the -R option) - these values were chosen fairly arbitrarily, and note the last
# record has trailing newline rather than 0x04. Then we use the C code below, compiled on-the-fly,
# to translate all non-printable characters to a space, and of course also to convert the 0x03
# and 0x04 characters back to their proper tab and newline values, respectively. One nit is that
# vsql may output a final newline which, if not careful would get translated by this to a space
# making an extra line in the resultant CSV with just a space which would mess up subsequent imports.
#
# P.S. Yes, this could be done using sed but would be much much slower.
# P.S. Using vsql -Q to get trailing 0x04 on last record inserts empty line every 1001 records, odd.
#
function compile_vertica_scrub_utility() {
    VERTICA_SCRUB_UTILITY=/tmp/tabledump-vertica-scrub-$$
    cat << ____EOVERTICASCRUBCODE > $VERTICA_SCRUB_UTILITY.c
    #include <stdio.h>
    int main() {
        const int _X = 102400; char _x[_X]; int _i = 1, _n = 1; const int _Y = 102400; char _y[_Y]; int _j = 0;
        #define GETC() (_n > 0 ? (_i < _n ? _x[_i++] : ((_n = fread(_x, 1, _X, stdin)) > 0 ? (_i = 1, _x[0]) : -1)) : -1)
        #define PUTC(c) (_y[_j++] = c, _j == _Y ? (fwrite(_y, 1, _j, stdout), _j = 0) : 0)
        #define FLUSH() (_j > 0 ? (fwrite(_y, 1, _j, stdout), _j = 0) : 0)
        #define ISFUNNYC(c) ((c >= 0x00) && (c <= 0x1F))
        for (int c = GETC() ; c != -1 ; ) {
            if (c == '\x03') PUTC('\t');
            else if (c == '\x04') PUTC('\n');
            else if (c == '\n') { if ((c = GETC()) != -1) PUTC(' '); continue; }
            else if (ISFUNNYC(c)) PUTC(' ');
            else PUTC(c);
            c = GETC();
        }
        FLUSH(); }
____EOVERTICASCRUBCODE
    $CC_CLI -O -std=c99 -o $VERTICA_SCRUB_UTILITY $VERTICA_SCRUB_UTILITY.c
}

# If the --scrub option is specified for MySQL, export with a 0x03 (ETX) column between each column,
# and a 0x04 (EOT) as the last column - these values were chosen fairly arbitrarily. Then we use the
# C code below, compiled on-the-fly, to translate all non-printable characters to a space, and of course
# also to convert the 0x03 and 0x04 characters back to their proper tab and newline values, respectively.
#
# P.S. Yes, this could be done using sed but would be much much slower.
# P.S. Due to the scrub strategy here this will not work (be allowed) with the --select option.
#
function compile_mysql_scrub_utility() {
    MYSQL_SCRUB_UTILITY=/tmp/tabledump-mysql-scrub-$$
    cat << ____EOMYSQLSCRUBCODE > $MYSQL_SCRUB_UTILITY.c
    #include <stdio.h>
    int main() {
        const int _X = 102400; char _x[_X]; int _i = 1, _n = 1; const int _Y = 102400; char _y[_Y]; int _j = 0;
        #define GETC() (_n > 0 ? (_i < _n ? _x[_i++] : ((_n = fread(_x, 1, _X, stdin)) > 0 ? (_i = 1, _x[0]) : -1)) : -1)
        #define PUTC(c) (_y[_j++] = c, _j == _Y ? (fwrite(_y, 1, _j, stdout), _j = 0) : 0)
        #define FLUSH() (_j > 0 ? (fwrite(_y, 1, _j, stdout), _j = 0) : 0)
        #define ISFUNNYC(c) ((c >= 0x00) && (c <= 0x1F))
        for (int c = GETC() ; c != -1 ; ) {
            if (c == '\t') {
                if ((c = GETC()) == '\x03') {
                    if ((c = GETC()) == '\t') PUTC('\t');
                    else PUTC(' ');
                } else if (c == '\x04') {
                    if ((c = GETC()) == '\n') PUTC('\n');
                    else PUTC(' ');
                } else { PUTC(' '); if (!ISFUNNYC(c)) PUTC(c); }
            }
            else if (c == '\n') { if ((c = GETC()) != -1) PUTC(' '); continue; }
            else if (ISFUNNYC(c)) PUTC(' ');
            else PUTC(c);
            c = GETC();
        }
        FLUSH(); }
____EOMYSQLSCRUBCODE
    $CC_CLI -O -std=c99 -o $MYSQL_SCRUB_UTILITY $MYSQL_SCRUB_UTILITY.c
}

function set_duration() {
    t=${1#-}
    values=($(($t % 60)) $(($t / 60 % 60)) $(($t / 3600 % 24)) $(($t / 86400)))
    seconds=${values[0]} ; minutes=${values[1]} ; hours=${values[2]}
    ss=`printf %02d $seconds`;  mm=`printf %02d $minutes` ; hh=${hours} ; duration=$hh:$mm:$ss
}

if [ $# -lt 1 ] ; then
    usage
fi

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

for arg in "$@"
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
            shift ; shift ;;
        --where)
            if [ $# -le 1 ] ; then usage ; fi
            WHERE=$2
            shift ; shift ;;
        --limit|--count)
            if [ $# -le 1 ] ; then usage ; fi
            LIMIT=$2
            shift ; shift ;;
        --skip|--offset)
            if [ $# -le 1 ] ; then usage ; fi
            SKIP=$2
            shift ; shift ;;
        --order)
            if [ $# -le 1 ] ; then usage ; fi
            ORDER=$2
            shift ; shift ;;
        --columns)
            if [ $# -le 1 ] ; then usage ; fi
            COLUMNS=$2
            shift ; shift ;;
        --select)
            #
            # Rather than a table to dump, and explicit SELECT can be specified.
            # If this value begins with a '@' then the the following string is
            # assumed to refer to a file (name), or of it begins with a '/' it
            # is assumed to refer to a file, which contains the SELECT;
            #
            if [ $# -le 1 ] ; then usage ; fi
            SELECT=$2
            FIRST_CHARACTER_OF_SELECT_SPECIFIER=`echo $SELECT | cut -c1-1`
            if [ $FIRST_CHARACTER_OF_SELECT_SPECIFIER = "@" ] ; then
                SELECT_FILE=`echo $SELECT | cut -c2-`
                FIRST_CHARACTER_OF_SELECT_FILE=`echo $SELECT_FILE | cut -c1-1`
                if [ $FIRST_CHARACTER_OF_SELECT_FILE != "/" ] ; then
                    SELECT_FILE="`pwd`/$SELECT_FILE"
                fi
                if [ ! -f $SELECT_FILE ] ; then
                    usage "Cannot open --select file: $SELECT_FILE"
                fi
                SELECT=`cat $SELECT_FILE`
            elif [ $FIRST_CHARACTER_OF_SELECT_SPECIFIER = "/" ] ; then
                SELECT_FILE=$2
                SELECT=`cat $SELECT_FILE`
            fi
            shift ; shift ;;
        -o|--output)
            if [ $# -le 1 ] ; then usage ; fi
            OUTPUT=$2
            if [ "$OUTPUT" = "-" ] ; then
                INPUT=/dev/stdout
            fi
            FIRST_CHARACTER_OF_OUTPUT_SPECIFIER=`echo $OUTPUT | cut -c1-1`
            if [ $FIRST_CHARACTER_OF_OUTPUT_SPECIFIER != "/" ] ; then
                OUTPUT="`pwd`/$OUTPUT"
            fi
            if [ $OUTPUT != "/dev/stdout" ] ; then
                IS_OUTPUT_TERMINAL=
                IS_OUTPUT_PIPE=
                IS_OUTPUT_REDIRECT=
                IS_OUTPUT_FILE=1
            fi
            shift ; shift ;;
        --noconvert)
            NOCONVERT=1
            shift ;;
        --zip|--gzip|--compress)
            ZIP=1
            shift ;;
        --scrub)
            SCRUB=1
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
        --mysql-cli|--mysql)
            if [ $# -le 1 ] ; then usage ; fi
            MYSQL_CLI=$2
            shift ; shift ;;
        --vsql-cli|--vsql)
            if [ $# -le 1 ] ; then usage ; fi
            VSQL_CLI=$2
            shift ; shift ;;
        --cc-cli|--cc)
            if [ $# -le 1 ] ; then usage ; fi
            CC_CLI=$2
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

if [ -z "$TABLE" -a -z "$SELECT" -a -z "$PING" ] ; then
    usage
fi

# On some systems if you sudo to a different user and do echo > /dev/stdout
# you get a "Permission denied" error, so to workaround if our output is
# actually to the terminal then use /dev/tty instead.
#
if [ $OUTPUT = "/dev/stdout" -a "$IS_OUTPUT_TERMINAL" ] ; then
    OUTPUT=/dev/tty
fi

if [ -z "$HOST" -o -z "$USERNAME" -o -z "$PASSWORD" ] ; then usage ; fi

log "CARTERA MYSQL/VERTICA TABLE EXPORT UTILITY"

debug "LOG: $LOGFILE"

if [ $VERTICA ] ; then

    start_time=`date +%s`

    if [ "$NOCONVERT" ] ; then
        usage "The --noconvert option is only for mysql ..."
    fi

    VSQL_COMMAND="$VSQL_CLI -h $HOST -p $PORT -U $USERNAME -w $PASSWORD -A -P footer"
    VSQL_COMMAND_OBFUSCATED="$VSQL_CLI -h $HOST -p $PORT -U $USERNAME -w $PASSWORD_OBFUSCATED -A -P footer"

    if [ $PING ] ; then
        PINGR=`echo "select 'prufrock'" | $VSQL_COMMAND -t 2>&1`
        if [ "$PINGR" = "prufrock" ] ; then echo "PING OK" ; exit 0 ; else echo "PING FAILED" ; echo $PINGR ; exit 1 ; fi
    fi

    if [ "$SELECT" ] ; then
        #
        # If the SELECT is explicitly specified, you're on your own kid ...
        #
        if [ "$WHERE" ] ; then usage "Cannot specify the --where option along with --select option ..." ; fi
        if [ "$ORDER" ] ; then usage "Cannot specify the --order option along with --select option ..." ; fi
        if [ "$COLUMNS" != "*" ] ; then usage "Cannot specify the --columns option along with --select option ..." ; fi
        if [ "$SELECT_FILE" ] ; then
            log "SELECT FILE: $SELECT_FILE"
        fi
    else
        SELECT="select $COLUMNS from $TABLE"
        if [ "$WHERE" ] ; then
            SELECT="$SELECT where $WHERE"
        fi
        if [ "$ORDER" ] ; then
            SELECT="$SELECT order by $ORDER"
        fi
    fi
    if [ "$LIMIT" ] ; then
        SELECT="$SELECT limit $LIMIT"
    fi
    if [ "$SKIP" ] ; then
        SELECT="$SELECT skip $SKIP"
    fi

    if [ $VERBOSE ] ; then
        if   [ $IS_OUTPUT_TERMINAL ] ; then OUTPUT_DESCRIPTION="(terminal)"
        elif [ $IS_OUTPUT_PIPE ]     ; then OUTPUT_DESCRIPTION="(pipe)"
        elif [ $IS_OUTPUT_REDIRECT ] ; then OUTPUT_DESCRIPTION="(redirect)"
        elif [ $IS_OUTPUT_FILE ]     ; then OUTPUT_DESCRIPTION="(file)" ; fi
        if [ "$SELECT_FILE" ] ; then
            log "VERTICA: Dumping given SELECT to $OUTPUT $OUTPUT_DESCRIPTION"
        else
            log "VERTICA: Dumping $TABLE to $OUTPUT $OUTPUT_DESCRIPTION"
        fi
        #
        # Cannot seem to get quoted -F and similar vsql args
        # to work when putting them in bash variables above.
        #
        if [ $SCRUB ] ; then
            log "COMMAND: $VSQL_COMMAND_OBFUSCATED -P null='\N' -F $'\x03' -R $'\x04'"
        else
            log "COMMAND: $VSQL_COMMAND_OBFUSCATED -P null='\N' -F $'\t'"
        fi
        if [ "$SELECT_FILE" ] ; then
            debug "SELECT SQL: $SELECT"
        else
            log "SELECT SQL: $SELECT"
        fi
    fi

    if [ $ZIP ] ; then

        echo "$SELECT" \
            | $VSQL_COMMAND -P null='\N' -F $'\t' \
                | gzip \
                    > $OUTPUT
    else
        if [ $SCRUB ] ; then
            compile_vertica_scrub_utility
            log "SCRUB: $VERTICA_SCRUB_UTILITY.c"
            echo "$SELECT" \
                | $VSQL_COMMAND -P null='\N' -F $'\x03' -R $'\x04' \
                    | $VERTICA_SCRUB_UTILITY \
                        > $OUTPUT
        else
            echo "$SELECT" \
                | $VSQL_COMMAND -P null='\N' -F $'\t' \
                    > $OUTPUT
        fi
    fi

    if [ -z "$DEBUG" ] ; then rm -f $VERTICA_SCRUB_UTILITY.c $VERTICA_SCRUB_UTILITY ; fi

    stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $start_time` ; set_duration $duration_seconds
    log "DURATION: $duration"

elif [ $MYSQL ] ; then

    start_time=`date +%s`

    ANNOYING_WARNING="Using a password on the command line interface can be insecure"

    MYSQL_COMMAND="$MYSQL_CLI -h$HOST -P$PORT -u$USERNAME -p$PASSWORD --quick"
    MYSQL_COMMAND_OBFUSCATED="$MYSQL_CLI -h$HOST -P$PORT -u$USERNAME -p$PASSWORD_OBFUSCATED --quick"

    if [ $PING ] ; then
        PINGR=`echo "select 'prufrock'" | $MYSQL_COMMAND --skip-column-names 2>&1| fgrep -v "$ANNOYING_WARNING"`
        if [ "$PINGR" = "prufrock" ] ; then echo "PING OK" ; exit 0 ; else echo "PING FAILED" ; echo $PINGR ; exit 1 ; fi
    fi

    if [ "$SELECT" ] ; then
        #
        # If the SELECT is explicitly specified, you're on your own kid ...
        #
        if [ "$WHERE" ] ; then usage "Cannot specify the --where option along with --select option ..." ; fi
        if [ "$ORDER" ] ; then usage "Cannot specify the --order option along with --select option ..." ; fi
        if [ "$COLUMNS" != "*" ] ; then usage "Cannot specify the --columns option along with --select option ..." ; fi
        if [ "$SCRUB" ] ; then usage "Cannot specify the --scrub option for mysql along with --select option ..." ; fi
    else
        if [ $SCRUB ] ; then
            MYSQL_COMMAND="$MYSQL_COMMAND -N"
            if [ "$COLUMNS" = "*" ] ; then
                COLUMNS=`$MYSQL_COMMAND -e "select column_name from information_schema.columns where concat(table_schema,'.',table_name) = '$TABLE'" | tr '\n' ',' | sed 's/,$//g'`
            fi
            for column in `echo $COLUMNS | tr ',' ' '`
            do if [ -z $COLUMNS_SCRUB ] ; then COLUMNS_SCRUB=$column ; else COLUMNS_SCRUB="$COLUMNS_SCRUB,X'03',$column" ; fi done ; COLUMNS_SCRUB="$COLUMNS_SCRUB,X'04'"
            ORIGINAL_COLUMNS=$COLUMNS
            COLUMNS=$COLUMNS_SCRUB
        fi
        SELECT="select $COLUMNS from $TABLE"
        if [ "$WHERE" ] ; then
            SELECT="$SELECT where $WHERE"
        fi
        if [ "$ORDER" ] ; then
            SELECT="$SELECT order by $ORDER"
        fi
    fi
    if [ "$LIMIT" ] ; then
        SELECT="$SELECT limit $LIMIT"
    fi
    if [ "$SKIP" ] ; then
        SELECT="$SELECT skip $SKIP"
    fi

    if [ $VERBOSE ] ; then
        if   [ $IS_OUTPUT_TERMINAL ] ; then OUTPUT_DESCRIPTION="(terminal)"
        elif [ $IS_OUTPUT_PIPE ]     ; then OUTPUT_DESCRIPTION="(pipe)"
        elif [ $IS_OUTPUT_REDIRECT ] ; then OUTPUT_DESCRIPTION="(redirect)"
        elif [ $IS_OUTPUT_FILE ]     ; then OUTPUT_DESCRIPTION="(file)" ; fi
        if [ "$SELECT_FILE" ] ; then
            log "MYSQL: Dumping given SELECT to $OUTPUT $OUTPUT_DESCRIPTION"
            log "COMMAND: $MYSQL_COMMAND_OBFUSCATED"
            log "SELECT FILE: $SELECT_FILE"
            debug "SELECT SQL: $SELECT"
        else
            log "MYSQL: Dumping $TABLE to $OUTPUT $OUTPUT_DESCRIPTION"
            log "COMMAND: $MYSQL_COMMAND_OBFUSCATED"
            log "SELECT SQL: $SELECT"
        fi
    fi

    # NULL_CONVERT_UTILITY converts 'NULL' values and the zero date/time values '0000-00-00'
    # and '0000-00-00 00:00:00' in the generated CSV file to '\N' values; the C version, defined
    # inline within this bash script, above (see function: compile_null_convert_utility) does
    # what the following set of sed commands does, except much much faster ...
    #
    #      sed -e 's/^NULL\t/\\N\t/g' \
    #    | sed -e 's/\tNULL\t/\t\\N\t/g' \
    #    | sed -e 's/\tNULL\t/\t\\N\t/g' \
    #    | sed -e 's/\tNULL$/\t\\N/g' \
    #    | sed -e 's/^0000-00-00 00:00:00\t/\\N\t/g' \
    #    | sed -e 's/\t0000-00-00 00:00:00\t/\t\\N\t/g' \
    #    | sed -e 's/\t0000-00-00 00:00:00$/\t\\N/g' \
    #    | sed -e 's/^0000-00-00 00:00:00$/\\N/g'

    if [ $NOCONVERT ] ; then
        FILTER=cat
        log "CONVERT: none"
    else
        compile_null_convert_utility
        FILTER="$NULL_CONVERT_UTILITY"
        if [ $SCRUB ] ; then
            compile_mysql_scrub_utility
            log "SCRUB: $MYSQL_SCRUB_UTILITY.c"
            FILTER="$MYSQL_SCRUB_UTILITY | $FILTER"
            echo $ORIGINAL_COLUMNS | tr ',' '\t' > $OUTPUT
        else
            echo -n > $OUTPUT
        fi
        log "CONVERT: $NULL_CONVERT_UTILITY.c"
    fi
    if [ $ZIP ] ; then
        FILTER="$FILTER | gzip"
    fi

    $MYSQL_COMMAND -e "$SELECT" \
        2>&1| fgrep -v "$ANNOYING_WARNING" \
            | eval $FILTER \
                >> $OUTPUT

    # This PIPESTATUS trick from Mike Smith on 2020-11-11 ...
    #
    for r in ${PIPESTATUS[@]}; do if [ $r -ne 0 ]; then RETURN_VALUE=$r; fi; done

    if [ -z "$DEBUG" ] ; then rm -f $NULL_CONVERT_UTILITY.c $NULL_CONVERT_UTILITY $MYSQL_SCRUB_UTILITY.c $MYSQL_SCRUB_UTILITY ; fi

    stop_time=`date +%s` ; duration_seconds=`expr $stop_time - $start_time` ; set_duration $duration_seconds
    log "DURATION: $duration"

else
    usage "First argument must be one of: mysql vertica"
fi

exit $RETURN_VALUE
