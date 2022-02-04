package com.cartera.dig.common.crypto;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

/**
 * The PGP decryption (DecryptedOutputStreamPGP) requires:
 * Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files
 * This is may not necessarily be enabled (or installed) on any given system; it
 * is on MacOS, but seems not to be on a number of Cartera Linux servers. So ...
 *
 * This class provides a bit of a HACK to enable this dynamically; this trick picked up from here:
 * https://stackoverflow.com/questions/3425766/how-would-i-use-maven-to-install-the-jce-unlimited-strength-policy-files
 *
 * In case this DOES NOT WORK or it if causes other problems, we have a backdoor to disable it,
 * via command-line option --disable-jce-setup-hack or environment variable DISABLE_JCE_SETUP_HACK.
 *
 * And the JCE Unlimited Strength Jurisdiction Policy Files can
 * alternatively be manually setup using the following notes:
 *
 *  https://www.oracle.com/java/technologies/javase-jce8-downloads.html
 *
 * Unzip the files contained in the zip file at that link, namely, these:
 *
 *  local_policy.jar
 *  US_export_policy.jar
 *
 * And (re)place them in the JRE (or JDK) lib/security directory,
 * which is usually in a directory that looks like this:
 *
 *  /usr/java/jdk1.8.0_121/jre/lib/security
 *
 * This has been tried (on MAPL-QA circa October 2020) and it worked.
 */
public class JceSetupHack {

    static {
        try {
            if (!isJceSetupHackDisabled()) {
                Field field = Class.forName("javax.crypto.JceSecurity").getDeclaredField("isRestricted");
                field.setAccessible(true);
                Field modifiersField = Field.class.getDeclaredField("modifiers");
                modifiersField.setAccessible(true);
                modifiersField.setInt(field, field.getModifiers() & ~Modifier.FINAL);
                field.set(null, false);
            }
        } catch (ClassNotFoundException | IllegalAccessException | NoSuchFieldException e) {
            System.err.println("Error dynamically setting up unlimited strength JCE.");
        } catch (Exception e) {
            System.err.println("Exception dynamically setting up unlimited strength JCE.");
        }
    }

    private static boolean isJceSetupHackDisabled() {
        String disableJceSetupHackProperty = System.getProperty("disable-jce-setup-hack");
        if (disableJceSetupHackProperty != null) {
            if (disableJceSetupHackProperty.toLowerCase().equals("1") ||
                disableJceSetupHackProperty.toLowerCase().equals("true")) return true;
        }
        String commandLine = System.getProperty("sun.java.command");
        if (commandLine != null) {
            String[] commandLineArgs = commandLine.split(" ");
            for (String arg : commandLineArgs) {
                if (arg.equals("--disable-jce-setup-hack")) return true;
            }
        }
        String disableJceSetupHackEnv = System.getenv("DISABLE_JCE_SETUP_HACK");
        if (disableJceSetupHackEnv != null) {
            if (disableJceSetupHackEnv.toLowerCase().equals("1") ||
                disableJceSetupHackEnv.toLowerCase().equals("true")) return true;
        }
        return false;
    }
}
