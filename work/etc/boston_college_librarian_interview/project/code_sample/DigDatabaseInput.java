package com.cartera.dig.common.io.input;

import com.cartera.dig.common.DigNode;
import com.cartera.dig.common.database.ConnectionManager;
import com.cartera.dig.common.util.Args;
import com.cartera.dig.common.util.TaskStats;
import com.cartera.dig.common.util.To;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.Statement;
import java.sql.Types;
import java.io.File;
import java.io.FileWriter;
import java.io.FileInputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.lang.StringUtils;

@Slf4j
public abstract class DigDatabaseInput extends DigInput {

  private ConnectionManager connectionManager = null;
  private String table = "";
  private String database = "";
  private String where = "";
  private String orderBy = "";
  private Integer limit = null;
  private String query = "";
  private String lastQuery = "";
  private Connection connection = null;
  private ResultSet resultSet = null;
  private Map<String, Integer> resultSetFieldMap = null;
  private boolean isPaused = false;
  private TaskStats readStats = new TaskStats();
  private DigNode readLastRecord = null;

  // Allow specification of an infinite repeat on a specified auto-increment column such
  // that the database/table would be queried continually for data where the specified
  // auto-increment column is greater-than-or-equal to the value of the last/previous query.
  //
  // Enabled only if repeatPeriod is greater than 0. This is the number of seconds to wait/sleep
  // after each query before doing another one, the next one being where repeatAutoIncrementColumn
  // greater-than the value of the previous query. If no repeatAutoIncrementtart is specified,
  // then starts with the current max value in the table.
  //
  // For simplicity for first cut we allow a *local* *file* path to be specified where
  // the last repeat value should be stored, to get at startup as its start value.

  private int repeatPeriod = 0;
  private String repeatAutoIncrementColumn = "";
  private Long repeatAutoIncrementStart = null;
  private Long repeatAutoIncrementLast = null;
  private String repeatDateColumn = "";
  private Long repeatDateStart = null;
  private Long repeatDateLast = null;
  private File repeatLastStore = null;

  public void setConnectionManager(ConnectionManager connection) {
    connectionManager = connection;
  }

  public void setDatabase(String value) {
    database = StringUtils.defaultString(value).trim();
  }

  public String getDatabase() {
    return database;
  }

  public void setTable(String value) {
    value = StringUtils.defaultString(value).trim();
    int dot = value.indexOf('.');
    if (dot != -1) {
      setDatabase(value.substring(0, dot));
      table = value.substring(dot + 1);
    } else {
      table = value;
    }
  }

  public String getTable() {
    return table;
  }

  public void setQuery(String value) {
    query = StringUtils.defaultString(value).trim();
  }

  public void setWhere(String value) {
    where = StringUtils.defaultString(value).trim();
  }

  public void setOrderBy(String value) {
    orderBy = StringUtils.defaultString(value).trim();
  }

  public void setLimit(Integer value) {
    limit = value;
  }

  /**
   * Sets the repeatPeriod. Specifies that the input database table query should be done
   * repeatedly, indefinitely every N seconds as specfied by the given seconds argument.
   * I.e. after the end of the database table input based on the query is reached, the
   * input process will wait/sleep for the specified number of seconds, and then will
   * do the query again as if from the beginning, and so on. See the
   * repeatAutoIncrementColumn which is also required for this option.
   * @param value The repeat period value.
   */
  public void setRepeatPeriod(int value) {
    repeatPeriod = value > 0 ? value : 0;
  }

  /**
   * Sets the repeatAutoIncrementColumn. Used (only) with the repeatPeriod option, this
   * specifies the name of the auto-increment column to use in the WHERE clause for the
   * database table query used for the input. In this case, the first time the database
   * table query is done, the WHERE clause will be such that this auto-increment column
   * is greater-than-or-equal to the repeatAutoIncrementStart value, if specified, or,
   * if not specified, then greater-than the current maximum value of the auto-increment
   * column in the table. Subsequent (repeated) calls will be such that this auto-increment
   * column is greater-than the maximum value of this auto-increment column in the last query.
   * @param value The repeat auto-increment column value.
   */
  public void setRepeatAutoIncrementColumn(String value) {
    repeatAutoIncrementColumn = StringUtils.defaultString(value).trim();
  }

