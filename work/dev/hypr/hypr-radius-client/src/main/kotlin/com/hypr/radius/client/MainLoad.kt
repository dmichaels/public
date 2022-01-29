package com.hypr.radius.client

import com.hypr.util.durationFormat
import com.hypr.util.formatMinimalIntLength
import com.hypr.util.minMaxIntRange
import com.hypr.util.nonNegativeInt
import com.hypr.util.positiveInt
import com.hypr.util.report
import org.tinyradius.util.RadiusClient
import java.util.concurrent.atomic.AtomicLong
import kotlin.system.exitProcess

fun mainLoad(vararg args: String) : Int {

    val mainArgs = object {
        var ping                 = false
        var silent               = false
        var verbose              = false
        var debug                = false
        var doNotAccessAdminPort = false
    }

    val radius = Radius()

    val loadArgs = object {
        var threadCount   = 1
        var requestCount  = 1
        var rampUpSeconds = 0
        var dryRun        = false
    }

    val loadFunctionArgs = object {
        var randomDurationMinMilliseconds = 1
        var randomDurationMaxMilliseconds = 1000
        var percentRejectVsAccept         = 0
    }

    fun mustBeNonNegativeInt(value: String?): Int {
        val result : Int? = nonNegativeInt(value)
        return if (result != null) result else { usage(); return 1 }
    }

    fun mustBePositiveInt(value: String?): Int {
        val result : Int? = positiveInt(value)
        return if (result != null) result else { usage(); return 0 }
    }

    // Parse command-line arguments.
    //
    var skip = 0
    for ((index, arg) in args.withIndex()) {
        if (skip > 0) {
            skip-- ; continue
        } else if (arg.lowercase() in listOf("-h", "-host", "--host")) {
            if (index + 1 == args.size) usage() ; skip = 1
            radius.host = args[index + 1].trim() ?: ""
            if (radius.host.isBlank()) usage()
        } else if (arg.lowercase() in listOf("-port", "--port", "-udp-port", "--udp-port", "-udpport", "--udpport")) {
            if (index + 1 == args.size) usage() ; skip = 1
            radius.port = mustBePositiveInt(args[index + 1])
        } else if (arg.lowercase() in listOf("-admin-port", "--admin-port", "-adminport", "--adminport")) {
            if (index + 1 == args.size) usage() ; skip = 1
            radius.adminPort = mustBePositiveInt(args[index + 1])
        } else if (arg.lowercase() in listOf("-noadmin", "--noadmin", "-no-admin", "--no-admin")) {
            mainArgs.doNotAccessAdminPort = true
        } else if  (arg.lowercase() in listOf("-secret", "--secret")) {
            if (index + 1 == args.size) usage() ; skip = 1
            radius.secret = args[index + 1] ?: ""
        } else if (arg.lowercase() in listOf("-timeout", "--timeout")) {
            if (index + 1 == args.size) usage() ; skip = 1
            val value = args[index + 1]
            radius.timeout = if (value.lowercase() == "default") Radius.defaultTimeout else mustBeNonNegativeInt(value)
        } else if (arg.lowercase() in listOf("-retry", "--retry", "-retries", "--retries")) {
            if (index + 1 == args.size) usage(); skip = 1
            val value = args[index + 1]
            radius.retry = if (value.lowercase() == "default") Radius.defaultRetry else mustBeNonNegativeInt(value)
        } else if (arg.lowercase() in listOf("dryrun", "-dryrun", "--dryrun")) {
            loadArgs.dryRun = true
            mainArgs.verbose = true
            radius.verbose = true
        } else if (arg.lowercase() in listOf("-n", "-requests", "--requests", "-nrequests", "--nrequests")) {
            if (index + 1 == args.size) usage() ; skip = 1
            loadArgs.requestCount = mustBePositiveInt(args[index + 1]) ; skip = 1
        } else if (arg.lowercase() in listOf("-t", "-threads", "--threads", "-nthreads", "--nthreads")) {
            if (index + 1 == args.size) usage() ; skip = 1
            loadArgs.threadCount = mustBePositiveInt(args[index + 1])
        } else if (arg.lowercase() in listOf("-ramp", "--ramp", "-rampup", "--rampup", "-ramp-up", "--ramp-up")) {
            if (index + 1 == args.size) usage() ; skip = 1
            loadArgs.rampUpSeconds = mustBeNonNegativeInt(args[index + 1])
        } else if (arg.lowercase() in listOf("-duration", "--duration", "-request-duration", "--request-duration")) {
            if (index + 1 == args.size) usage() ; skip = 1
            val minmax = minMaxIntRange(args[index + 1])
            if (minmax == null) usage()
            loadFunctionArgs.randomDurationMinMilliseconds = minmax!!.first
            loadFunctionArgs.randomDurationMaxMilliseconds = minmax!!.second
            if (loadFunctionArgs.randomDurationMinMilliseconds == 0)
                loadFunctionArgs.randomDurationMinMilliseconds = 1
            if (loadFunctionArgs.randomDurationMaxMilliseconds == 0)
                loadFunctionArgs.randomDurationMaxMilliseconds = 1
        } else if (arg.lowercase() in listOf("-reject", "--reject", "-rejects", "--rejects")) {
            if (index + 1 == args.size) usage() ; skip = 1
            loadFunctionArgs.percentRejectVsAccept = mustBeNonNegativeInt(args[index + 1])
        } else if (arg.lowercase() in listOf("-ping", "--ping")) {
            mainArgs.ping = true
        } else if (arg.lowercase() in listOf("-verbose", "--verbose")) {
            mainArgs.verbose = true
            radius.verbose = true
        } else if (arg.lowercase() in listOf("-silent", "--silent")) {
            if (!loadArgs.dryRun) {
                mainArgs.silent = true
            }
        } else if (arg.lowercase() in listOf("-debug", "--debug")) {
            mainArgs.debug = true
            radius.debug = true
        } else if (arg.lowercase() in listOf("-exceptions", "--exceptions")) {
            radius.exceptions = true
        } else if (arg.lowercase() in listOf("help", "-help", "--help")) {
            usage()
        } else if (arg.startsWith("-", ignoreCase = true)) {
            usage("Unknown option: ${arg}")
        } else {
            usage("Unknown argument: ${arg}")
        }
    }

    if (loadArgs.threadCount > loadArgs.requestCount) {
        loadArgs.threadCount = loadArgs.requestCount
    }

    if (!mainArgs.silent) {
        println("HYPR RADIUS Client Load Test Utility")
        println("RADIUS HOST: ${radius.host}")
        if (!mainArgs.debug) {
            println("RADIUS SECRET: ${radius.secret}")
        }
        println("RADIUS UDP PORT: ${radius.port}")
        println("RADIUS ADMIN PORT: ${radius.adminPort}${if (mainArgs.doNotAccessAdminPort) " (NO ACCESS)" else ""}")
    }

    val totalFailedRequests = AtomicLong(0)

    // The actual individual load test request function.
    // Creates a special username like one of these:
    //
    //   test.radius.accept.1234@hypr.com
    //   test.radius.reject.5678@hypr.com
    //
    // which will be treated specially by the HYPR RADIUS Server (version 6.9.2 and above),
    // to not call HYPR ControlCenter for authentication, but rather to just sleep for the
    // specified number of milliseconds (i.e. 1234 and 5678 in the above example), and then
    // to return either an ACCEPT or REJECT status (as shown in the special username, above).
    //
    val formatDuration = "%0${formatMinimalIntLength(loadFunctionArgs.randomDurationMaxMilliseconds)}d"
    fun requestFunction(requestId : String) : Boolean {
        val durationMs : Int = (loadFunctionArgs.randomDurationMinMilliseconds..loadFunctionArgs.randomDurationMaxMilliseconds).random()
        val randomPercent : Int = (1..100).random()
        val acceptOrReject = if (randomPercent <= loadFunctionArgs.percentRejectVsAccept) "reject" else "accept"
        val ms = String.format(formatDuration, if (durationMs <= 0) 1 else durationMs)
        val username = "test.radius.${acceptOrReject}.${ms}@hypr.com"
        val status = radius.authenticate(username, requestId)
        if (status == Radius.Authentication.FAILED) {
            totalFailedRequests.incrementAndGet()
        }
        //
        // Always return true; false would terminate the load testing loop for this thread.
        //
        return true
    }

    if (!mainArgs.silent) {
        if (mainArgs.doNotAccessAdminPort) {
            println("RADIUS STATUS: NO ADMIN PORT ACCESS")
            println("RADIUS THREAD POOL SIZE: NO ADMIN PORT ACCESS")
        } else {
            val radiusThreadPoolSize = radius.threadPoolSize()
            println("RADIUS STATUS: ${if (radiusThreadPoolSize != null) "UP" else "DOWN"}")
            println("RADIUS THREAD POOL SIZE: ${radiusThreadPoolSize}")
        }
        println("RADIUS CLIENT TIMEOUT: ${radius.timeout}")
        println("RADIUS CLIENT RETRY COUNT: ${radius.retry}")
        var randomDurationAverageMilliseconds : Float
        if (loadFunctionArgs.randomDurationMinMilliseconds == loadFunctionArgs.randomDurationMaxMilliseconds) {
            randomDurationAverageMilliseconds = loadFunctionArgs.randomDurationMinMilliseconds.toFloat()
            println("REQUEST DURATION: ${loadFunctionArgs.randomDurationMinMilliseconds}ms")
        } else {
            randomDurationAverageMilliseconds = (loadFunctionArgs.randomDurationMinMilliseconds +
                                                 loadFunctionArgs.randomDurationMaxMilliseconds) / 2F
            println("REQUEST DURATION: ${loadFunctionArgs.randomDurationMinMilliseconds}ms..${loadFunctionArgs.randomDurationMaxMilliseconds}ms (AVERAGE: ${randomDurationAverageMilliseconds.toLong()}ms)")
        }
        if (loadFunctionArgs.percentRejectVsAccept > 0) {
            println("REQUEST REJECTS: ${loadFunctionArgs.percentRejectVsAccept}%")
        }
        println("REQUEST COUNT: ${loadArgs.requestCount}")
        println("THREAD COUNT: ${loadArgs.threadCount}")
        if (loadArgs.rampUpSeconds > 0) {
            println("THREAD RAMP UP: ${loadArgs.rampUpSeconds} seconds")
        }
        val perThreadRequestsPerSecond : Float = 1000F / randomDurationAverageMilliseconds
        val averageRequestsPerSecond = perThreadRequestsPerSecond * loadArgs.threadCount
        println("REQUESTS/SECOND: ${"%.2f".format(averageRequestsPerSecond)} (AVERAGE)")
    }

    if (loadArgs.dryRun) {
        println("DRY RUN. NOT DOING ANYTHING. EXIT.")
        exitProcess(0)
    }

    // Start the load test.

    if (!mainArgs.silent) {
        report("LOAD TEST START")
    }

    val startTimestamp = System.currentTimeMillis()

    val totalRequests = com.hypr.util.loadTest(
        { requestId -> requestFunction(requestId) },
        loadArgs.requestCount,
        loadArgs.threadCount,
        loadArgs.rampUpSeconds
    ) { requestsRunning ->
        if (!mainArgs.silent) {
            report("AWAIT THREAD COMPLETION: ${requestsRunning} RUNNING")
            System.out.flush()
        }
    }

    val duration = System.currentTimeMillis() - startTimestamp

    if (!mainArgs.silent) {
        val rate = totalRequests / (duration / 1000F)
        val failed = if (totalFailedRequests.get() > 0) " | FAILED: ${"%.2f".format(totalFailedRequests.get().toFloat() / totalRequests.toFloat() * 100F)}% (${totalFailedRequests.get()})" else ""
        report("LOAD TEST DONE: ${durationFormat(duration)} | REQUESTS: ${totalRequests} | THREADS: ${loadArgs.threadCount} | REQUESTS/SECOND: ${"%.2f".format(rate)}${failed}")
    }

    return 0
}

