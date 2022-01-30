**Boston College Librarian Interview Discussion (Show & Tell) Topic**: <br />
_Cartera/Rakuten Realtime Welcome Emails_ (circa 2018)

---

**Basic Problem**
* Web site where users can signup, becoming _members_, and can subsequently login to use site/services.
* Send out _Welcome_ emails to new members on first signup.
* Currently (for historical technical reasons) Welcome emails not sent until 24-48 hours after-the-fact.
  - Bad user experience, lowers confidence in and lessens engagement with the service.

**Basic Goal**
* Send Welcome emails (nearly) immediately after signup.
  - To engender member confidence in the service, engage with them immediately, encouraging service usage, et cetera.  

**Basic Solution**

* Delayed Welcome emails due to (mere) daily data (MySQL) replication model of overall data management system.
* Move to more realtime event-driven data streaming architecture (Kafka) to propogate/process new member data as it arrives.

---

**Some More Background** (_skip if short on time_)

* Cartera/Rakuten provides loyalty/rewards affiliate programs (frontend/backend services) for clients to incentivize customer shopping.
  - E.g. Provide ability for United Airlines (the client) customers (the member) to signup/login (with frequent flyer number)
    and shop through the United Airline portal, and to earn frequent flyer miles for each purchase.
  - Member gets reward (miles, or points, dollars) / Client (e.g. United) gets a cut. / Cartera gets a cut. / Win-win-win.
  - Track transactions, get back from transaction aggregators (e.g. Pepperjam, LinkShare, Performics),
    processed, send _Accrual File_ to client (validate transactions), get confirmation from client, process response, fraud detection, distribute funds/data, et cetera.
  - Member and other data propogated/aggregated to/in _Data Warehouse_, downstream, and sent (SFTP) to Salesforce where marketing emails are sent/managed.
  - Emails include _Service_ emails like (transaction) _Confirmation_ emails, email for _Promotions_, and, _Welcome_ emails.
  - Tend to move data around in bulk (only) on a <ins>daily</ins> basis via database (MySQL) replication (MySQL/bash scripts).
  - So for something like Welcome emails, getting sent out as much as 48 hours after the member actually joined.
* Data Warehouse uniquely responsible for assigning globally unique Salesforce member ID (_Subscriber Key_),
  the algorithm for which (for historical reasons) is arcane and non-trivial; this is a data-flow bottleneck dependency is a complicating factor.
* BTW this Data Warehouse was not really a "data warehouse" in any real rigourous technical sense, just a repository and aggregation system (MySQL, and Groovy, and bash scripts).

---

**Solution**

- The POC (proof-of-concept) project, to demonstrate how a more realtime, less daily-replication driven,
  system could improve the user (member) experience, was to employ Kafka as a realtime data pipeline.

- Kafka is a realtime, high-throughput (append-only, non-destructive) streaming data bus (queue, pipeline).
  - Data (_messages_, _events_) can be written (streamed) to a Kafka named _topic_ (and are timestamped), by any process (producer).
  - Other processes (consumers) may immediately read/stream (asynchronously) the messages from the Kafka topic queue.
  - Unlike (the default/typical behavior for, say) RabbitMQ, messages are durable, stay on the Kafka queue,
    so multiple independent processes can read/stream from the queue, and starting from any point in time (e.g. for replay purposes,
    for testing, and re-processing in the event of problems).
  - Scalable (linearly) because can add more consumer processes to handle higher volumes of data.
    - Doing so requires consumers to be part of same _consumer group_ whereby incoming messages on the queue
      will be distributed evenly to each consumer in the group.

- Implemented as Spring Boot Java app, using the Java/Kafka SDK.

- App component breakdown below.
  - Each component is independent; they run concurrently.
  - Deployed as single app, but written in such a way as to be easily changed to deploy as multiple independent apps.
    - Doing so at outset seemed overkill, and would introduce more moving (deployment) parts.
  - Each component had HTTP status port to return basic health info, some stats, et cetera, for troublshooting/monitoring.

**Solution Components**

- <ins>New Member Listener</ins>
  - Kafka/MySQL plugin to monitor our member table in MySQL for new (member) records.
  - Monitors our (MySQL) database for new members and places them on a Kafka queue.
    - The raw new member records are placed, as JSON blobs/messages, on the Kafka _new_member_ topic queue.
    - > **_Full disclosure:_** We actually had problems with the Kafka/MySQL plugin at the time (probably fixed now),
      and as a workaround, we had to implement this as a brute polling process which polled/queried the MySQL member table,
      periodically (every 20 seconds, say), and wrote the member data to the Kafka _new_member_ topic queue.

