package com.cartera.dig.common.io.output;

import com.cartera.dig.common.DigNode;
import com.cartera.dig.common.kafka.KafkaTopicInfo;
import com.cartera.dig.common.security.EncryptionService;
import com.cartera.dig.common.util.Args;
import com.cartera.dig.common.util.ConnectionInfo;
import com.cartera.dig.common.util.TaskStats;
import com.cartera.dig.common.util.To;
import com.fasterxml.jackson.databind.JsonNode;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Date;
import java.util.Map;
import java.util.Properties;
import org.apache.commons.lang.StringUtils;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.common.serialization.StringSerializer;

public class DigKafkaOutput implements DigOutput {

    private static final String DEFAULT_CLIENT_ID_PREFIX = "DigKafkaOutput-";

    private String                          servers = "localhost:9092";
    private String                          clientId;
    private ConnectionInfo                  connectionInfo = new ConnectionInfo(servers);
    private String                          topic = "";
    private Properties                      properties;
    private String                          keyField = "";
    private EncryptionService               encryptionService;
    private TaskStats                       writeStats = new TaskStats();
    private DigNode                         writeLastRecord;
    private KafkaProducer<String, String>   producer;
    private boolean                         isOpen = false;

    
    public void setServers(String url) {
		servers = url;
    }

    public void setClientId(String clientId) {
        this.clientId = clientId;
    }
    
    public void setConnection(String value) {
        //
        // TODO
        //
        connectionInfo = new ConnectionInfo(value);
        servers = connectionInfo.getHost() + ":" + connectionInfo.getPort();
    }

    public void setTopic(String value) {
        topic = StringUtils.defaultString(value).trim();
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

    /**
     * Set the JSON/DigNode field whose value should be used as a key for produced
     * messages. Keys are not necessary, but (I think) can help distribute messages
     * among the written to partitions for the topic.
     * @param value The name of the JSON field whose value should be used as a key.
     */
    public void setKeyField(String value) {
        keyField = StringUtils.defaultString(value).trim();
    }

    public void setEncryptionService(EncryptionService value) {
        encryptionService = value;
    }

    @Override
    public void open() throws Exception {
        producer = createProducer();
        isOpen = true;
    }

    @Override
    public synchronized void close() {
        if (producer != null) {
            producer.flush();
            producer.close();
            isOpen = false;
        }
    }

    @Override
    public void write(DigNode data) throws Exception {

        if (!isOpen) {
            open();
        }

        final long writeTimestamp = writeStats.noteStart();

        String key = null;

        if (keyField.length() > 0) {
            JsonNode keyValue = data.get(keyField);
            key = (keyValue != null) ? StringUtils.defaultString(keyValue.asText()) : "";
        }

        String value = data.toString();

        if (encryptionService != null) {
            value = encryptionService.encrypt(value);
        }

        ProducerRecord<String, String> record =
            new ProducerRecord<String, String>(topic, key, value);

        producer.send(record);

        writeLastRecord = data;
        writeStats.noteEnd(writeTimestamp);
    }

    @Override
    public DigNode status(Args args) {
        Date now = new Date();
        DigNode status = DigNode.create();
        status.put("class", this.getClass().getName());
        status.put("timestamp", To.IsoDate(now));
        status.put("servers", servers);
        status.put("clientId", clientId);
        status.put("connection", properties.getProperty("bootstrap.servers"));
        status.put("topic", topic);
        status.put("writeStats", writeStats.status());
        status.put("writeLastRecord", writeLastRecord);
        if (To.Int(args.get("kafkaoutputtopicinfo"), 0) == 1) {
            KafkaTopicInfo kafkaTopicInfo = new KafkaTopicInfo(properties);
            status.put("topicInfo", kafkaTopicInfo.topicStatus(topic));
        }
        return status;
    }

    @Override
    public String toString() {
        return "kafka:" + topic;
    }

    private KafkaProducer<String, String> createProducer() throws Exception {
        if (properties == null) {
            properties = new Properties();
            properties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, servers);
        }

        if (StringUtils.isEmpty(clientId)) {
            clientId = getRandomClientId();
        }

        properties.put(ProducerConfig.CLIENT_ID_CONFIG, clientId);
        properties.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        properties.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        return new KafkaProducer<String, String>(properties);
    }

    /**
     * Returns random client Id
     * @return
     */
    private String getRandomClientId() {
        return DEFAULT_CLIENT_ID_PREFIX + Math.ceil(Math.random()*100);
    }
}
