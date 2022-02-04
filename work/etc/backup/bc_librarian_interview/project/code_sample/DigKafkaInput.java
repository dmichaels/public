package com.cartera.dig.common.io.input;

import com.cartera.dig.common.DigNode;
import com.cartera.dig.common.kafka.KafkaTopicInfo;
import com.cartera.dig.common.security.EncryptionService;
import com.cartera.dig.common.util.Args;
import com.cartera.dig.common.util.TaskStats;
import com.cartera.dig.common.util.To;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.lang.StringUtils;
import org.apache.kafka.clients.consumer.Consumer;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRebalanceListener;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.consumer.OffsetAndMetadata;
import org.apache.kafka.clients.consumer.OffsetAndTimestamp;
import org.apache.kafka.common.PartitionInfo;
import org.apache.kafka.common.TopicPartition;
import org.apache.kafka.common.errors.WakeupException;
import org.apache.kafka.common.serialization.StringDeserializer;

@Slf4j
public class DigKafkaInput extends DigInput {

  private String topic = "";
  private String groupId = "DigKafkaInputGroup";
  private String autoOffsetReset = "";
  private int maxPollRecords = 100;
  private Long offset;
  private Date offsetTimestamp;
  private Map<Integer,Long> offsetTimestampMap;
  private Properties properties;
  private KafkaConsumer<String, String> consumer;
  private ConsumerRecords<String, String> records;
  private List<TopicPartition> assignedPartitions;
  private Iterator<ConsumerRecord<String, String>> iterator;
  private AtomicLong pollInterval = new AtomicLong(1000L);
  private EncryptionService encryptionService;
  private boolean includeMetadata;
  private TaskStats readStats = new TaskStats();
  private long readCount;
  private DigNode readLastRecord;
  private long readLastOffset;
  private long readLastPartition;
  private long readLastTimestamp;
  private boolean isPaused = false;
  private AtomicBoolean isExited = new AtomicBoolean(false);

  public final static long TOPIC_OFFSET_BEGINNING = -1L;
  public final static long TOPIC_OFFSET_END = -2L;

 


  public void setTopic(String value) {
    topic = StringUtils.defaultString(value).trim();
  }

  /**
   * Set the group ID for this Kafka consumer.
   */
  public void setGroupId(String value) {
    groupId = StringUtils.defaultString(value).trim();
  }

  public void setAutoOffsetReset(String value) {
    autoOffsetReset = StringUtils.defaultString(value).trim();
  }

  public void setMaxPollRecords(int value) {
    if (value > 0) {
      maxPollRecords = value;
    }
  }

  public void setEncryptionService(EncryptionService value) {
    encryptionService = value;
  }

  public void setIncludeMetadata(boolean value) {
    includeMetadata = value;
  }

  /*
   * Requests that this Kafka input/reader/consumer should start reading
   * from the specified offset. Possible values are:
   *
   * - The string-literal "beginning" (or "begin" or "start" or "earliest"),
   *   meaning that the consumer will start reading the topic from the beginning.
   *
   * - The string-literal "end" (or "latest"),
   *   meaning that the consumer will start reading the topic from the end, i.e.
   *   only messages written to the topic after this consumer start will be read.
   *
   * - A string-literal representing a timestamp of the form "YYYY-MM-DD hh:mm:ss",
   *   meaning that the consumer will start reading messages from the topic which have
   *   timestamps (i.e. were written at times) greater-than-or-equal to the specified timestamp.
   *
   * - A string-literal representing non-negative integer,
   *   meaing that the consumer will start reading from the specified offset;
   *   i.e. the offset for each partition of the topic will be set to the given value.
   *
   *   This is probably not actually useful, as it sets the offset for each/every
   *   partition of the topic explicitly to the given offset; and generally it's not
   *   necessarily the right thing to set the offset for each partion to the same value.
   *
   *   We generally want to either start from the beginning, the end, from a specific
   *   timestamp, or from where we left off,
   *
   * - The string-literal "none",
   *   meaning the default behavior, which is to not set the offset at all, so that, by default,
   *   we should start reading from where we left off (from the last read/processed/committed offset),
   *   based on the consumer group; though this is true even when not using consumer group.
   *
   * - Anything which is not one of the above, in which case this call is a no-op. 
   */
  public void setOffset(String value) {
    value = StringUtils.defaultString(value).trim();
    if (!value.equals("none")) {
      if (value.equals("beginning") || value.equals("begin") || value.equals("start") || value.equals("earliest")) {
        setOffsetBeginning();
      } else if (value.equals("end") || value.equals("latest")) {
        setOffsetEnd();
      } else {
        Long offset = To.Long(value, null);
        if (offset != null) {
            setOffset(offset);
        }
        else {
          setOffset(toDate(value, null));
        }
      }
    }
    else {
      offset = null;
    }
  }

