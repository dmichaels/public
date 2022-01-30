package com.cartera.dig.service.welcomes.newmember;

import com.cartera.dig.common.datawarehouse.SubscriberKeyService;
import com.cartera.dig.common.io.DigPipeline;
import com.cartera.dig.service.welcomes.util.LogUtil;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

/**
 * This is a Spring {@link Service} class representing a kafka listener that listens for any new member events placed
 * in one of the kafka topics, adds a subscriber key for the record associated with that new member event and
 * places it on another topic. The {@link Slf4j} annotation will initialize the slf4j logger.
 */
@Slf4j
@Service
public class NewMemberListener implements ApplicationRunner {
    private final SubscriberKeyService subscriberKeyService;
    private final DigPipeline digNewMemberPipeline;

    @Autowired
    private NewMemberProcessor processor;
    @Value("${kafka.topic.welcomes.new-member.consumer.input}")
    private String kafkaNewMemberInputTopic;
    @Value("${kafka.topic.welcomes.email-generator.consumer.input}")
    private String kafkaNewMemberOutputTopic;

    /**
     * Constructor that takes in a {@link SubscriberKeyService} and a {@link DigPipeline}
     * instance (qualifier = "newMember" ) and creates a {@link NewMemberListener} instance.
     * PS : Within dig-welcomes application context, it is Spring boot that will instantiate the
     * {@link NewMemberListener} instance
     *
     * @param subscriberKeyService a {@link SubscriberKeyService} instance which will be used to generate
     *                             subscriber key for a given member.
     * @param digNewMemberPipeline a {@link DigPipeline} instance that will be used for streaming
     *                             data from source to sink (Both will be Kafka topics in new member process)
     */
    @Autowired
    NewMemberListener(SubscriberKeyService subscriberKeyService,
                      @Qualifier("newMember") DigPipeline digNewMemberPipeline) {
        this.subscriberKeyService = subscriberKeyService;
        this.digNewMemberPipeline = digNewMemberPipeline;
    }

    /**
     * This is an async method which is ran as soon as Spring Boot app starts.
     * It calls the "listenNewMemberEvents" method which listens for each new member event/record, generates a subscriber
     * key for each associated record and creates a new event for the sink kafka topic.
     *
     * @param args application arguements
     */
    @Async
    @Override
    public void run(ApplicationArguments args) {
        String[] appArgs = args.getSourceArgs();
        if (appArgs != null) {
            for (String arg : appArgs) {
                if (arg.equals("--nolistener")) {
                    return;
                }
            }
        }
        listenNewMemberEvents();
    }

    /**
     * This method uses {@link DigPipeline} to stream new member events record by record, add subscriber key for
     * each of the record and pass it onto the sink kafka topic. Please note that the stream method being called
     * is an infinite while loop, polling for new member events/records continuously. Thats why this should
     * be run in a separate thread altogether.
     */
    public void listenNewMemberEvents() {
        try {
            digNewMemberPipeline.stream(
                record -> {
                    //
                    // This (DigTransformer) lambda is called for each (DigNode) record coming
                    // in from the DigInput. This can do whatever processing it needs to do with
                    // the record and then returns the record it wants to send to the DigOutput.
                    //
                    return processor.newMemberRecordProcessor(record);
                },
                onException -> {
                    //
                    // This (onException) lambda is called when an exception occurs during
                    // the reading/processing of an input (DigNode) record coming in from
                    // the DigInput. The arg is a DigStreamingException; its getRecord method
                    // can be used to get the actual record which experienced the exception.
                    // If this is called then the next (onSuccess) lambda is not called.
                    //
                    String id = LogUtil.rummageForMemberIdentifier(onException.getRecord());
                    log.error("Error processing listener record [{}]", id, onException);
                },
                onSuccessRecord -> {
                    //
                    // This (onSuccess) lambda is called after the input (DigNode) record coming from
                    // the DigInput has been successfully read/processed from the DigInput (optionally
                    // processed/transformed using the first lambda on this stream call), and output to
                    // the DigOutput. Post-processing on this successfully processed record can be done
                    // here. If this is called then the previous (onException) lambda is/was not called.
                    //
                }
            );
        } catch (Exception e) {
            log.error(
                    "Exception in datapipeline while generating welcome_emails event. Input topic : {}, "
                            + "Output topic : {}",
                    kafkaNewMemberInputTopic, kafkaNewMemberOutputTopic, e);
        }
    }


}
