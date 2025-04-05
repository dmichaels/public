package com.cartera.dig.app.tools;

import com.cartera.dig.common.DigNode;
import com.cartera.dig.common.kafka.KafkaTopicInfo;
import com.cartera.dig.common.util.To;
import com.cartera.dig.common.security.AesCtrEncryptionService;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Spliterator;
import java.util.Spliterators;
import java.util.stream.Collectors;
import java.util.stream.Stream;
import java.util.stream.StreamSupport;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.common.PartitionInfo;
import org.apache.kafka.common.TopicPartition;

import static java.util.Map.Entry.comparingByKey;
import static java.util.stream.Collectors.toList;

/**
 * Utility to get sundry Kafka topic info. Usages described below.
 *
 * § Print a list of all topics names in alphabetical order.
 *   Required Arguments: None
 *   Optional Arguments:
 *    --verbose: Include partition and message count.
 *
 * § Print the topic partition and message counts.
 *   Required Arguments:
 *     --topic topic: The topic name.
 *   Optional Arguments:
 *     --verbose:     Include beginning/end offsets and message count for each partition in the topic.
 *     --timestamps:  Includes above info plus timestamps for beginning/end messages for each partition.
 *     --messages:    Includes above info plus actual content of beginning/end messages for each partition.
 *
 * § Print the earliest offset for each partition for the topic whose
 *   timestamp is greater than or equal to the given timestamp.
 *   Required Arguments:
 *     --topic topic: The topic name.
 *     --timestamp T: The timestamp described about; in the form: "YYYY-MM-DD hh:mm:ss".
 *   Optional Arguments:
 *     --messages:    Includes the actual content of each message corresponding to the displayed offset.
 *
 * § Print the message timestamp and optionally the actual content
 *   of the message at the given offset in the given partition for the topic.
 *   Required Arguments:
 *     --topic topic:         The topic name.
 *     --partition partition: The partition number.
 *     --offset offset:       The partition offset.
 *   Optional Arguments:
 *     --messages:            The partition offset.
 *
 * § Print the committed offsets for the given consumer group for the topic.
 *   TODO? How to do this without actually subscribing to a topic with a group ID
 *   without causing a rebalance for the group, i.e. thereby altering/interfering
 *   other running consumers in that group for the topic.
 *
 * All usages require the Kafka connection info be specified using one of the following options:
 * If not specified then the connection will be assumed to be "localhost:9002".
 *
 *   --connection host:port
 *   --properties file
 */
public class MainKafkaInfo {