  /*
   * Request to set the offsets for each partition of the Kafka input topic to (begin at) messages
   * whose timestamps are the earliest which are greater-than-or-equal to the given. For example,
   * if you want to (re)process messages from time T (e.g. because your process crashed at that
   * time), call this method with a value of time T, and the Kafka input will *begin* reading the
   * topic (partitions) with messages (written) with timestamps equal to or greater thant time T.
   */
  public void setOffset(Date value) {
    offsetTimestamp = value;
  }

  public void setOffsetBeginning() {
    offset = TOPIC_OFFSET_BEGINNING;
  }

  public void setOffsetEnd() {
    offset = TOPIC_OFFSET_END;
  }

  public void setOffset(long value) {
    if (value >= 0) {
        offset = value;
    }
  }

  public void setProperties(String propertiesFile) throws IOException {
    Properties properties = new Properties();
    properties.load(new FileInputStream(propertiesFile));
    setProperties(properties);
  }

  public void setProperties(Map<String, Object> propertiesValues) {
    Properties properties = new Properties();
    properties.putAll(propertiesValues);
    setProperties(properties);
  }

  public void setProperties(Properties propertiesValues) {
    properties = new Properties();
    properties.putAll(propertiesValues);
  }

  public void setPollInterval(long value) {
    pollInterval.set(value >= 0L ? value : 1000L);
  }
  
  @Override
  public String toString() {
     return "kafka:" + topic;
  }

  @Override
  public void open() throws Exception {
    if (consumer != null) {
      close();
    }
    consumer = createConsumer();
    readCount = 0;
  }

  @Override
  public synchronized void close() {
    shutdownConsumer();
    if (consumer != null) {
      consumer.close();
      consumer = null;
    }
  }

  @Override
  public DigNode read() throws Exception {

    if (isExited.get()) {
      shutdownConsumer();
      return DigInput.END_INPUT;
    }

    if (isPaused) {
      Thread.sleep(100);
      return DigInput.NO_INPUT;
    }

    final long readTimestamp = readStats.noteStart();

    if ((iterator == null) || !iterator.hasNext()) {
      try {
        records = consumer.poll(pollInterval.get());

      } catch (WakeupException e) {
        //
        // We a closing, cleanly, via Consumer.wakeup.
        // Normally we don't ever exit, but note anyway in case there is a hook to do so.
        //
        if (isExited.get()) {
          shutdownConsumer();
          return DigInput.END_INPUT;

        } else {
          log("Wakeup error.", e);
        }

      } catch (Exception e) {
        log("Polling error.", e);
      }

      readCount += records.count();
      iterator = records.records(topic).iterator();
    }

    if (!iterator.hasNext()) {
      readStats.noteCancel();
      return DigInput.NO_INPUT;
    }

    ConsumerRecord<String, String> record = iterator.next();

    if (record == null) {
      return DigInput.NO_INPUT;
    }

    readStats.noteEnd(readTimestamp);

    DigNode data = consumerRecordToJson(record);

    readLastOffset = record.offset();
    readLastPartition = record.partition();
    readLastTimestamp = record.timestamp();
    readLastRecord = data;

    if (includeMetadata) {
        DigNode wrappedData = new DigNode();
        DigNode metadata = wrappedData.create("metadata");
        metadata.put("timestamp", readLastTimestamp);
        metadata.put("datetime", toTimestampString(readLastTimestamp));
        metadata.put("topic", topic);
        if (!groupId.equals("none")) {
          metadata.put("group", groupId);
        }
        metadata.put("partition", readLastPartition);
        metadata.put("offset", readLastOffset);
        TopicPartition partition = findAssignedPartition(readLastPartition);
        if (partition != null) {
          OffsetAndMetadata commitedMetadata = consumer.committed(partition);
          if (commitedMetadata != null) {
            metadata.put("committed", commitedMetadata.offset());
          }
        }
        wrappedData.put("data", data);
        return wrappedData;
    }

    return data;
  }

