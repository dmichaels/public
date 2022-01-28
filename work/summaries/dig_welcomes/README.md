**Discussion (Show & Tell) Topic**: <ins>Cartera/Rakuten Realtime Welcome Emails</ins> - _IN PROGRESS_

---

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

---

**Some More Background**

* Cartera/Rakuten provides loyalty/rewards affiliate programs (frontend/backend services) for clients to incentivize customer shopping.
  - E.g. Provide ability for United Airlines (the client) customers (the member) to signup/login (with frequent flyer number)
    and shop through the United Airline portal, and to earn frequent flyer miles for each purchase.
  - Member gets reward (miles, or points, dollars) / Client (e.g. United) gets a cut. / Cartera gets a cut. / Win-win-win.
  - Track transactions, get back from transaction aggregators (e.g. Pepperjam, LinkShare, Performics),
    processe, send _Accrual File_ to client (validate transactions), get confirmation from client, process response/data, distribute funds/data, et cetera.
  - Member and other data propogated/aggregated to/in _Data Warehouse_, downstream, and sent (SFTP) to Salesforce where marketing emails are sent/managed.
  - Emails include _Service_ emails like (transaction) _Confirmation_ emails, email for _Promotions_, and, _Welcome_ emails.
  - Tend to move data around in bulk (only) on a <ins>daily</ins> basis via database (MySQL) replication (MySQL/bash scripts).
  - So for something like Welcome emails, getting sent out as much as 48 hours after the member actually joined.
* I worked largely with this Data Warehouse system.
* Data Warehouse uniquely responsible for assigning globally unique Salesforce member ID (_Subscriber Key_),
  the algorithm for which (for historical reasons) is arcane and non-trivial; this dependency a complicating factor.

---

**Solution**

- The POC (proof-of-concept) project, to demonstrate how a more realtime, less daily-replication driven,
  system could improve the user (member) experience, was to employ Kafka as a realtime data pipline.

- Kafka is a realtime, high-throughput (append-only, non-destrutive) streaming data bus (queue, pipeline).
  - Data (_messages_, _events_) can be written (streamed) to a Kafka named _topic_ (and are timestamped), by any process (producer).
  - Other processes (consumers) may immediately read/stream (asynchronously) the messages from the Kafka topic queue.
  - Unlike (the default/typical behavior for, say) RabbitMQ, messages are "durable" and stay on the Kafka queue,
    so multiple independent processes can read/stream from the queue, and starting from any point in time (e.g. for replay purposes,
    for testing, and re-processing in the event of problems).
  - Scalable (linearly) because can add more consumer processes to handle higher volumes of data.
    - Doing so requires consumers to be part of same _consumer group_ whereby incoming messages on the queue
      will be distributed evenly to each consumer in the group.

**Solution Components**

- <ins>New Member Listener</ins>
  - Kafka/MySQL plugin to monitor our member table in MySQL for new (member) records.
  - Monitors our (MySQL) database for new members and places them on a Kafka queue.
    - The raw new member records are placed, as JSON blob/message on the Kafka _new_member_ topic queue.
    - > **_Full disclosure:_** We actually had problems with the Kafka/MySQL plugin at the time (probably fixed now),
      and as a workaround, we had to implement this as a polling process which polled/queried the MySQL member table,
      periodically (every 20 seconds, say), and wrote the member data to the Kafka _new_member_ topic queue.

- <ins>New Member Augmenter</ins>
  - Picks up raw new member data from Kafka, augments with unique Salesforce ID, and placed them on another Kafka queue.
  - Monitors our (MySQL) database for new members and places them on a Kafka queue
  - Member data sent to Salesforce need a unique ID ("Subscriber Key") which *only* the Data Warehouse
    knows how to construct. Due to historical (hysterical) reasons, the generation of this ID is non-trivial,
    and replicating the logic of generating this ID was ill-advised.
  - So this service reads (member records) from the Kafka _new_member_ topic queue (independent from the New Member Listener process).
  - Calls an API in the Data Warehouse we set up to generate the required ID (Subscriber Key),
    from the (per-organization) member ID (from the member table).
  - Augments the member data (read from Kafka) with retrieved Subscriber Key for the member.
  - Then writes the augmented member record onto another _new_member_augmented_ topic (for another, any other, process to pick up).

- <ins>New Member Emailer</ins>
  - Picks up augmented new member data from Kafka and sends to Salesforce via API.
  - And we actually wrote (yet) another Kafka messages to (yet) another Kafka topic queue indicating the record had been sent to Salesforce
    and then (yet) another consumer process would pickup (read) from that topic queue and write to a _new_members_sent_to_salesforce_ table;
    but this was really a bit overkill (a bit overengineered).

**Results**
- It worked/works (still in production).
- Beginning to end took about 3 months.
- Long time due to new technology (Kafka) usage.
  - Core development only couple/few weeks.
  - Decided to use hosted Kafka provider, for scalability, maintainence, monitoring, et cetera.
    - So just working with them and this was  
  - Pretty heavy company process for new apps.
    - E.g. Getting logging right, setting up for Splunk, monitoring, reporting, et cetera.
  - Other new changes incorporated, e.g. getting configuration from AWS Parameter Store.

**Role**
- Worked closely with team lead (architect) in defining solution.
- Implemented most of code (Java, Spring Boot, Kafka, MySQL), with some help from one or two other team members.

**Diagrams**

![dig_welcomes diagram](https://github.com/dmichaels/public/blob/master/work/summaries/dig_welcomes/dig_welcomes.png | width=400x500)

<img src="https://github.com/dmichaels/public/blob/master/work/summaries/dig_welcomes/dig_welcomes.png " width="400" height="500" />
