package com.cartera.dig.service.welcomes.connector;

import com.cartera.dig.common.io.DigPipeline;
import com.cartera.dig.common.io.input.DigInput;
import com.cartera.dig.common.io.input.DigMySqlInput;
import com.cartera.dig.common.io.output.DigOutput;
import com.cartera.dig.common.io.output.DigKafkaOutput;
import com.cartera.dig.common.security.EncryptionService;
import com.cartera.dig.common.status.DigStatusService;
import com.cartera.dig.service.welcomes.util.KafkaConfigurationUtil;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

/**
 * Main configuration class for the new-member-connector phase.
 */
@Configuration
@Slf4j
public class NewMemberConnectorConfig {

    private final KafkaConfigurationUtil kafkaConfigurationUtil;

    // Input is the MySQL member table.
    // Queried repeatedly on the auto-increment column iid.

    @Value("${db.mysql.host}")
    private String mysqlHost;
    @Value("${db.mysql.port}")
    private String mysqlPort;
    @Value("${db.mysql.username}")
    private String mysqlUsername;
    @Value("${common.database.mysql.password}")
    private String mysqlPassword;

    @Value("${welcomes.mysql.table.member}")
    String memberTableName;

    @Value("${welcomes.new-member-connector.repeat-period}")
    private Integer repeatPeriod;
    @Value("${welcomes.new-member-connector.repeat-auto-increment-column}")
    private String repeatAutoIncrementColumn;
    @Value("${welcomes.new-member-connector.repeat-auto-increment-start}")
    private Long repeatAutoIncrementStart;
    @Value("${welcomes.new-member-connector.repeat-last-store}")
    private String repeatLastStore;

    // Output is the Kafka topic new_members.

    @Value("${kafka.topic.welcomes.new-member.consumer.input}")
    String kafkaOutputTopic;

    @Value("${kafka.topic.welcomes.new-member-connector.producer.client-id}")
    String kafkaOutputClientId;

    private EncryptionService kafkaEncryptionService;

    /**
     * A very useful Javadoc.
     * @param kafkaConfigurationUtil The Kafka config.
     * @throws Exception on error.
     */
    @Autowired
    public NewMemberConnectorConfig(KafkaConfigurationUtil kafkaConfigurationUtil) throws Exception {
        this.kafkaConfigurationUtil = kafkaConfigurationUtil;
        this.kafkaEncryptionService = kafkaConfigurationUtil.createEncryptionService();
    }

    /**
     * Returns the DigInput.
     * @return The DigInput.
     */
    @Bean
    @Qualifier("connector")
    public DigInput digNewMemberConnectorInput() {

        String connectionString = mysqlUsername + ":" + mysqlPassword + "@" + mysqlHost + ":" + mysqlPort;

        DigMySqlInput input = new DigMySqlInput();

        input.setConnection(connectionString);
        input.setTable(memberTableName);

        input.setRepeatPeriod(repeatPeriod != null ? repeatPeriod : 60);
        input.setRepeatAutoIncrementColumn(repeatAutoIncrementColumn);

        if (repeatAutoIncrementStart != null) {
            input.setRepeatAutoIncrementStart(repeatAutoIncrementStart);
        }
        if (repeatLastStore != null) {
            input.setRepeatLastStore(repeatLastStore);
        }

        log.info(">>>>>>>>>>>>>>>>>>>>>>>> newMemberConnector input <<<<<<<<<<<<<<<<<<<<<<<<<<<");

        return input;
    }

    /**
     * Returns the DigOutput.
     * @return The DigOutput.
     */
    @Bean
    @Qualifier("connector")
    public DigOutput digNewMemberConnectorOutput() {

        DigKafkaOutput output = new DigKafkaOutput();

        output.setTopic(kafkaOutputTopic);
        output.setClientId(kafkaOutputClientId);
        output.setProperties(kafkaConfigurationUtil.getKafkaContainerProperties());
        output.setEncryptionService(kafkaEncryptionService);

        log.info(">>>>>>>>>>>>>>>>>>>>>>>> newMemberConnector output <<<<<<<<<<<<<<<<<<<<<<<<<<<<");

        return output;
    }

    /**
     * Returns the DigPipeline.
     * @param digInput The DigInput.
     * @param digOutput The DigOutput.
     * @return The DigPipeline.
     */
    @Bean
    @Qualifier("connector")
    public DigPipeline digNewMemberConnectorPipeline(@Qualifier("connector") DigInput digInput,
            @Qualifier("connector") DigOutput digOutput) {
        log.info(">>>>>>>>>>>>>>>>>>>>>>>> creating newMemberConnector pipeline bean");
        return new DigPipeline(digInput, digOutput);
    }

    /**
     * Returns the DigStatusService.
     * @param digInput The DigInput.
     * @param digOutput The DigOutput.
     * @return The DigStatusService.
     */
    @Bean
    @Qualifier("connector")
    public DigStatusService digNewMemberConnectorStatusPort(
            @Qualifier("connector") DigInput digInput,
            @Qualifier("connector") DigOutput digOutput) {
        log.info(">>>>>>>>>>>>>>>>>>>>>>>> creating welcome newMemberConnector status port bean");
        DigStatusService digStatusService = new DigStatusService();
        digStatusService.register(digInput, digOutput);
        return digStatusService;
    }
}