  // TODO
  // Make the usage of this a simple inline lambda function call.
  //
  private TopicPartition findAssignedPartition(long partition) {
      synchronized (this) {
        if (assignedPartitions != null)
          for (TopicPartition p : assignedPartitions) {
            if (p.partition() == partition) {
              return p;
            }
          }
      }
      return null;
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


  private KafkaConsumer<String, String> createConsumer() throws Exception {
	  
	  if(this.properties == null) {
		  //TODO: change to DigException
		  throw new IllegalArgumentException("Properities for this input cannot be null. Please set them before creating a consumer");
	  }


    if (!groupId.equals("none")) {
      //
      // Note that, based on observation, even if the groupId is empty we are still a
      // part of a consumer group, I guess/think by virtue of the consumer.subscribe call;
      // as opposed to calling consumer.assign which is done, here, only if the groupId is
      // explicitly specified as "none"; This usage - groupId of "none" - where we *assign*
      // topic partitions rather than *subscribe* to a topic is used only if we know there
      // will only be a single consumer; but even there is only a single consumer it doesn't
      // hurt (and is perhaps beneficial) if we act as a part of a (single) group.
      //
      // Also, ran into issue (only) with Kafka Eventador (in contrast to Kafka Confluent Cloud)
      // where specifying an empty groupId yields this exception:
      // org.apache.kafka.common.errors.InvalidGroupIdException: The configured groupId is invalid
      //
      // So, probably not a good idea to have an empty groupId anyways,
      // so if it's empty then just force it to be something, say, the full
      // name of this class (com.cartera.dig.common.io.input.DigKafkaInput).
      //
      properties.put(ConsumerConfig.GROUP_ID_CONFIG, groupId);
    }

    // The auto.offset.reset property defines the consumer offset behavior when there
    // is no initial offset. Note that this seems to work even when the consumer is not
    // named as a member of group, which I found slightly surprising; i.e. it (the Kafka
    // server/cluster) remembers the offset where an unnamed consumer left off.
    //
    if (autoOffsetReset.length() > 0) {
      properties.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, autoOffsetReset);
    }

    // The max.poll.records property defines the maximum number of records (messages)
    // returned by each consumer poll method call. If the processing time of each record
    // is expected to be long (e.g. more then 1000ms) then this should be set low so that
    // not so much time passes between calls to the poll method.
    //
    if (maxPollRecords > 0) {
      properties.put(ConsumerConfig.MAX_POLL_RECORDS_CONFIG, maxPollRecords);
    }

    properties.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG,
        StringDeserializer.class.getName());
    properties.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG,
        StringDeserializer.class.getName());

    KafkaConsumer<String, String> consumer = new KafkaConsumer<>(properties);

    // When using consumer groups you "subscribe" to a topic; when not using consumer
    // groups you assign yourself to partitions in the topic; you cannot do both.

    List<TopicPartition> partitions = null;

    if (!groupId.equals("none")) {
      consumer.subscribe(Arrays.asList(topic), new PartitionRebalanceListener(this));
    } else {
      partitions = getPartitions(consumer, topic);
      consumer.assign(partitions);
    }

    // Set the offset specifically only if explicitly specified.
    //
    if (offsetTimestamp != null) {
      if (partitions == null) {
        partitions = getPartitions(consumer, topic);
      }
      Map<TopicPartition,Long> offsets =
          getOffsetsForTimestamp(consumer, partitions, offsetTimestamp);
      if (offsets != null) {
        ConsumerRecords<String, String> dummy = consumer.poll(0);
        offsetTimestampMap = new HashMap<>();
        offsets.forEach((partition, offset) -> {
          if ((partition != null) && (offset != null)) {
            offsetTimestampMap.put(partition.partition(), offset);
            consumer.seek(partition, offset);
          }
        });
      }
    } else if (offset != null) {
      if (partitions == null) {
        partitions = getPartitions(consumer, topic);
      }
      if (offset == TOPIC_OFFSET_BEGINNING) {
        //
        // TODO
        // Since subscribe (and assign) to topics is lazy, need a dummy poll call
        // before doing any seeking; so says the Interwebs, but don't understand,
        // how do we know (is it true) that this dummy poll will return no records?
        //
        // NOTE
        // When running a second consumer on the same topic in the same conumer
        // group (including an empty group ID), and trying to explicitly set the
        // offset (to beginning, end, or given offset value), we get an exception like this:
        // Caused by: java.lang.IllegalStateException: No current assignment for partition topic-8
        //
        ConsumerRecords<String, String> dummy = consumer.poll(0);
        consumer.seekToBeginning(partitions);
      } else if (offset == TOPIC_OFFSET_END) {
        ConsumerRecords<String, String> dummy = consumer.poll(0);
        consumer.seekToEnd(partitions);
      } else if (offset >= 0) {
        //
        // NOTE
        // This may be problematic with multiple consumers in a group.
        //
        // And this - explicitly setting offsets (for all partitions)
        // for a topic - is probably not actually useful. We generally
        // want to either start from the beginning, the end, from where
        // we left off, or from a specific time - see setOffset(Date).
        //
        ConsumerRecords<String, String> dummy = consumer.poll(0);
        for (TopicPartition partition : partitions) {
          consumer.seek(partition, offset);
        }
      }
    }

    // Attempt to catch shutdowns in order to close consumer.
    //
    Runtime.getRuntime().addShutdownHook(new ShutdownHook(this));

    return consumer;
  }

  private List<TopicPartition> getPartitions(Consumer<String, String> consumer, String topic) {
    List<TopicPartition> partitions = new ArrayList<TopicPartition>();
    List<PartitionInfo> partitionInfos = consumer.partitionsFor(topic);
    for (PartitionInfo partitionInfo : partitionInfos) {
      TopicPartition partition =
          new TopicPartition(partitionInfo.topic(), partitionInfo.partition());
      partitions.add(partition);
    }
    return partitions;
  }

  private DigNode consumerRecordToJson(ConsumerRecord<String, String> record) {
    try {
      String value = record.value();
      if (encryptionService != null) {
        value = encryptionService.decrypt(value);
      }
      return DigNode.fromJson(value);
    } catch (Exception e) {
      //
      // TODO
      // Is this the/a right thing to do?
      // Means the caller need to be aware they may get back a DigNode representing
      // a message parse error. Other options: just let the exception propogate up
      // and out back to the caller, and/or catch and throw more specifc exception.
      //
      DigNode json = DigNode.create();
      json.put("error", e.toString());
      json.put("data", record.toString());
      return json;
    }
  }

  private Map<TopicPartition,Long> getOffsetsForTimestamp(Consumer<?,?> consumer, List<TopicPartition> partitions, Date offsetTimestamp) {
    if ((consumer == null) || (offsetTimestamp == null)) {
        return null;
    }
    long timestamp = offsetTimestamp.getTime();
    final Map<TopicPartition,Long> timestampMap = new HashMap<>();
    for (final TopicPartition topicPartition: partitions) {
      timestampMap.put(topicPartition, timestamp);
    }
    final Map<TopicPartition, OffsetAndTimestamp> offsetMap = consumer.offsetsForTimes(timestampMap);
    final Map<TopicPartition, Long> partitionOffsetMap = new HashMap<>();
    for (Map.Entry<TopicPartition, OffsetAndTimestamp> entry: offsetMap.entrySet()) {
      if ((entry.getKey() != null) && (entry.getValue() != null)) {
        partitionOffsetMap.put(entry.getKey(), entry.getValue().offset());
      }
    }
    return partitionOffsetMap;
  }

  private void shutdownConsumer() {
    synchronized (this) {
      if (consumer != null) {        
        consumer.close();
        consumer = null;
      }
    }
  }

  @Override
  public DigNode status(Args args) {
    Date now = new Date();
    DigNode status = DigNode.create();
    status.put("class", this.getClass().getName());
    status.put("timestamp", To.IsoDate(now));
    status.put("connection", properties.getProperty("bootstrap.servers"));
    status.put("topic", topic);
    status.put("group", groupId);
    status.put("autoOffsetReset", autoOffsetReset);
    status.put("maxPollRecords", maxPollRecords);
    if (offsetTimestamp != null) {
      status.put("offsetTimestamp", To.IsoDate(offsetTimestamp));
      status.put("offsetTimestampMap", offsetTimestampMap.toString());
    } else if (offset != null) {
      if (offset == TOPIC_OFFSET_BEGINNING)
        status.put("offset", "beginning");
      else if (offset == TOPIC_OFFSET_END)
        status.put("offset", "end");
      else
        status.put("offset", offset);
    }
    status.put("pollInterval", pollInterval.get());
    status.put("readStats", readStats.status());
    status.put("readCount", readCount);
    status.put("readLastOffset", readLastOffset);
    status.put("readLastPartition", readLastPartition);
    status.put("readLastTimestamp", readLastTimestamp);
    status.put("readLastRecord", readLastRecord);
    status.put("readBufferedCount", readCount - readStats.totalCount());
    synchronized (this) {
      if (assignedPartitions != null) {
        DigNode.Array statusPartitions = status.createArray("assignedPartitions");
        assignedPartitions.forEach(partition -> {
          statusPartitions.add(partition.toString());
        });
      }
    }
    long kafkaSetPollInterval = To.Long(args.get("kafkasetpollinterval"), -1L);
    if (kafkaSetPollInterval >= 0L) {
      setPollInterval(kafkaSetPollInterval);
    }
    int kafkaSetMaxPollRecords = To.Int(args.get("kafkasetmaxpollrecords"), 0);
    if (kafkaSetMaxPollRecords > 0) {
      setMaxPollRecords(kafkaSetMaxPollRecords);
    }
    if (To.Int(args.get("kafkainputtopicinfo"), 0) == 1) {
      KafkaTopicInfo kafkaTopicInfo = new KafkaTopicInfo(properties);
      status.put("topicInfo", kafkaTopicInfo.topicStatus(topic));
    }
    if (To.Int(args.get("kafkaexit"), 0) == 1) {
      //
      // TODO
      // Dangerous-ish hook for testing.
      // Note that Consumer.wakeup, the mechanism/method used to cause a consumer
      // to exit, is the only Kafka consumer API safe to call from another thread.
      //
      isExited.set(true);
      if (consumer != null) {
        consumer.wakeup();
      }
    }
    return status;
  }

  // This ConsumerRebalanceListener implementation is *only* (currently) used to
  // track the currently assigned topic partitions, and is *only* for informational
  // purposes (i.e. returned in a DigStatus.status call). This has proved useful in
  // debugging, monitoring, and general understanding of consumer groups and partitions.
  //
  // There is a consumer.assignment method which can be used to get this info,
  // but since a consumer cannot be referenced from another thread, e.g. from
  // a status-port request/thread, this cannot be used.
  //
  private static class PartitionRebalanceListener implements ConsumerRebalanceListener {
    private DigKafkaInput kafkaInput;

    public PartitionRebalanceListener(DigKafkaInput input) {
      kafkaInput = input;
    }

    @Override
    public void onPartitionsRevoked(Collection<TopicPartition> partitions) {}

    @Override
    public void onPartitionsAssigned(Collection<TopicPartition> partitions) {
      synchronized (kafkaInput) {
        kafkaInput.assignedPartitions = new ArrayList<TopicPartition>(partitions);
      }
    }
  }

  private static class ShutdownHook extends Thread {
    private DigKafkaInput input;

    public ShutdownHook(DigKafkaInput value) {
      input = value;
    }

    @Override
    public void run() {
      input.isExited.set(true);
      synchronized (input) {
        if (input.consumer != null) {
          input.consumer.wakeup();
        }
      }
    }
  }

  /*
   * TODO
   * Move to util.To or something.
   */
  private static Date toDate(String value, Date fallback) {
    try {
      SimpleDateFormat dateFormat= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
      return dateFormat.parse(value);
    } catch (Exception e) {
      return fallback;
    }
  }

  private static String toTimestampString(long timestamp) {
    try {
      SimpleDateFormat dateFormat= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
      return dateFormat.format(new Date(timestamp));
    } catch (Exception e) {
      return "";
    }
  }

  private static void log(String message, Exception e) {
    log.error(message, e);
  }

}