- <ins>New Member Augmenter</ins>
  - Picks up raw new member data from the Kafka _new_member_ topic queue as they come in.
  - Augments member data with a _Subscriber Key_ - a globally unique Salesforce member ID.
    - Any member data sent to Salesforce needs a unique ID (_Subscriber Key_) which <ins>*only*</ins> the Data Warehouse
      knows how to construct. Due to historical (hysterical) reasons, the generation of this ID is non-trivial,
      and replicating the logic of generating this ID was ill-advised.
    - Calls a newly written API (simple Java) in the Data Warehouse we set up to generate the required Subscriber Key,
      from the given (per-organization) member ID in the data.
  - Writes augmented member data to another Kafka topic queue (_new_member_augmented_), for another process to pick up (below).

- <ins>New Member Emailer</ins>
  - Picks up augmented new member data from the Kafka _new_member_augmented_ topic queue.
  - Sends to Salesforce via a Salesforce API.

- <ins><a href="https://github.com/dmichaels/public/blob/master/work/etc/bc_librarian_interview/project/diagram.png">Diagram Here</a></ins>

---

**Results**
- It worked/works (still in production).
- Beginning to end took about three months.
  - Long time due to new technology (Kafka) usage, et.al.
  - Core development only a couple/few weeks.
  - Had to use hosted/managed Kafka provider for scalability, maintenance, monitoring, et cetera.
    - First Apache Kafka then switched to Eventador/Cloudera.
    - So just working with them and this was a lot of back/forth, troublshooting, et cetera.
  - Pretty heavy company process for new apps, and other new configuration/deployment features incorporated.
    - Storing/getting configuration in/from AWS Parameter Store (integrated with Java Spring Boot).
    - New deployment model using Chef.
    - New CI/CD (continuous integration/deployment) pipeline (GitHub, Jenkins) and release process.
    - Getting logging right, setting up for Splunk, monitoring, reporting, et cetera.
- IMO this may have been over-engineered a bit.
  - Would have been simpler and just fine to collapse the Member Augmenter and member Emails into one component.
  - But imagined possible other uses for the Kafka (_new_member_augmented_) stream of member data (never happened).
  - And we were a little eager to try out Kafka to a little greater extent.
- After this started using Kafka in a couple other projects, e.g. realtime order placed notification emails.

**My Role**
- I worked closely with team lead (architect) in defining solution.
- I implemented most of code (Java, Spring Boot, Kafka, MySQL), with some help from one or two other team members.
- I implemented the Data Warehouse Subscriber Key API (for the Member Augmenter component).
- I worked with Eventador helping with configuration, access, troubleshooting the hosted Kafka system (via Slack, email).
- I worked closely with the email marketing team (which manages Salesforce), defining member data we send to them for emailing. 
  - Helped them figure out, setup, configure Salesforce event-driven email triggering mechanism.
- I worked closely QA on testing requirements/scenarios, et cetera.
- Worked within basic Agile/Scrum process (three week sprints)
- All code management via GitHub/GitLab, branching, pull/merge requests, code reviews, et cetera.
- Pre-COVID so most collaboration in-office/in-person, and Slack, email.

---

**Some Code of Mine**
- Can't really show code for above project as no longer with this (or any) company.
- But got permission and access to a few isolated modules:
  - <a href="https://github.com/dmichaels/public/tree/master/work/etc/bc_librarian_interview/project/code_sample">Sample modules</a> (Java)
- Recent Linux PAM work (for HYPR) in C (_to be open sourced_):
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/hypr/hypr-pam">hypr-pam</a> (C)
- Recent HYPR RADIUS Server (load) testing command-line utility (_not copyrighted_):
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/hypr/hypr-radius-client">hypr-radius-client</a> (Kotlin)
- Recent-ish bash scripts to dump/load MySQL/Vertica table handling various oddities (_with permission_):
  - <a href="https://github.com/dmichaels/public/blob/master/dev/bash/tabledump.sh">tabledump.sh</a>, <a href="https://github.com/dmichaels/public/blob/master/dev/bash/tableload.sh">tableload.sh</a> (bash, C)
- Fairly old stuff here (_no longer copyrighted_):
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/liant/preprocessor">ANSI-C Preprocessor</a> (PL/I, C)
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/liant/disassembler">Motorala 680x0 Disassembler</a> (PL/I, C)
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/liant/views/src">C++/Views UI Library</a> (C++ | _much of but not nearly all mine_)
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/lincoln">MIT Lincon Laboratory Apps</a> (C)
- Early Java app (_personal project_)
  - <a href="https://github.com/dmichaels/public/tree/master/work/code/liant/views/apps/tetris">Tetris</a> (early Java)
- Recent iOS app (_personal project_):
  - <a href="https://github.com/dmichaels/public/tree/master/code/xcode/SetGame">SET Game</a> (Swift)

---

**Links**

- <a href="https://github.com/dmichaels/public/blob/master/work/resume.md">Resume</a>
- <a href="https://docs.google.com/document/d/1IfFhVwaQIJW3q-JkraAKyGyUCCi5gGflugGdMJB1TVQ/edit#heading=h.2qpcfapfn87t">Boston College Systems Librarian Info</a>
