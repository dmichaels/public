package com.hypr.radius.client

import com.hypr.util.nonNegativeInt
import com.hypr.util.positiveInt
import kotlin.system.exitProcess

// The default Radius timeout (from org.tinyradius.util.RadiusClient) seems
// to be only 3 seconds (3000ms), a bit short, so set the default for this
// CLI utility to be 10 seconds (10000ms).
//
private val DEFAULT_RADIUS_TIMEOUT = 10000

fun mainAuthenticate(vararg args: String) : Int {

    val defaultUsername = "test.radius.accept.1@hypr.com"

    val mainArgs = object {
        var username = ""
        var silent   = false
        var verbose  = false
        var debug    = false
    }

    val radius = Radius()

    radius.timeout = DEFAULT_RADIUS_TIMEOUT

    fun mustBeNonNegativeInt(value: String?): Int {
        val result: Int? = nonNegativeInt(value)
        return if (result != null) result else { usage(); return 1 }
    }

    fun mustBePositive(value: String?): Int {
        val result: Int? = positiveInt(value)
        return if (result != null) result else { usage(); return 0 }
    }

    // Parse command-line arguments.
    //
    var skip = 0
    for ((index, arg) in args.withIndex()) {
        if (skip > 0) {
            skip-- ; continue
        } else if (arg.lowercase() in listOf("-h", "-host", "--host")) {
            if (index + 1 == args.size) usage(); skip = 1
            radius.host = args[index + 1].trim() ?: ""
            if (radius.host.isBlank()) usage()
        } else if (arg.lowercase() in listOf("-port", "--port", "-udp-port", "--udp-port", "-udpport", "--udpport")) {
            if (index + 1 == args.size) usage(); skip = 1
            radius.port = mustBePositive(args[index + 1])
        } else if (arg.lowercase() in listOf("-secret", "--secret")) {
            if (index + 1 == args.size) usage(); skip = 1
            radius.secret = args[index + 1] ?: ""
        } else if (arg.lowercase() in listOf("-conf", "--conf", "-config", "--config")) {
            //
            // TODO
            // Allow specifying a config (YAML) file like the one used by the RADIUS PAM in /etc/raddb/server,
            // and also used by the Golang based RADIUS client test utility from Ryan Rowcliffe.
            // Its contents look like this:
            //
            //     RADIUSHost: "localhost"
            //     RADIUSPort: "1812"
            //     RADIUSSecret: "hypr"
            //
            usage()
        } else if (arg.lowercase() in listOf("-u", "-user", "--user", "-username", "--username")) {
            if (index + 1 == args.size) usage(); skip = 1
            mainArgs.username = args[index + 1].trim()
        } else if (arg.lowercase() in listOf("-timeout", "--timeout")) {
            if (index + 1 == args.size) usage(); skip = 1
            val value = args[index + 1]
            radius.timeout = if (value.lowercase() == "default") Radius.defaultTimeout else mustBeNonNegativeInt(value)
        } else if (arg.lowercase() in listOf("-retry", "--retry", "-retries", "--retries")) {
            if (index + 1 == args.size) usage(); skip = 1
            val value = args[index + 1]
            radius.retry = if (value.lowercase() == "default") Radius.defaultRetry else mustBeNonNegativeInt(value)
        } else if (arg.lowercase() in listOf("-silent", "--silent")) {
            mainArgs.silent = true
        } else if (arg.lowercase() in listOf("-verbose", "--verbose")) {
            mainArgs.verbose = true
            radius.verbose = true
        } else if (arg.lowercase() in listOf("-debug", "--debug")) {
            mainArgs.debug = true
            radius.debug = true
        } else if (arg.lowercase() in listOf("help", "-help", "--help")) {
            usage()
        } else if (arg.startsWith("-", ignoreCase = true)) {
            usage("Unknown option: ${arg}")
        } else if (mainArgs.username.isBlank()) {
            mainArgs.username = arg.trim()
        }
        else {
            usage("Unknown argument: ${arg}")
        }
    }

    if (mainArgs.username.isBlank()) {
        mainArgs.username = defaultUsername
    }

    if (!mainArgs.silent) {
        println("HYPR RADIUS Client Authentication Utility")
        println("RADIUS HOST: ${radius.host}")
        if (!mainArgs.debug) {
            println("RADIUS SECRET: ${radius.secret}")
        }
        println("RADIUS UDP PORT: ${radius.port}")
        println("RADIUS CLIENT TIMEOUT: ${radius.timeout}")
        println("RADIUS CLIENT RETRY COUNT: ${radius.retry}")
        println("USERNAME: ${mainArgs.username}")
    }

    // Do the authentication.

    val status : Radius.Authentication = radius.authenticate(mainArgs.username)

    return when (status) {
        Radius.Authentication.ACCEPT  -> { if (!mainArgs.silent) println("AUTHENTICATION RESULT: ACCEPT")  ; 0 }
        Radius.Authentication.REJECT  -> { if (!mainArgs.silent) println("AUTHENTICATION RESULT: REJECT")  ; 1 }
        Radius.Authentication.TIMEOUT -> { if (!mainArgs.silent) println("AUTHENTICATION RESULT: TIMEOUT") ; 2 }
        Radius.Authentication.FAILED  -> { if (!mainArgs.silent) println("AUTHENTICATION RESULT: FAILED")  ; 3 }
    }
}

private fun usage(message : String = "") {
    if (message.length > 0) {
        System.err.println(message)
    }
    System.err.println("USAGE: java -jar hypr-radius-client.jar AUTHENTICATE OPTIONS")
    System.err.println("OPTIONS:")
    System.err.println("  --host       host      # RADIUS server host name or IP (default: ${Radius.defaultHost})")
    System.err.println("  --port       port      # RADIUS server port (default: ${Radius.defaultPort})")
    System.err.println("  --secret     secret    # RADIUS server secret (default: ${Radius.defaultSecret})")
    System.err.println("  --timeout    ms        # RADIUS client timeout in milliseconds (default: ${DEFAULT_RADIUS_TIMEOUT})")
    System.err.println("  --timeout    default   # Use default RADIUS client timeout from org.tinyradius.util.RadiusClient (${Radius.defaultTimeout})")
    System.err.println("  --retry      retry     # RADIUS client retry count (default: ${Radius.defaultRetry})")
    System.err.println("  --username   username  # Username to authenticate")
    System.err.println("  --silent               # Less verbose output")
    System.err.println("  --verbose              # More verbose output")
    System.err.println("  --debug                # Even more verbose output")
    System.err.println("  --exceptions           # Output any exceptions in full")
    System.err.println("  --help                 # This message")
    exitProcess(-1)
}
