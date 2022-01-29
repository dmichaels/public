package com.hypr.util

import java.util.concurrent.Executors
import java.util.concurrent.Future
import java.util.concurrent.TimeUnit
import java.util.concurrent.atomic.AtomicLong
import kotlin.random.Random

/**
 * A simple load test function.
 *
 * Calls the given function the given number of times (requestCount) concurrently
 * via the given number of threads (threadCount). A ramp-up time in seconds may
 * be specified (rampUpSeconds) which will delay (i.e. sleep before) the launching
 * of each thread by that given amount divided by the thread count. And delay may
 * also be specified (i.e. a sleep) before each call to the given function by a
 * random number of milliseconds within a specified range (requestDelayMilliseconds).
 *
 * @param function Function to load test; will be called repeatedly requestCount
 * times across threadCount threads; the function should return true unless
 * it wishes to terminate the call loop within the calling thread.
 * @param requestCount Total number of calls to be made to the given function.
 * @param threadCount Number of concurrent threads to spread the calls across.
 * @param rampUpSeconds Number of seconds to throttle thread startup process. Defaults to 0.
 * @param requestDelayMilliseconds Random number of milliseconds within
 * this range to sleep before each function call. Defaults to 0.
 * @param shutdownWaitTimeSeconds Number of seconds to wait for the thread-pool
 * to terminate when the load test is complete. Defaults to 10.
 * @param onShutdown Callback to be called after all threads have been spawned.
 * @return Total number of requests (function calls) actually made.
 */
fun loadTest(function                 : (requestId : String) -> Boolean,
             requestCount             : Int,
             threadCount              : Int,
             rampUpSeconds            : Int = 0,
             requestDelayMilliseconds : IntRange? = null,
             shutdownWaitTimeSeconds  : Int = 10,
             onShutdown               : (Long) -> Unit = {}) : Long
{

    // Validate the request and thread counts; cannot be
    // less that one; cannot be more threads than requests.
    //
    val _requestCount = if (requestCount > 0) requestCount else 1
    val _threadCount  = if (threadCount > 0 && threadCount <= _requestCount) threadCount else requestCount

    // Calculate the number of requests each thread will execute.
    //
    val requestsPerThread = _requestCount / _threadCount
    val requestsPerThreadRemainder = _requestCount % _threadCount

    // To track the total number of request as the occur.
    //
    val totalRequests = AtomicLong(0)
    val requestsRunning = AtomicLong(0)

    // Construct a request ID for each request (call to the given function), which consists
    // of the thread index and the request number within the thread, one-indexed/zero-padded,
    // and separated by a dash, and prefixed with the total current request count followed
    // by a colon, e.g. 02059:02-0123, indicating the 2059th request, from thread number 2,
    // and the 123rd request within that thread. This is passed as an argument to
    // the given function which can choose to display it or not as desired.
    //
    fun formatRequestId() : String {
        return String.format("%%0%dd:%%0%dd-%%0%dd",
                              formatMinimalIntLength(_requestCount),
                              formatMinimalIntLength(_threadCount),
                              formatMinimalIntLength(if (requestsPerThreadRemainder > 0) requestsPerThread + 1 else requestsPerThread))
    }

    val formatRequestId = formatRequestId()

    fun requestId(totalRequests : Long, threadIndex : Int, requestIndex : Int) =
        String.format(formatRequestId, totalRequests, threadIndex, requestIndex)

    // Determine the (random) range of milliseconds to wait/sleep on/before each request,
    // i.e. call of the given function, from the given requestDelayMilliseconds range;
    // used only if the range comprises a positive integer range.
    //
    var requestDelayMinMilliseconds : Int = 0
    var requestDelayMaxMilliseconds : Int = 0

    if (requestDelayMilliseconds != null) {
        if ((requestDelayMilliseconds.first > 0) && (requestDelayMilliseconds.last > 0)) {
            requestDelayMinMilliseconds = requestDelayMilliseconds.first
            requestDelayMaxMilliseconds = requestDelayMilliseconds.last
            //
            // Guard against the range values being reversed.
            //
            if (requestDelayMinMilliseconds > requestDelayMaxMilliseconds) {
                requestDelayMinMilliseconds =
                    requestDelayMaxMilliseconds.also {
                        requestDelayMaxMilliseconds = requestDelayMinMilliseconds }
            }
        }
    }

    // This function is executed for each thread spawned. It repeatedly calls the
    // given function the specified number of times, which is to say, the total number
    // of requests (requestCount) divided by the total number of threads (threadCount),
    // plus an extra one if there is a remainder (this logic handled by caller, below).
    // If a request delay range (requestDelayMilliseconds) is specified then a sleep is
    // done before each function call for a random length of time within the given range.
    //
    fun requestThread(nrequests : Int, threadIndex: Int) {
        repeat (nrequests) { requestIndex ->
            //
            // Insert a random wait before the function call if the
            // given waitBetweenRequestsMilliseconds was specified.
            //
            if (requestDelayMinMilliseconds > 0) {
                val delayDurationMilliseconds =
                    if (requestDelayMinMilliseconds == requestDelayMaxMilliseconds ) {
                        requestDelayMinMilliseconds
                    } else {
                        Random.nextInt(requestDelayMinMilliseconds, requestDelayMaxMilliseconds + 1)
                    }
                if (delayDurationMilliseconds == 0) {
                    Thread.yield()
                } else {
                    Thread.sleep(delayDurationMilliseconds.toLong())
                }
            }
            //
            // Call the given function.
            //
            val requestId = requestId(totalRequests.incrementAndGet(), threadIndex, requestIndex + 1)
            requestsRunning.incrementAndGet()
            val succeeded = function(requestId)
            requestsRunning.decrementAndGet()
            if (!succeeded) {
                return
            }
        }
    }

    // Computes the ramp-up milliseconds/nanoseconds, which is the amount
    // of time to wait/sleep before the (initial) spawning of each thread.
    //
    fun rampUpSleepValues(nseconds : Int, nthreads : Int) : Pair<Long,Int> {
        if (nthreads <= 1) return Pair(0, 0)
        var nanoseconds = (nseconds * 1_000_000_000) / nthreads - 1
        val milliseconds = nanoseconds / 1_000_000L
        nanoseconds = nanoseconds % 1_000_000
        return Pair(milliseconds, nanoseconds)
    }

    // Start the load test here.
    //
    val (rampUpMilliseconds, rampUpNanoseconds) = rampUpSleepValues(rampUpSeconds, _threadCount)
    val threadPool = Executors.newCachedThreadPool()
    val tasks = mutableListOf<Future<*>>()
    var nrequestsRemainder = requestsPerThreadRemainder
    repeat (_threadCount) { threadIndex ->
        if ((rampUpMilliseconds > 0) || (rampUpNanoseconds > 0)) {
            Thread.sleep(rampUpMilliseconds, rampUpNanoseconds)
        }
        val nrequests = if (nrequestsRemainder-- > 0) { requestsPerThread + 1 } else requestsPerThread
        tasks.add(threadPool.submit {
            requestThread(nrequests, threadIndex + 1)
        })
    }
    tasks.map { it.get() }
    onShutdown(requestsRunning.get())
    threadPool.shutdown()
    threadPool.awaitTermination(shutdownWaitTimeSeconds.toLong(), TimeUnit.SECONDS)
    return totalRequests.get()
}