  /**
   * Used (only) with the repeatAutoIncrementColumn option, this specifies the (integer)
   * value to use as the first value of the auto-increment column to query for, i.e. if
   * specified then the WHERE clause of the first query (in the infinitely repeated loop
   * of queries) will contain a predicate equivalent to: (auto-increment-column &gt;= value).
   * @param value The repeat auto-increment start value.
   */
  public void setRepeatAutoIncrementStart(Long value) {
    repeatAutoIncrementStart = (value != null) && (value >= 0) ? value : null;
  }

  /**
   * Same as auto-increment version except for a date.
   * @param value The repeat date column value.
   */
  public void setRepeatDateColumn(String value) {
    repeatDateColumn = StringUtils.defaultString(value).trim();
  }

  /**
   * Same as auto-increment version except for a date.
   * @param value The repeat date start value.
   */
  public void setRepeatDateStart(Date value) {
    repeatDateStart = (value != null) ? value.getTime() : null;
  }

  /**
   * Sets the full path of a file to use to store the last repeat value.
   * @param value The repeat last store value.
   */
  public void setRepeatLastStore(String value) {
    value = StringUtils.defaultString(value).trim();
    if (value.length() > 0) {
        repeatLastStore = new File(value);
    }
  }

  @Override
  public void open() throws Exception {
    if (connection != null) {
      close();
    }
    connection = connectionManager.connect();
  }

  @Override
  public void close() throws Exception {

    // TODO
    // The close calls here hang if closing before the result set has been
    // fully traveresed, e.g. if/when we are limiting the input to a number
    // of records fewer than what is being returned by this query.

    if (resultSet != null) {
      Statement statement = resultSet.getStatement();
      if (statement != null) {
        statement.cancel();
        statement.close();
      }
      resultSet.close();
      resultSet = null;
      resultSetFieldMap = null;
    }
    if (connection != null) {
      connection.close();
      connection = null;
    }
  }

  @Override
  public DigNode read() throws Exception {

    // The (apparent) infinite loop is to handle the repeatPeriod functionality.

    while (true) {

      if (isPaused) {
        Thread.sleep(100);
        return DigInput.NO_INPUT;
      }

      final long readTimestamp = readStats.noteStart();

      if (resultSet == null) {
        Statement statement = connection.createStatement();
        if (this instanceof DigMySqlInput) {
          statement.setFetchSize(Integer.MIN_VALUE);
        }
        String query = generateQuery();
        resultSet = statement.executeQuery(query);
      }

      if (resultSet.next()) {
        if (resultSetFieldMap == null) {
          resultSetFieldMap = getFieldMap(resultSet);
        }
        DigNode result = resultSetToJson(resultSet, resultSetFieldMap);
        if (repeatAutoIncrementLast != null) {
          //
          // If we're repeating the query based on a specified auto-increment column
          // then we watch for the largest value in this result set; this value will
          // be used as the basis for the next query in the repeated query loop.
          //
          long autoIncrementValue = result.toLong(repeatAutoIncrementColumn, -1);
          if (autoIncrementValue > repeatAutoIncrementLast) {
            repeatAutoIncrementLast = autoIncrementValue;
            writeRepeatLastToStore(repeatAutoIncrementLast);
          }
        } else if (repeatDateLast != null) {
          Date dateValue = To.DateTime(result.toString(repeatDateColumn));
          if (dateValue != null) {
            long dateValueLong = dateValue.getTime();
            if (dateValueLong > repeatDateLast) {
              repeatDateLast = dateValueLong;
              writeRepeatLastToStore(repeatDateLast);
            }
          }
        }
        readStats.noteEnd(readTimestamp);
        readLastRecord = result;
        return result;
      }

      // If we get here then we're done iterating through the query results,
      // i.e. since the resultSet.next() call above was false.

      readStats.noteCancel();

      if (repeatPeriod > 0) {
        close();
        Thread.sleep(repeatPeriod * 1000);
        open();

      } else {
        readStats.noteDone();
        return DigInput.END_INPUT;
      }
    }
  }

