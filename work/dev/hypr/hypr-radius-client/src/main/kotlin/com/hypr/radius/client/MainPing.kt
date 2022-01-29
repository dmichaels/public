package com.hypr.radius.client

import com.hypr.util.nonNegativeInt
import com.hypr.util.positiveInt
import kotlin.system.exitProcess

/*
 * Pings the specified HYPR RADIUS Server.
 * Typically the authentication (UDP) port is: 1812
 * Typically the admin (HTTP) port is: 9077
 * The admin port has a /login page for local/test logins.
 * The admin port has a /radius/settings/radiusconfigs?configKey=configKey page for RADIUS settings.
 * The admin port has a /radius/stats page for RADIUS authentication/thread-pool stats/info.
 */
fun mainPing(vararg args: String) : Int {

    val mainArgs = object {
        var silent                        = false
        var verbose                       = false
        var debug                         = false
        var doNotAccessAdminPort          = false
        var doNotAccessAuthenticationPort = false
        var doNotAccessSettingsEndpoint   = false
        var doNotAccessStatsEndpoint      = false
        var showSecretInfo                = false
    }

    val radius = Radius()

    fun mustBePositiveInt(value: String?): Int {
        val result : Int? = positiveInt(value)
        return if (result != null) result else { usage(); return 1 }
    }

    fun mustBeNonNegativeInt(value: String?): Int {
        val result : Int? = nonNegativeInt(value)
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
        } else if  (arg.lowercase() in listOf("-secret", "--secret")) {
            if (index + 1 == args.size) usage() ; skip = 1
            radius.secret = args[index + 1] ?: ""
        } else if (arg.lowercase() in listOf("-timeout", "--timeout")) {
            if (index + 1 == args.size) usage() ; skip = 1
            val value = args[index + 1]
            radius.timeout = if (value.lowercase() == "default") Radius.defaultTimeout else mustBeNonNegativeInt(value)
        } else if (arg.lowercase() in listOf("-config-key", "--config-key", "-configkey",  "--configkey",
                                             "-config", "--config", "-conf-key", "--conf-key",
                                             "-confkey", "--confkey", "-conf", "--conf", "-key", "--key")) {
            if (index + 1 == args.size) usage() ; skip = 1
            radius.configKey = args[index + 1]
        } else if (arg.lowercase() in listOf("-noadmin", "--noadmin", "-no-admin", "--no-admin")) {
            mainArgs.doNotAccessAdminPort = true
        } else if (arg.lowercase() in listOf("-nostats", "--nostats", "-no-stats", "--no-stats")) {
            mainArgs.doNotAccessStatsEndpoint = true
        } else if (arg.lowercase() in listOf("-nosettings", "--nosettings", "-no-settings", "--no-settings")) {
            mainArgs.doNotAccessSettingsEndpoint = true
        } else if (arg.lowercase() in listOf("-noudp", "--noudp", "-no-udp", "--no-udp")) {
            mainArgs.doNotAccessAuthenticationPort = true
        } else if (arg.lowercase() in listOf("-silent", "--silent")) {
            mainArgs.silent = true
        } else if (arg.lowercase() in listOf("-verbose", "--verbose")) {
            mainArgs.verbose = true
        } else if (arg.lowercase() in listOf("-show-secrets", "--show-secrets", "-show-secret", "--show-secret",
                                             "-showsecrets", "--showsecrets", "-showsecret", "--showsecret", "-show", "--show")) {
            mainArgs.showSecretInfo = true
        } else if (arg.lowercase() in listOf("-debug", "--debug")) {
            mainArgs.debug = true
            mainArgs.showSecretInfo = true
            radius.verbose = true
            radius.debug = true
        } else if (arg.lowercase() in listOf("help", "-help", "--help")) {
            usage()
        } else if (arg.startsWith("-", ignoreCase = true)) {
            usage("Unknown option: ${arg}")
        } else {
            usage("Unknown argument: ${arg}")
        }
    }

    if (!mainArgs.silent) {
        println("HYPR RADIUS Client Ping Utility")
        println("RADIUS HOST: ${radius.host} (SECRET: ${if (mainArgs.showSecretInfo) radius.secret else "********"})")
        if (!mainArgs.doNotAccessAuthenticationPort) {
            println("RADIUS UDP PORT: ${radius.port}")
        }
        if (!mainArgs.doNotAccessAdminPort) {
            println("RADIUS ADMIN PORT: ${radius.adminPort}")
        }
    }

    var status = 0

    if (!mainArgs.doNotAccessAuthenticationPort) {
        val result = radius.pingAuthenticationPort();
        if (result == Radius.Authentication.ACCEPT) {
            if (!mainArgs.silent) {
                println("RADIUS PING UDP PORT: OK")
            }
        } else if (result == Radius.Authentication.TIMEOUT) {
            if (!mainArgs.silent) {
                println("RADIUS PING UDP PORT: TIMEOUT")
            }
        } else {
            if (!mainArgs.silent) {
                println("RADIUS PING UDP PORT: FAILED (CONFIRM: --secret)")
            }
            status++
        }
    }

    if (!mainArgs.doNotAccessAdminPort) {
        if (radius.pingAdminPort()) {
            if (!mainArgs.silent) {
                println("RADIUS PING ADMIN PORT: OK")
            }
        } else {
            if (!mainArgs.silent) {
                println("RADIUS PING ADMIN PORT: FAILED")
            }
            status++
        }
        if (!mainArgs.doNotAccessStatsEndpoint) {
            val stats = radius.stats()
            if (mainArgs.verbose || mainArgs.debug) {
                println("RADIUS STATS ENDPOINT: ${radius.statsEndpoint()}")
            }
            if (stats != null) {
                println("RADIUS STATS THREAD POOL: ${stats.threadPoolSize}")
                println("RADIUS STATS AUTHENTICATIONS: ${stats.authenticationCountTotal} (SUCCESS: ${stats.authenticationAcceptCountTotal} | REJECT: ${stats.authenticationRejectCountTotal})")
                println("RADIUS STATS AUTHENTICATIONS RUNNING: ${stats.authenticationsRunning}")
                println("RADIUS STATS AUTHENTICATIONS QUEUED: ${stats.threadsQueued}")
                if (mainArgs.debug) {
                    println("RADIUS STATS ENDPOINT RESPONSE:")
                    println(stats.json.toPrettyString())
                }
            } else {
                println("RADIUS STATS ENDPOINT ACCESS FAILED")
            }
        }
        if (!mainArgs.doNotAccessSettingsEndpoint) {
            val settings: Radius.Settings? = radius.settings()
            if (mainArgs.verbose || mainArgs.debug) {
                println("RADIUS SETTINGS ENDPOINT: ${if (mainArgs.showSecretInfo) radius.settingsEndpoint() else radius.settingsEndpointObfuscated()}")
            }
            if (settings != null) {
                println("RADIUS SETTINGS DOMAIN: ${settings.radiusDomain}")
                println("RADIUS SETTINGS TIMEOUT: ${settings.radiusTimeout}")
                println("RADIUS SETTINGS SECRET: ${if (mainArgs.showSecretInfo) settings.localRadiusSecret else "*******"}")
                println("RADIUS SETTINGS RP APP: ${settings.rpAppId}")
                println("RADIUS SETTINGS RP URL: ${settings.rpUrl}")
                println("RADIUS SETTINGS RP ACCESS TOKEN: ${if (mainArgs.showSecretInfo) settings.accessToken else "********"}")
                println("RADIUS SETTINGS CLIENT${if (settings.clientSecrets.size == 1) "" else "S"}: ${settings.clientSecrets.size}")
                settings.clientSecrets.forEach {
                    println("RADIUS SETTINGS CLIENT: ${it.key} (SECRET: ${if (mainArgs.showSecretInfo) it.value else "********"})")
                }
                if (settings.proxyEnabled) {
                    println("RADIUS SETTINGS PROXY: true")
                    println("RADIUS SETTINGS PROXY HOST: ${settings.proxyHost}")
                    println("RADIUS SETTINGS PROXY PORT: ${settings.proxyPort}")
                    println("RADIUS SETTINGS PROXY USERNAME: ${settings.proxyUsername}")
                }
                if (!settings.customAttributeName.isNullOrBlank()) {
                    println("RADIUS SETTINGS CUSTOM ATTRIBUTE NAME: ${settings.customAttributeName}")
                }
                if (!settings.customAttributeValue.isNullOrBlank()) {
                    println("RADIUS SETTINGS CUSTOM ATTRIBUTE VALUE: ${settings.customAttributeValue}")
                }
                if (mainArgs.debug) {
                    println("RADIUS SETTINGS ENDPOINT RESPONSE:")
                    println(settings.json.toPrettyString())
                }
            } else {
                if (!mainArgs.silent) {
                    if (radius.configKey.isNullOrEmpty()) {
                        println("RADIUS SETTINGS ENDPOINT NOT CALLED (NO: --key)")
                    } else {
                        println("RADIUS SETTINGS ENDPOINT CALL FAILED (CONFIRM: --key)")
                    }
                }
                status++
            }
        }
    }

    return status
}

