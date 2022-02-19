package com.cartera.dig.service.welcomes.connector;

import com.cartera.dig.common.io.DigPipeline;
import com.cartera.dig.service.welcomes.util.LogUtil;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.ApplicationArguments;
import org.springframework.boot.ApplicationRunner;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

/**
 * Main listener class for the new-member-connector phase.
 */
@Slf4j
@Service
public class NewMemberConnectorListener implements ApplicationRunner {

  private final DigPipeline digNewMemberConnectorPipeline;

  @Autowired
  private NewMemberConnectorProcessor processor;

  @Value("${welcomes.mysql.table.member}")
  private String input;

  @Value("${kafka.topic.welcomes.new-member.consumer.input}")
  private String output;

  /**
   * The constructor. Note that the DigPipeline is passed/injected in.
   * @param digNewMemberConnectorPipeline The DigPipeline.
   */
  @Autowired
  NewMemberConnectorListener(DigPipeline digNewMemberConnectorPipeline) {
    this.digNewMemberConnectorPipeline = digNewMemberConnectorPipeline;
  }

  /**
   * The main run method called auto-magically by Spring.
   * @param args The args.
   * @throws Exception on error.
   */
  @Override
  @Async
  public void run(ApplicationArguments args) throws Exception {
    String[] appArgs = args.getSourceArgs();
    if (appArgs != null) {
      for (String arg : appArgs) {
        if (arg.equals("--noconnector")) {
            return;
        }
      }
    }
    listen();
  }

  /**
   * This method uses {@link DigPipeline} to stream new member events record by record from
   * the MySQL member table and just passes through the data unchanged to the Kafka topic.
   * Note that the stream method being called is an infinite while loop, polling
   * for new member events/records continuously.
   */
  public void listen() {
    try {
      digNewMemberConnectorPipeline.stream(
        record -> {
          //
          // This (DigTransformer) lambda is called for each (DigNode) record coming
          // in from the DigInput. This can do whatever processing it needs to do with
          // the record and then returns the record it wants to send to the DigOutput.
          //
          return processor.process(record);
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
          log.error("Error processing new-member-connector record [{}]", id, onException);
          //
          // Try closing and reopening the DigInput.
          // This is to handle cases, which can happen (and had been observed happening
          // in DEV) due, for example, to intermittent MySQL connectivity issues. This
          // is a bit blunt, as this exception could be something else; should ideally
          // be checking for specific connectivity-related exception, but at the moment
          // not certain what precise exception might get thrown.
          //
          try {
              digNewMemberConnectorPipeline.getInput().close();
              Thread.sleep(500);
              digNewMemberConnectorPipeline.getInput().open();
          } catch (Exception e) {
              log.error(
                "Error while attempting new-member-connector reopen of input after exception on record [{}].",
                  onException.getRecord().toString("iid"), e);
          }
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
      log.error("Exception in DigDataPipeline while streaming new-member-connector events. Input: {}, " + "Output: {}", input, output, e);
    }
  }
}
