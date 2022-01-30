HYPR RADIUS Client Test Utility

To test access, authentication, and performance on a HYPR RADIUS Server.
Uses the tinyradius RadiusClient for HYPR RADIUS Server access.

Performance/load testing uses special usernames, treated specially by the HYPR RADIUS Server
as of version 6.9.2 (July 2021), to short-circuit (obviate) calls to ControlCenter,
and instead waits (sleeps) a random number of milliseconds (specified in the username);
i.e. it just exercises the HYPR RADIUS Server itself. These special usernames are
of the form: **test.radius.**{**accept**,**reject**}**.***sleep-milliseconds***@hypr.com**

```
USAGE: java -jar hypr-radius-client.jar PING OPTIONS
OPTIONS:
  --host       host      # RADIUS server host name or IP (default: localhost)
  --port       port      # RADIUS server port (default: 1812)
  --admin-port port      # RADIUS server port (default: 9077)
  --secret     secret    # RADIUS server secret (default: hypr)
  --timeout    timeout   # RADIUS client timeout in milliseconds (default: 10000)
  --noadmin              # Do not ping the admin (i.e. e.g. 9700) port
  --silent               # No output (use shell status, i.e. $?, to test result, 0 on success)
  --verbose              # More verbose output
  --exceptions           # Output any exceptions in full
  --help                 # This message

USAGE: java -jar hypr-radius-client.jar AUTHENTICATE OPTIONS
OPTIONS:
  --host       host      # RADIUS server host name or IP (default: localhost)
  --port       port      # RADIUS server port (default: 1812)
  --secret     secret    # RADIUS server secret (default: hypr)
  --timeout    timeout   # RADIUS client timeout in milliseconds (default: 3000)
  --retry      retry     # RADIUS client retry count (default: 3)
  --username   username  # Username to authenticate
  --silent               # Less verbose output
  --verbose              # More verbose output
  --debug                # Even more verbose output
  --exceptions           # Output any exceptions in full
  --help                 # This message

USAGE: java -jar hypr-radius-client.jar LOAD OPTIONS
OPTIONS:
  --host       host      # RADIUS server host name or IP (default: localhost)
  --port       port      # RADIUS server port (default: 1812)
  --admin-port port      # RADIUS server port (default: 9077)
  --secret     secret    # RADIUS server secret (default: hypr)
  --timeout    timeout   # RADIUS client timeout in milliseconds (default: 3000)
  --retry      retry     # RADIUS client retry count (default: 3)
  --requests   nrequests # Total number of requests to make (default: 1)
  --threads    nthreads  # Total number of concurrent threads to spread requests evenly across (default: 1)
  --ramp       seconds   # Number of seconds to throttle thread startup process (default: 0)
  --duration   min..max  # Random number of milliseconds within range the request will take (default: 1..1000)
  --reject     percent   # Percent of requests to result in REJECT rather than ACCEPT (default: 0)
  --dryrun               # Only output what would be done (does nothing)
  --noadmin              # Do not ping the admin (i.e. e.g. 9700) port (only to get/display thread pool count)
  --silent               # Less verbose output
  --verbose              # More verbose output
  --debug                # Even more verbose output
  --exceptions           # Output any exceptions in full
  --help                 # This message
```