private fun usage(message : String = "") {
    if (message.length > 0) {
        System.err.println(message)
    }
    System.err.println("USAGE: java -jar hypr-radius-client.jar LOAD OPTIONS")
    System.err.println("OPTIONS:")
    System.err.println("  --host       host      # RADIUS server host name or IP (default: ${Radius.defaultHost})")
    System.err.println("  --port       port      # RADIUS server port (default: ${Radius.defaultPort})")
    System.err.println("  --admin-port port      # RADIUS server port (default: ${Radius.defaultAdminPort})")
    System.err.println("  --secret     secret    # RADIUS server secret (default: ${Radius.defaultSecret})")
    System.err.println("  --timeout    timeout   # RADIUS client timeout in milliseconds (default: ${Radius.defaultTimeout})")
    System.err.println("  --retry      retry     # RADIUS client retry count (default: ${Radius.defaultRetry})")
    System.err.println("  --requests   nrequests # Total number of requests to make (default: 1)")
    System.err.println("  --threads    nthreads  # Total number of concurrent threads to spread requests evenly across (default: 1)")
    System.err.println("  --ramp       seconds   # Number of seconds to throttle thread startup process (default: 0)")
    System.err.println("  --duration   min..max  # Random number of milliseconds within range the request will take (default: 1..1000)")
    System.err.println("  --reject     percent   # Percent of requests to result in REJECT rather than ACCEPT (default: 0)")
    System.err.println("  --dryrun               # Only output what would be done (does nothing)")
    System.err.println("  --noadmin              # Do not ping the admin (i.e. e.g. 9700) port (only to get/display thread pool count)")
    System.err.println("  --silent               # Less verbose output")
    System.err.println("  --verbose              # More verbose output")
    System.err.println("  --debug                # Even more verbose output")
    System.err.println("  --exceptions           # Output any exceptions in full")
    System.err.println("  --help                 # This message")
    exitProcess(-1)
}
