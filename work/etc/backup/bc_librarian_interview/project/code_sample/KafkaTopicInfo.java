package com.cartera.dig.common.kafka;

import com.cartera.dig.common.DigNode;
import com.cartera.dig.common.util.ConnectionInfo;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import lombok.extern.slf4j.Slf4j;
import org.apache.kafka.clients.consumer.Consumer;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.consumer.OffsetAndMetadata;
import org.apache.kafka.clients.consumer.OffsetAndTimestamp;
import org.apache.kafka.common.PartitionInfo;
import org.apache.kafka.common.TopicPartition;
import org.apache.kafka.common.serialization.StringDeserializer;

@Slf4j
public class KafkaTopicInfo {

    private Properties      properties;
    private String          servers         = "localhost:9092";
    private ConnectionInfo  connectionInfo  = new ConnectionInfo(servers);

    public KafkaTopicInfo() {
    }

    public KafkaTopicInfo(Properties value) {
        properties = value;
    }

    public void setConnection(String value) {
        //
        // TODO
        //
        connectionInfo = new ConnectionInfo(value);
        servers = connectionInfo.getHost() + ":" + connectionInfo.getPort();
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
    
    public Properties getProperties() {
        return properties;
    }

    public String getServers() {
        return servers;
    }

    public ConnectionInfo getConnectionInfo() {
        return connectionInfo;
    }

    public DigNode topicStatus(String topic) {
        return topicStatus(topic, true, false);
    }

    public DigNode topicStatus(String topic, boolean includePartitions, boolean includeMessageTimestamps) {

        // FYI
        // Tried passing in consumer, when called from
        // DigKafkaInput, but get this exception on any access:
        // java.util.ConcurrentModificationException:
        // KafkaConsumer is not safe for multi-threaded access

        try {

            // Just seeing what kind of possibly interesting topic info we can easily get;
            // get the the apparent current message count in the topic by taking the maximum value
            // of the difference between the end-offset and beginning-offset of each partition,
            // and summing them up ... I *think* this is right ... But it's because I'm still
            // not 100% sure the we refer to it as "apparent" message count.

            DigNode status = DigNode.create();
            status.put("topic", topic);
            long totalMessageCount = 0;
            Consumer<String, String> consumer = createConsumer();
            List<TopicPartition> partitions = getPartitions(consumer, topic);
            DigNode.Array statusBeginningOffsets = status.createArray("beginningOffsets");
            Map<TopicPartition, Long> beginningOffsets = consumer.beginningOffsets(partitions);
            for (Map.Entry<TopicPartition, Long> entry : beginningOffsets.entrySet()) {
                DigNode statusBeginningOffset = statusBeginningOffsets.add();
                TopicPartition partition = entry.getKey();
                Long offset = entry.getValue();
                statusBeginningOffset.put("topic", partition.topic());
                statusBeginningOffset.put("partition", partition.partition());
                statusBeginningOffset.put("offset", offset);
            }
            DigNode.Array statusEndOffsets = status.createArray("endOffsets");
            Map<TopicPartition, Long> endOffsets = consumer.endOffsets(partitions);
            for (Map.Entry<TopicPartition, Long> entry : endOffsets.entrySet()) {
                DigNode statusEndOffset = statusEndOffsets.add();
                TopicPartition partition = entry.getKey();
                Long offset = entry.getValue();
                Long correspondingBeginningOffset = beginningOffsets.get(partition);
                Long messageCount = null;
                if ((correspondingBeginningOffset != null) && (offset != null) && (offset >= correspondingBeginningOffset)) {
                    statusEndOffset.put("beginningOffset", correspondingBeginningOffset);
                    messageCount = offset - correspondingBeginningOffset;
                    statusEndOffset.put("messageCount", messageCount);
                    totalMessageCount += messageCount;
                }
                statusEndOffset.put("topic", partition.topic());
                statusEndOffset.put("partition", partition.partition());
                statusEndOffset.put("offset", offset);
            }
            status.put("messageCount", totalMessageCount);
            if (includePartitions) {
                DigNode.Array statusPartitions = status.createArray("partitions");
                consumer.assign(partitions);
                for (TopicPartition partition : partitions) {
                    DigNode statusPartition = statusPartitions.add();
                    status.put("topic", partition.topic());
                    statusPartition.put("name", partition.toString());
                    statusPartition.put("position", consumer.position(partition));
                    OffsetAndMetadata metadata = consumer.committed(partition);
                    if (metadata != null) {
                        statusPartition.put("committedOffset", consumer.committed(partition).offset());
                    }
                }
            }
            return status;

        } catch (Exception e) {
            log.error("Unexpected error retreiving topicStatus" + e.getMessage(), e );
            return null;
        }
    }

    // TODO: Refactor. Obviously.
    // Used only for status, troubleshooting, utility purposes.
    //
    public static class TopicInfo
    {
        public String                   topic;
        public long                     messageCount;
        public List<TopicPartitionInfo> partitions = new ArrayList<TopicPartitionInfo>();
    }

    public static class TopicPartitionInfo
    {
        public String         topic;
        public Long           firstOffset;
        public Long           lastOffset;
        public Long           messageCount;
        public TopicPartition partition;
    }

    public TopicInfo topicInfo(String topic) throws Exception {

        TopicInfo info = new TopicInfo();
        info.topic = topic;

        Consumer<String, String> consumer = createConsumer();
        List<TopicPartition> partitions = getPartitions(consumer, topic);
        consumer.assign(partitions);

        Map<TopicPartition,Long> beginningOffsets = consumer.beginningOffsets(partitions);
        Map<TopicPartition,Long> endOffsets = consumer.endOffsets(partitions);
        consumer.close();

        for (TopicPartition partition : partitions) {
            TopicPartitionInfo partitionInfo = new TopicPartitionInfo();
            info.partitions.add(partitionInfo);
            partitionInfo.topic = topic;
            partitionInfo.partition = partition;
            partitionInfo.firstOffset = beginningOffsets.get(partition);
            partitionInfo.lastOffset = endOffsets.get(partition);
            if ((partitionInfo.firstOffset != null) && (partitionInfo.lastOffset != null)) {
                partitionInfo.messageCount = partitionInfo.lastOffset - partitionInfo.firstOffset;
                info.messageCount += partitionInfo.messageCount;
            }
        }

        info.partitions.sort((a,b) -> a.partition.partition() - b.partition.partition());

        return info;
    }

    public Map<String,List<PartitionInfo>> topicList() throws Exception {
        Consumer<String, String> consumer = createConsumer();
        Map<String, List<PartitionInfo>> topics = consumer.listTopics();
        return topics;
    }

    public Map<TopicPartition, Long> getOffsetsForTimestamp(String topic, Date date) throws Exception {
        //
        // TODO: Use Java 8 date/time.
        //
        Consumer<String, String> consumer = createConsumer();
        List<TopicPartition> partitions = getPartitions(consumer, topic);
        Map<TopicPartition, Long> offsets = getOffsetsForTimestamp(consumer, partitions, date.getTime());
        return offsets;
    }

    public Map<TopicPartition, Long> getOffsetsForTimestamp(String topic, long timestamp) throws Exception {
        Consumer<String, String> consumer = createConsumer();
        List<TopicPartition> partitions = getPartitions(consumer, topic);
        Map<TopicPartition, Long> offsets = getOffsetsForTimestamp(consumer, partitions, timestamp);
        return offsets;
    }

    private Map<TopicPartition,Long> getOffsetsForTimestamp(Consumer<?, ?> consumer, List<TopicPartition> partitions, long timestamp) {
        final Map<TopicPartition, Long> timestampMap = new HashMap<>();
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

    public String getSingleMessage(TopicPartition partition, long offset) throws Exception {
        ConsumerRecord<String, String> record = getSingleRecord(partition, offset);
        return (record != null) ? record.value() : "";
    }

    public ConsumerRecord<String, String> getSingleRecord(TopicPartition partition, long offset) throws Exception {
        Consumer<String, String> consumer = createConsumer();
        consumer.assign(Arrays.asList(partition));
        if (offset < 0) {
            consumer.seekToBeginning(Arrays.asList(partition));
        }
        else {
            consumer.seek(partition, offset);
        }
        for (int i = 0; i < 10; i++) {
            ConsumerRecords<String, String> records = consumer.poll(500L);
            if (records != null) {
                Iterator<ConsumerRecord<String,String>> iterator = records.iterator();
                if (iterator.hasNext()) {
                    ConsumerRecord<String, String> record = iterator.next();
                    consumer.close();
                    return record;
                }
            }
        }
        return null;
    }

    Consumer<String, String> createConsumer() throws Exception {
        if (properties == null) {
            properties = new Properties();
            properties.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, servers);
        }
        properties.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        properties.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        return new KafkaConsumer<>(properties);
    }

    List<TopicPartition> getPartitions(Consumer<String, String> consumer, String topic) {
        List<TopicPartition> partitions = new ArrayList<TopicPartition>();
        List<PartitionInfo> partitionInfos = consumer.partitionsFor(topic);
        if(partitionInfos == null)
            return partitions;
        for (PartitionInfo partitionInfo : partitionInfos) {
            TopicPartition partition =
                new TopicPartition(partitionInfo.topic(), partitionInfo.partition());
            partitions.add(partition);
        }
        return partitions;
    }
}