  @Override
  public void pause() {
    if (!isPaused) {
      isPaused = true;
    }
  }

  @Override
  public void resume() {
    if (isPaused) {
      isPaused = false;
    }
  }

  @Override
  public boolean isPaused() {
    return isPaused;
  }

  @Override
  public DigNode status(Args args) {
    Date now = new Date();
    DigNode status = DigNode.create();
    status.put("class", this.getClass().getName());
    status.put("timestamp", To.IsoDate(now));
    status.put("database", database);
    status.put("table", table);
    status.put("connection", connectionManager.getConnectionInfo().status(args));
    status.put("query", lastQuery);
    status.put("isPaused", isPaused);
    status.put("repeatPeriod", repeatPeriod);
    status.put("repeatAutoIncrementColumn", repeatAutoIncrementColumn);
    status.put("repeatAutoIncrementLast", repeatAutoIncrementLast);
    status.put("repeatAutoIncrementStart", repeatAutoIncrementStart);
    status.put("repeatDateColumn", repeatDateColumn);
    status.put("repeatDateLast", repeatDateLast);
    status.put("repeatDateStart", repeatDateStart);
    status.put("repeatLastStore", repeatLastStore != null ? repeatLastStore.getAbsolutePath() : null);
    status.put("readStats", readStats.status());
    status.put("readLastRecord", readLastRecord);
    return status;
  }

  protected String generateQuery() throws Exception {

    if (query.length() > 0) {
      lastQuery = query;
      return query;
    }

    String query = "select * from " + database + "." + table;
    String whereClause = where;
    String repeatClause = "";

    if (repeatPeriod > 0) {
      if (repeatAutoIncrementColumn.length() > 0) {
          if (repeatAutoIncrementLast == null) {
            if (repeatAutoIncrementStart != null) {
              //
              // Sic. Our query is for *greater-than* the *last* auto-increment value;
              // so if a start value is given set the "last" value to it minus 1.
              //
              repeatAutoIncrementLast = repeatAutoIncrementStart - 1;
            }
            else {
              repeatAutoIncrementLast = getRepeatAutoIncrementLastOrMax();
            }
          }
          if (repeatAutoIncrementLast != null) {
            if (whereClause.length() > 0) whereClause += " and ";
            whereClause += "(" + repeatAutoIncrementColumn + " > " + repeatAutoIncrementLast + ")";
          }
      }
      else if (repeatDateColumn.length() > 0) {
          if (repeatDateLast == null) {
            if (repeatDateStart != null) {
              repeatDateLast = repeatDateStart - 1;
            }
            else {
              repeatDateLast = getRepeatDateLastOrMax();
            }
          }
          if (repeatDateLast != null) {
            if (whereClause.length() > 0) whereClause += " and ";
            whereClause += "(" + repeatDateColumn + " > '" + To.IsoDate(new Date(repeatDateLast)) + "')";
          }
      }
    }

    if (whereClause.length() > 0) {
      query += " where " + whereClause;
    }
    if (orderBy.length() > 0) {
      query += " order by " + orderBy;
    }
    if ((limit != null) && (limit > 0)) {
      query += " limit " + limit;
    }

    lastQuery = query;

    return query;
  }

  protected Map<String, Integer> getFieldMap(ResultSet rs) throws Exception {
    Map<String, Integer> fieldMap = new HashMap<String, Integer>();
    ResultSetMetaData metadata = rs.getMetaData();
    final int fieldCount = metadata.getColumnCount();
    for (int i = 1; i <= fieldCount; i++) {
      fieldMap.put(metadata.getColumnName(i), metadata.getColumnType(i));
    }
    return fieldMap;
  }