private fun usage(message : String = "") {
    if (message.length > 0) {
        System.err.println(message)
    }
    System.err.println("USAGE: java -jar hypr-radius-client.jar PING OPTIONS")
    System.err.println("OPTIONS:")
    System.err.println("  --host       host      # RADIUS server host name or IP (default: ${Radius.defaultHost})")
    System.err.println("  --port       port      # RADIUS server port (default: ${Radius.defaultPort})")
    System.err.println("  --admin-port port      # RADIUS server port (default: ${Radius.defaultAdminPort})")
    System.err.println("  --secret     secret    # RADIUS server secret (default: ${Radius.defaultSecret})")
    System.err.println("  --timeout    timeout   # RADIUS client timeout in milliseconds (default: ${Radius.defaultTimeout})")
    System.err.println("  --noadmin              # Do not ping the admin (i.e. e.g. ${Radius.defaultAdminPort}) port")
    System.err.println("  --silent               # No output (use shell status, i.e. $?, to test result, 0 on success)")
    System.err.println("  --verbose              # More verbose output")
    System.err.println("  --show-secrets         # Show any secret info in plaintext")
    System.err.println("  --exceptions           # Output any exceptions in full")
    System.err.println("  --config-key key       # Config key used to hit settings endpoint to get config info")
    System.err.println("  --help                 # This message")
    exitProcess(-1)
}
