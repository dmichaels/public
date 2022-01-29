package com.hypr.radius.client

import com.fasterxml.jackson.databind.JsonNode
import com.fasterxml.jackson.module.kotlin.jacksonObjectMapper
import com.hypr.util.httpGetJson
import com.hypr.util.httpPing
import com.hypr.util.normalizeSpace
import com.hypr.util.report
import org.tinyradius.util.RadiusClient
import java.net.SocketTimeoutException
import java.util.concurrent.atomic.AtomicLong

private val authenticationsRunning = AtomicLong(0)
//
// This is just so we can get/initialize the default timeout and retry values.
//
private val dummyClient = RadiusClient("localhost", "dummy-password")

/**
 * Class through which we talk to the HYPR Radius Server, using the tinyradius
 * RadiusClient for authentication. Also provides access to the (new, as of HYPR
 * RADIUS Server 6.9.2 July 2021) /stats endpoint for basic stats on the server,
 * primarily for our purposes just the thread-pool state, and for pinging.
 */
data class Radius (
    var host       : String  = defaultHost,
    var secret     : String  = defaultSecret,
    var port       : Int     = defaultPort,
    var adminPort  : Int     = defaultAdminPort,
    var timeout    : Int     = defaultTimeout,
    var retry      : Int     = defaultRetry,
    var configKey  : String? = null,
    var verbose    : Boolean = false,
    var debug      : Boolean = false,
    var exceptions : Boolean = false
) {

    companion object defaults {
        val defaultHost      = "localhost"
        val defaultSecret    = "hypr"
        val defaultPort      = 1812
        val defaultAdminPort = 9077
        val defaultTimeout   = dummyClient.socketTimeout
        val defaultRetry     = dummyClient.retryCount
    }

    data class Stats(
        val threadsRunning: Long,
        val threadsScheduledTotal: Long,
        val threadsCompletedTotal: Long,
        val threadsQueued: Long,
        val threadPoolSize: Long,
        val threadPoolCoreSize: Long,
        val threadPoolQueueSize: Long,
        val authenticationsRunning: Long,
        val authenticationsRunningMax: Long,
        val authenticationCountTotal: Long,
        val authenticationAcceptCountTotal: Long,
        val authenticationRejectCountTotal: Long,
        val authenticationTimeAverage: Long,
        val authenticationTimeMax: Long,
        var json: JsonNode
    )

    data class Settings(
        val accessToken: String,
        val clientSecrets: Map<String,String>,
        val customAttributeName: String?,
        val customAttributeValue: String?,
        val localRadiusSecret: String,
        val proxyEnabled: Boolean,
        val proxyHost: String?,
        val proxyPassword: String?,
        val proxyPort: String?,
        val proxyUsername: String?,
        val radiusDomain: String,
        val radiusTimeout: Int,
        val rpAppId: String,
        val rpUrl: String,
        var json: JsonNode
    )

    enum class Authentication {
        ACCEPT,
        REJECT,
        FAILED,
        TIMEOUT
    }

    fun authenticate(username: String, requestId: String = "", report: (String) -> Unit = ::report): Authentication {
        authenticationsRunning.incrementAndGet()
        var status : Authentication
        val messagePrefix = if (requestId.length > 0) "[${requestId}] " else ""
        val startTimestamp = System.currentTimeMillis()
        try {
            val client = RadiusClient(this.host, this.secret)
            client.setAuthPort(this.port)
            //
            // If timeout is non-negative (may be zero) then use that,
            // otherwise (negative) use the default RadiusClient timeout.
            //
            if (this.timeout >= 0) {
                client.setSocketTimeout(this.timeout)
            } else {
                client.setSocketTimeout(dummyClient.socketTimeout)
            }
            if (this.retry >= 0) {
                client.setRetryCount(this.retry)
            }
            if (this.debug) {
                report("${messagePrefix}RADIUS authentication: ${username} -> STARTING [RUNNING: ${authenticationsRunning.get()}]")
            }
            val succeeded = client.authenticate(username, "dummy-password")
            //
            // Don't forget to close!
            //
            client.close()
            val duration = System.currentTimeMillis() - startTimestamp
            if (succeeded) {
                if (this.verbose) {
                    report("${messagePrefix}RADIUS authentication: ${username} -> OK (ACCEPT) [${duration}ms]")
                }
                status = Authentication.ACCEPT
            } else {
                if (this.verbose) {
                    report("${messagePrefix}RADIUS authentication: ${username} -> OK (REJECT) [${duration}ms]")
                }
                status = Authentication.REJECT
            }
        } catch (e: Exception) {
            //
            // TODO
            // Should we be trying to close the RadiusClient here?
            //
           val duration = System.currentTimeMillis() - startTimestamp
            if (this.verbose) {
                val isTimeout = e is SocketTimeoutException
                report("${messagePrefix}RADIUS authentication: ${username} -> FAILED (${if (isTimeout) "TIMEOUT" else e.javaClass.simpleName}) [${duration}ms]")
            }
            if (this.exceptions) {
                report("${messagePrefix}${normalizeSpace(e.toString())}: ${normalizeSpace(e.stackTraceToString())}")
            }
            status = if (e is SocketTimeoutException) Authentication.TIMEOUT else Authentication.FAILED
        }
        authenticationsRunning.decrementAndGet()
        return status
    }

    fun pingAuthenticationPort(): Authentication {
        val result = this.authenticate("test.radius.accept.1@hypr.com");
        return if (result == Authentication.ACCEPT || result == Authentication.TIMEOUT) result else Authentication.FAILED;
    }

    fun pingAdminPort() : Boolean =
        httpPing(this.pingEndpoint(), 200)

    fun stats(): Stats? {
        try {
            val responseJson = this.statsJson() ?: return null
            var response = jacksonObjectMapper().treeToValue(responseJson, Stats::class.java) ?: return null
            response.json = responseJson
            return response
        } catch (e : Exception) {
            return null
        }
    }

    fun statsJson(): JsonNode? =
        httpGetJson(statsEndpoint())

    fun threadPoolSize() : Int? =
        this.stats()?.threadPoolCoreSize?.toInt()

    fun settings(): Settings? {
        try {
            val responseJson = this.settingsJson() ?: return null
            var response = jacksonObjectMapper().treeToValue(responseJson, Settings::class.java) ?: return null
            response.json = responseJson
            return response
        } catch (e : Exception) {
            return null
        }
    }

    fun settingsJson(): JsonNode? =
        if (configKey != null) httpGetJson(settingsEndpoint(configKey!!)) else null

    fun pingEndpoint(): String =
        "http://${this.host}:${this.adminPort}/radius/login"

    fun statsEndpoint(): String =
        "http://${this.host}:${this.adminPort}/radius/stats"

    fun settingsEndpoint(configKey: String? = null): String {
        var radiusConfigKey = if (!configKey.isNullOrEmpty()) configKey else this.configKey
        if (radiusConfigKey.isNullOrEmpty()) radiusConfigKey = "<UNSPECIFIED_CONFIG_KEY>"
        return "http://${this.host}:${this.adminPort}/radius/settings/radiusconfigs?configKey=${radiusConfigKey}"
    }

    fun settingsEndpointObfuscated(): String =
        "http://${this.host}:${this.adminPort}/radius/settings/radiusconfigs?configKey=********"
}