  protected DigNode resultSetToJson(ResultSet rs, Map<String, Integer> fieldMap) throws Exception {

    DigNode json = DigNode.create();

    for (Map.Entry<String, Integer> field : fieldMap.entrySet()) {

      String name = field.getKey();
      Integer type = field.getValue();

      if (type == Types.VARCHAR) {
        json.put(name, rs.getString(name));
      } else if ((type == Types.INTEGER) || (type == Types.SMALLINT) || (type == Types.TINYINT)) {
        json.put(name, rs.getInt(name));
      } else if (type == Types.BIGINT) {
        json.put(name, rs.getLong(name));
      } else if ((type == Types.BOOLEAN) || (type == Types.BIT)) {
        json.put(name, rs.getBoolean(name));
      } else if (type == Types.FLOAT) {
        json.put(name, rs.getFloat(name));
      } else if (type == Types.DOUBLE) {
        json.put(name, rs.getDouble(name));
      } else if ((type == Types.DATE) || (type == Types.TIMESTAMP)) {
        //
        // May need to handle various date formats.
        //
        try {
          json.put(name, rs.getString(name));
        } catch (Exception e) {
          json.put(name, "");
        }
      } else {
        json.put(name, rs.getString(name));
      }
    }

    return json;
  }

  /**
   * Gets the last (auto-increment) repeat value to the "store" if such a store
   * is enabled (via repeatLastStore property) or if not, then the max auto-increment
   * value extant in the database.
   * @return value The last (auto-increment) value in the store or the max in the database.
   */
  private Long getRepeatAutoIncrementLastOrMax() throws Exception {
      if (repeatLastStore != null) {
          Long value = readRepeatLastFromStore();
          if (value != null) {
              return value;
          }
      }
      String sql = "select max(" + repeatAutoIncrementColumn + ") value from " + database + "." + table;
      Statement statement = connection.createStatement();
      ResultSet resultSet = statement.executeQuery(sql);
      if ((resultSet != null) && resultSet.next()) {
          Long value = resultSet.getLong("value");
          return (value != null) && (value >= 0) ? value : null;
      }
      return null;
  }

  /**
   * Same as auto-increment version except for a date.
   * @return value The last repeatDateColumn value in the store or the max in the database.
   */
  private Long getRepeatDateLastOrMax() throws Exception {
      if (repeatLastStore != null) {
          Long value = readRepeatLastFromStore();
          if (value != null) {
              return value;
          }
      }
      String sql = "select max(" + repeatDateColumn + ") value from " + database + "." + table;
      Statement statement = connection.createStatement();
      ResultSet resultSet = statement.executeQuery(sql);
      if ((resultSet != null) && resultSet.next()) {
          Date value = resultSet.getTimestamp("value");
          return (value != null) ? value.getTime() : null;
      }
      return null;
  }

  // This could/should be eventually factored out into dependency-injected class/interface
  // which could store in database, Kafka, wherever.

  /**
   * Stores the last (auto-increment) repeat value to the "store".
   * Currently the "store" is just a file specified by the repeatLastStore property.
   * @param value The last (auto-increment) read.
   */
  private void writeRepeatLastToStore(long value)
  {
      if (repeatLastStore == null) {
          return;
      }
      try {
        try (FileWriter writer = new FileWriter(repeatLastStore)) {
            writer.write(value + "");
            writer.close();
        }
      } catch (Exception e) {
          log.error(e.toString());
      }
  }

  /**
   * Retrieves the last (auto-increment) repeat value from the "store".
   * Currently the "store" is just a file specified by the repeatLastStore property.
   * @return The value of the last (auto-increment) read.
   */
  private Long readRepeatLastFromStore()
  {
      if (repeatLastStore == null) {
          return null;
      }
      try {
        if (!repeatLastStore.exists() || !repeatLastStore.isFile()) {
            return null;
        }
        long fileLength = repeatLastStore.length();
        if (fileLength > 32) {
            //
            // Guard against reading in some random/wrong file which is huge.
            // This file just has the last repeat (auto-increment) value.
            //
            return null;
        }
        try (FileInputStream input = new FileInputStream(repeatLastStore)) {
            byte[] data = new byte[(int)fileLength];
            input.read(data);
            input.close();
            String value = new String(data, "UTF-8");
            return To.Long(value, null);
        }
      } catch (Exception e) {
          log.error(e.toString());
          return null;
      }
  }
}