    public Object run(String[] args) throws Exception {

        String topic = "";
        String connection = "";
        String properties = "";
        boolean verbose = false;
        boolean includeMessageTimestamps = false;
        boolean includeMessages = false;
        Date timestamp = null;
        Integer atPartition = null;
        Long atOffset = null;
        AesCtrEncryptionService encryption = null;
        String committedOffsetsForGroup = "";
        KafkaTopicInfo topicInfo = new KafkaTopicInfo();

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("--connection") || args[i].equals("-c")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo input connection usage error.");
                }
                connection = args[i];

            } else if (args[i].equals("--properties") || args[i].equals("-p")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo input properties usage error.");
                }
                properties = args[i];

            } else if (args[i].equals("--topic") || args[i].equals("-t")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo input topic usage error.");
                }
                topic = args[i];

            } else if (args[i].equals("--verbose")) {
                verbose = true;

            } else if (args[i].equals("--timestamps")) {
                verbose = true;
                includeMessageTimestamps = true;

            } else if (args[i].equals("--messages") || args[i].equals("--message")) {
                verbose = true;
                includeMessageTimestamps = true;
                includeMessages = true;

            } else if (args[i].equals("--timestamp")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo input timestamp usage error.");
                }
                SimpleDateFormat dateFormat= new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                verbose = true;
                timestamp = dateFormat.parse(args[i]);

            } else if (args[i].equals("--partition")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo partition usage error.");
                }
                atPartition = To.Int(args[i], null);

            } else if (args[i].equals("--offset")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo offset usage error.");
                }
                atOffset = To.Long(args[i], null);

            } else if (args[i].equals("--committed")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo committed usage error.");
                }
                //
                // TODO
                //
                committedOffsetsForGroup = args[i];

            } else if (args[i].equals("--encryption")) {
                if (++i == args.length) {
                    throw new Exception("KafkaInfo encryption usage error.");
                }
                String encryptionKey = args[i];
                encryption = new AesCtrEncryptionService(encryptionKey);
            }
        }

        if (verbose) {
            System.out.println("DIG KAFKA INFO UTILITY:");
        }

        if (connection.length() > 0)  {
            topicInfo.setConnection(connection);
        }
        else if (properties.length() > 0) {
            topicInfo.setProperties(properties);
        }
        else {
            connection = "localhost:9092";
            topicInfo.setConnection(connection);
        }

        if (verbose) {
            System.out.println("Connection: " + connection);
        }

        if (topic.length() == 0) {
            //
            // No topic given; just list all topics, alphabetically.
            //
            Map<String,List<PartitionInfo>> topics = topicInfo.topicList();
            Map<String,List<PartitionInfo>> sortedTopics =
                topics.entrySet().stream().sorted(comparingByKey())
                    .collect(Collectors.toMap(e -> e.getKey(), e -> e.getValue(), (k,v) -> v, LinkedHashMap::new));
            for (Map.Entry<String,List<PartitionInfo>> topicEntry : sortedTopics.entrySet()) {
                System.out.printf("%s\n", topicEntry.getKey());
                if (verbose) {
                    System.out.printf("  Partitions: %d\n", topicEntry.getValue().size());
                    DigNode status = topicInfo.topicStatus(topicEntry.getKey(), false, false);
                    System.out.printf("  Messages:   %d\n", status.get("messageCount").asInt());
                }
            }
            System.exit(0);
        }

        // Here, a specific topic has been specified.

        KafkaTopicInfo.TopicInfo info = topicInfo.topicInfo(topic);
        System.out.printf("Topic: %s\n", info.topic);
        System.out.printf("Messages: %d\n", info.messageCount);
        System.out.printf("Partitions: %d\n", info.partitions.size());

        if ((atPartition != null) && (atOffset != null)) {
            //
            // Get the timestamp and optionally the message
            // content for the specific partition and offset.
            //
            KafkaTopicInfo.TopicPartitionInfo partition = info.partitions.get(atPartition);
            ConsumerRecord<String,String> record =
                topicInfo.getSingleRecord(partition.partition, atOffset);
            System.out.printf("Partition-%02d: Offset %d\n", partition.partition.partition(), atOffset);
            if (record != null) {
                System.out.printf("              Timestamp: %s (%d)\n", DateTimeString(record.timestamp()), record.timestamp());
                if (includeMessages) {
                    if (encryption != null) {
                        System.out.printf("              Message: [%s]\n", encryption.decrypt(record.value()));
                    }
                    else {
                        System.out.printf("              Message: [%s]\n", record.value());
                    }
                }
            }
            else {
                System.out.printf("              NULL-RECORD\n");
            }
            System.exit(0);
        }

        if (!verbose) {
            System.exit(0);
        }

        if (timestamp != null) {
            //
            // Get offsets and optionally the message corresponding to the given
            // timestamp, i.e. where the offset is the earliest whose timestamp
            // is greater-than-or-equal to the given timestamp.
            //
            Map<TopicPartition,Long> offsets = topicInfo.getOffsetsForTimestamp(topic, timestamp);
            if (offsets != null) {
                System.out.printf("Minimum Timestamp: %s (%d)\n", DateTimeString(timestamp), timestamp.getTime());
                List<TopicPartition> sortedOffsets = offsets.keySet().stream().sorted((a,b) -> a.partition() - b.partition()).collect(toList());
                for (TopicPartition partition : sortedOffsets) {
                    Long offset = offsets.get(partition);
                    System.out.printf("Partition-%02d: Offset %d\n", partition.partition(), offset);
                    if (includeMessages || includeMessageTimestamps) {
                        try {
                            ConsumerRecord<String,String> record = topicInfo.getSingleRecord(partition, offset);
                            System.out.printf("              Timestamp: %s (%d)\n", DateTimeString(record.timestamp()), record.timestamp());
                            if (encryption != null) {
                                System.out.printf("              Message:   [%s]\n", encryption.decrypt(record.value()));
                            }
                            else {
                                System.out.printf("              Message:   [%s]\n", record.value());
                            }
                        } catch (Exception e) {
                            System.err.println(e.toString());
                        }
                    }
                }
            }
            System.exit(0);
        }

        Stream<KafkaTopicInfo.TopicPartitionInfo> partitions =
            StreamSupport.stream(Spliterators
                .spliteratorUnknownSize(info.partitions.iterator(), Spliterator.ORDERED), false);

        List<KafkaTopicInfo.TopicPartitionInfo> partitionInfoList = partitions.collect(toList());

        long minRecordTimestamp = Long.MAX_VALUE;
        long maxRecordTimestamp = 0;

        for (KafkaTopicInfo.TopicPartitionInfo partitionInfo : partitionInfoList) {
            System.out.printf("Partition-%02d: ", partitionInfo.partition.partition());
            System.out.printf("Messages:   %d\n", partitionInfo.messageCount);
            System.out.printf("              Offsets:    %d — %d\n", partitionInfo.firstOffset, partitionInfo.lastOffset);
            if (includeMessageTimestamps) {
                if (partitionInfo.messageCount > 1) {
                    System.out.printf("              Timestamps:");
                    ConsumerRecord<String,String> firstRecord = null;
                    ConsumerRecord<String,String> lastRecord = null;
                    try {
                        firstRecord = topicInfo.getSingleRecord(partitionInfo.partition, partitionInfo.firstOffset);
                        long firstRecordTimestamp = firstRecord.timestamp();
                        System.out.printf(" %s (%d) —\n", DateTimeString(firstRecordTimestamp), firstRecordTimestamp);
                        if (firstRecordTimestamp < minRecordTimestamp) {
                            minRecordTimestamp = firstRecordTimestamp;
                        }
                    } catch (Exception e) {
                        System.err.printf
                            ("Error reading single/first Kafka message (topic: %s, partition: %d, offset: %d)",
                                topic, partitionInfo.partition.partition(), partitionInfo.firstOffset);
                    }
                    try {
                        lastRecord = topicInfo.getSingleRecord(partitionInfo.partition, partitionInfo.lastOffset - 1);
                        long lastRecordTimestamp = lastRecord.timestamp();
                        System.out.printf("                          %s (%d)\n",
                            DateTimeString(lastRecordTimestamp), lastRecordTimestamp);
                        if (lastRecordTimestamp > maxRecordTimestamp) {
                            maxRecordTimestamp = lastRecordTimestamp;
                        }
                    } catch (Exception e) {
                        System.err.printf
                            ("Error reading single/last Kafka message (topic: %s, partition: %d, offset: %d)",
                                topic, partitionInfo.partition.partition(), partitionInfo.lastOffset - 1);
                    }
                    if (includeMessages) {
                        if (firstRecord != null) {
                            if (encryption != null) {
                                System.out.printf("              First:      [%s]\n", encryption.decrypt(firstRecord.value()));
                            }
                            else {
                                System.out.printf("              First:      [%s]\n", firstRecord.value());
                            }
                        }
                        if (lastRecord != null) {
                            if (encryption != null) {
                                System.out.printf("              Last:       [%s]\n", encryption.decrypt(lastRecord.value()));
                            }
                            else {
                                System.out.printf("              Last:       [%s]\n", lastRecord.value());
                            }
                        }
                    }
                }
                else if (partitionInfo.messageCount == 1) {
                    System.out.printf("              Timestamp:");
                    ConsumerRecord<String,String> firstRecord = null;
                    try {
                        firstRecord = topicInfo.getSingleRecord(partitionInfo.partition, partitionInfo.firstOffset);
                        long firstRecordTimestamp = firstRecord.timestamp();
                        System.out.printf("  %s (%d)\n", DateTimeString(firstRecordTimestamp), firstRecordTimestamp);
                    } catch (Exception e) {
                        System.err.printf
                            ("Error reading single Kafka message (topic: %s, partition: %d, offset: %d)",
                                topic, partitionInfo.partition.partition(), partitionInfo.firstOffset);
                    }
                    if (includeMessages) {
                        if (firstRecord != null) {
                            if (encryption != null) {
                                System.out.printf("              First/Last: [%s]\n", encryption.decrypt(firstRecord.value()));
                            }
                            else {
                                System.out.printf("              First/Last: [%s]\n", firstRecord.value());
                            }
                        }
                    }
                }
            }
        }
        if (minRecordTimestamp < Long.MAX_VALUE) {
            System.out.printf
                ("Timestamps:   %s (%d)",
                    DateTimeString(minRecordTimestamp), minRecordTimestamp);
            if (maxRecordTimestamp > 0) {
                System.out.printf
                    (" —\n              %s (%d)\n",
                        DateTimeString(maxRecordTimestamp), maxRecordTimestamp);
            }
        }

        return null;
    }

    private static String DateTimeString(Date value) {
        if (value == null) {
            return "";
        }
        return new java.text.SimpleDateFormat("yyyy-MM-dd' 'HH:mm:ss").format(value);
    }

    private static String DateTimeString(long timestamp) {
        return DateTimeString(new Date(timestamp));
    }
}
