package com.hypr.util

import com.fasterxml.jackson.databind.JsonNode
import com.fasterxml.jackson.module.kotlin.jacksonObjectMapper
import com.fasterxml.jackson.module.kotlin.readValue
import java.net.URI
import java.net.http.HttpClient
import java.net.http.HttpRequest
import java.net.http.HttpResponse
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

/**
 * Returns a duration display string for the given milliseconds, like: HH:MM:SS.mmm.
 * Where HH is hours, MM is minutes, SS is seconds and mmm is milliseconds.
 * The hours part (HH:) is left off if zero.
 * @param milliseconds duration in milliseconds.
 * @return Formatted duration string suitable for display.
 */
fun durationFormat(milliseconds: Long): String? {
    val hours = milliseconds / 1000 / 3600
    val minutes = milliseconds / 1000 / 60 % 60
    val seconds = milliseconds / 1000 % 60
    val ms = milliseconds % 1000
    return String.format("%s%02d:%02d.%03d", if (hours > 0) "$hours:" else "", minutes, seconds, ms)
}

/**
 * Returns the integer value of the given string iff it is non-negative, otherwise null.
 * @param value Value to convert to a non-negative integer.
 * @return Non-negative value of the given string or null if malformed or negative.
 */
fun nonNegativeInt(value: String?): Int? {
    try {
        val result : Int? = value?.trim()?.toInt()
        return if ((result == null) || (result < 0)) null else result
    } catch (e : Exception) {
        return null
    }
}

/**
 * Returns the integer value of the given string iff it is positive, otherwise null.
 * @param value Value to convert to a positive integer.
 * @return Positive value of the given string or null if malformed or less-than-one.
 */
fun positiveInt(value: String?): Int? {
    try {
        val result : Int? = value?.trim()?.toInt()
        return if ((result == null) || (result <= 0)) null else result
    } catch (e : Exception) {
        return null
    }
}

/**
 * Parses the given string value of one of the following forms (for example),
 * representing a non-negative range of integer values:
 *
 *    123..456
 *    123...456
 *    123-456
 *
 * Returns the pair of values for the range it represents; or null if malformed or negative.
 *
 * @param value Value to convert to a pair/range of integers.
 * @return Pair/range of integers from the given string value or null if malformed.
 */
fun minMaxIntRange(value : String?) : Pair<Int,Int>? {
    var min : Int? ; var max : Int?
    val minmax = value?.trim()?.split("..", "...", "-")
    if (minmax?.size == 2) {
        min = nonNegativeInt(minmax.get(0).trim())
        max = nonNegativeInt(minmax.get(1).trim())
    } else {
        min = nonNegativeInt(value)
        max = min
    }
    if ((min == null) || (max == null)) {
        return null
    }
    if (min > max) {
        min = max.also { max = min }
    }
    return Pair(min!!, max!!)
}

/**
 * Normalizes the spaces in the given string an returns it, meaning it trims
 * leading/trailing spaces, and converts multiple consecutive spaces to a single space.
 * @param value String to normalize.
 * @return Normalized string.
 */
fun normalizeSpace(value : String?) : String =
    value?.replace("\\s+", " ") ?: ""

/**
 * Simple HTTP GET of the given URL, returning its result/body as a string,
 * or null if some error occurred.
 * @param url URL to fetch (HTTP GET).
 * @return Result (body) of the (HTTP GET) fetch as a string or null on error.
 */
fun httpGet(url: String): String? {
    try {
        val client = HttpClient.newBuilder().build()
        val request = HttpRequest.newBuilder().uri(URI.create(url)).build()
        return client.send(request, HttpResponse.BodyHandlers.ofString()).body()
    } catch (e : Exception) {
        return null
    }
    return ""
}

/**
 * Simple HTTP GET of the given URL, returning its result/body as a JSON object,
 * or null if some error occurred.
 * @param url URL to fetch (HTTP GET).
 * @return Result (body) of the (HTTP GET) fetch as a JSON object or null on error.
 */
fun httpGetJson(url: String): JsonNode? {
    try {
        val response = httpGet(url)
        if (response != null) {
            return jacksonObjectMapper().readValue<JsonNode>(response!!)
        }
    } catch (e : Exception) {}
    return null
}

/**
 * Simple HTTP GET of the given URL, returning true iff its result
 * HTTP status code is equal to the given expectedStatusCode.
 * @param url URL to fetch (HTTP GET).
 * @param expectedStatusCode Expected HTTP status code.
 * @return True iff the (HTTP GET) fetch returns the same HTTP status as given.
 */
fun httpPing(url: String, expectedStatusCode: Int): Boolean {
    try {
        val client = HttpClient.newBuilder().build()
        val request = HttpRequest.newBuilder().uri(URI.create(url)).build()
        return client.send(request, HttpResponse.BodyHandlers.ofString()).statusCode() == expectedStatusCode
    } catch (e : Exception) {}
    return false
}

/**
 * Returns the minimal number of decimal digits needed to represent the given integer.
 * @param n for which to return the minimal number of decimal digits to represent.
 * @return Minimal number of decimal digits needed to represent the given integer.
 */
fun formatMinimalIntLength(n : Int) : Int =
    when (n) {
        in 1..99 -> 2
        in 100..999 -> 3
        in 1000..9999 -> 4
        in 10000..99999 -> 5
        in 100000..999999 -> 6
        in 1000000..9999999 -> 7
        else -> 8 }

/**
 * Output the given message to the stderr, preceded by a date/time stamp in the form: YYYY-MM-DD hh:mm:ss.
 * @param Message to display.
 */
private val formatter = DateTimeFormatter.ofPattern("yyyy-dd-MM HH:mm:ss.SSS")
fun report(message : String) =
    println("${formatter.format(LocalDateTime.now())}: ${message}")