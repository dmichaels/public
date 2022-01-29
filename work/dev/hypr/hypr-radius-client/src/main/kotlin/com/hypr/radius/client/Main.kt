package com.hypr.radius.client

import kotlin.system.exitProcess

/**
 * HYPR RADIUS Server Client Test Utility.
 * To test access, authentication, and load.
 * Uses the tinyradius RadiusClient for HYPR RADIUS Server access.
 *
 * Load testing uses special usernames to short-circuit (obviate) calls to ControlCenter
 * and waits (sleeps) a random number of milliseconds (specified in the username);
 * i.e. just exercises the HYPR RADIUS Server itself; see MainLoad.requestFunction.
 */
fun main(args: Array<String>) {

    if (args.size < 1) {
        usage()
    }

    val command = args[0].lowercase()

    if (command in listOf("load", "-load", "--load", )) {
        exitProcess(mainLoad(*args.copyOfRange(1, args.size)))
    } else if (command in listOf("auth", "-auth", "--auth", "authenticate", "-authenticate", "--authenticate")) {
        exitProcess(mainAuthenticate(*args.copyOfRange(1, args.size)))
    } else if (command in listOf("ping", "-ping", "--ping")) {
        exitProcess(mainPing(*args.copyOfRange(1, args.size)))
    } else {
         usage()
    }
}

private fun usage(message : String = "") {
    if (message.length > 0) {
        System.err.println(message)
    }
    System.err.println("USAGE: java -jar hypr-radius-client.jar COMMAND")
    System.err.println("COMMANDS: { AUTHENTICATE | LOAD | PING } [options] [--help]")
    exitProcess(1)
}
