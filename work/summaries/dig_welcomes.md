**Discussion (Show & Tell) Topic**: <ins>Cartera/Rakuten Realtime Welcome Emails</ins>

**Basic Problem**
* Web site where users can signup, becoming _members_, and can subsequently login to use site/services.
* Send out _Welcome_ emails to new members on first signup.
* Currently (historical technical reasons) Welcome emails not sent until 24-48 hours after-the-fact.

**Basic Goal**
* Send Welcome emails (nearly) immediately after signup.
  To engender member confidence in the service, engage with them immediately, encouraging service usage, et cetera.  

**Basic Solution**

* Delayed Welcome emails due to (mere) daily data (MySQL) replication model of overall data management system.
* Move to more realtime event-driven data streaming architecture (Kafka) to propogate/process new member data as it arrives.

**Little More Background/Detail**

* Cartera/Rakuten provides loyalty/rewards affiliate programs (frontend/backend services) for clients to incentivize customer shopping.
  - E.g. We provide ability for United Airlines (the client) customers (the member) to signup/login (with frequent flyer number)
    and shop through the United Airline portal, and to earn frequent flyer miles for each purchase.
  - Member gets reward (miles, or points, dollars) / Client (e.g. United) gets a cut. / Cartera gets a cut. / Win-win-win.
  - We track purchases/transactions, gets them back from transaction aggregators (e.g. Pepperjam, LinkShare, Amazon, Performics),
    processes, sends _Accrual File_ to client (validate transactions), gets confirmation from client, processes response/data, distributes funds/data, et cetera.
* I worked largely with member data aggregation, downstream (in a _Data Warehouse_), getting it into our Salesforce system where marketing emails are sent/managed.
* Emails include"service" emails like (transaction) Confirmation emails, email for Promotions, and, Welcome emails.
* Cartera tends to move data around in bulk (only) on a daily basis via database replication (scripts).
* So for something like Welcome emails, which get sent when a member first signs up for the program,
  these were getting sent out as much as 48 hours after the member actually joined.

  - Our current (old) system (in our Data Warehouse), aggregated new member data (replicated from upstream) on a daily basis,
           assigned our (globally) unique (Salesforce) member ID ("Subscriber Key") and sent (via SFTP) data to Salesforce to send the emails.
         - Goal then was to make this more realtime, so members would get the Welcome email nearly immediately after signing up.
         - Solution, which was a sort of POC (proof-of-concept) project, to demonstrate how a more realtime (less daily-replication driven)
           system could improve the user (member) experience, was to employ Kafka as a realtime data pipline. Here are the components:
         - New Member Listener
           - Monitors our (MySQL) database for new members and places them on a Kafka queue.
           - Kafka/MySQL plugin to monitor our member table in MySQL for new (member) records.
             - Kafka is a realtime, high-throughput (append-only, non-destrutive) streaming data bus (queue, pipeline, whatever)
               - Data ("messages", "events") can be written (streamed) to a Kafka named "topic" (and are timestamped), by one process (producer).
               - Other processes (consumers) may immediately read/stream (asynchronously) the messages from the Kafka topic queue.
                 - Unlike (the default/typical behavior for) RabbitMQ, messages are "durable" and stay on the Kafka queue,
                   so multiple independent processes can read/stream from the queue, starting from any point in time.
               - Scalable (linearly) because can add more consumer processes to handle higher volumes of data.
                 - Doing so requires consumers to be part of same "consumer group" whereby incoming messages on the queue
                   will be directed evenly to each consumer in the group.
             - The raw new member records are placed, as JSON blob/message on the Kafka "new-member" topic queue.
             - Full disclosure: We actually had problems with the Kafka/MySQL plugin at the time (probably fixed now),
               and as a workaround, we had to implement this as a polling process which polled/queried the MySQL member table,
               periodically (every 20 seconds, say), and wrote the member data to the Kafka new-member topic queue.
         - New Member Augmenter
           - Picks up raw new member data from Kafka, augments with unique Salesforce ID, and placed them on another Kafka queue.
           - Monitors our (MySQL) database for new members and places them on a Kafka queue
           - Member data sent to Salesforce need a unique ID ("Subscriber Key") which *only* the Data Warehouse
             knows how to construct. Due to historical (hysterical) reasons, the generation of this ID is non-trivial,
             and replicating the logic of generating this ID was ill-advised.
           - So this service reads (member records) from the Kafka new-member topic queue (independent from the New Member Listener process).
           - Calls an API in the Data Warehouse we set up to generate the required ID (Subscriber Key),
             from the (per-organization) member ID (from the member table).
           - Augments the member data (read from Kafka) with retrieved Subscriber Key for the member.
           - Then writes the augmented member record onto another "new-member-augmented" topic (for another, any other, process to pick up).
         - New Member Emailer
           - Picks up augmented new member data from Kafka and sends to Salesforce via API.
           - And we actually wrote (yet) another Kafka messages to (yet) another Kafka topic queue indicating the record had been sent to Salesforce
             and then (yet) another consumer process would pickup (read) from that topic queue and write to a new-members-sent-to-salesforce table;
             but this was really a bit overkill (a bit overengineered).
