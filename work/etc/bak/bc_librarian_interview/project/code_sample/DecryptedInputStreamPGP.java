package com.cartera.dig.common.crypto;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.IOException;
import java.security.NoSuchProviderException;
import java.security.Security;
import java.util.Iterator;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openpgp.PGPEncryptedDataList;
import org.bouncycastle.openpgp.PGPCompressedData;
import org.bouncycastle.openpgp.PGPException;
import org.bouncycastle.openpgp.PGPLiteralData;
import org.bouncycastle.openpgp.PGPPrivateKey;
import org.bouncycastle.openpgp.PGPPublicKeyEncryptedData;
import org.bouncycastle.openpgp.PGPSecretKey;
import org.bouncycastle.openpgp.PGPSecretKeyRingCollection;
import org.bouncycastle.openpgp.PGPUtil;
import org.bouncycastle.openpgp.jcajce.JcaPGPObjectFactory;
import org.bouncycastle.openpgp.operator.jcajce.JcaKeyFingerprintCalculator;
import org.bouncycastle.openpgp.operator.jcajce.JcePBESecretKeyDecryptorBuilder;
import org.bouncycastle.openpgp.operator.jcajce.JcePublicKeyDataDecryptorFactoryBuilder;

/**
 * This is an InputStream implementation which decrypts its input from the
 * specified underlying InputStream using the specified PGP private-key file.
 * This decrypts in exactly the reverse way that EncryptedOutputStreamPGP encrypts.
 *
 * IMPORTANT NOTE
 * --------------
 * The PGP decryption (common/security/DecryptedOutputStreamPGP) requires:
 * Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files
 * This is may not necessarily be enabled (or installed) on any given system; it
 * is on MacOS, but seems not to be on a number of Cartera Linux servers. So ...
 *
 * There is a hack in our app code (commons/security/JceSetupHack) which enables this dynamically.
 * In case this DOES NOT WORK and/or causes other problems, we have a backdoor to disable it,
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
 * which is usually in a directory like this:
 *
 *  /usr/java/jdk1.8.0_121/jre/lib/security
 *
 * ENCRYPTION
 * ----------
 * A plaintext file encrypted using the gpg command like the below
 * can be decrypted by this class:
 *
 *  gpg -r pgp-key-id -o encrypted-file -e plaintext-file
 *
 * where the pgp-key-id is the ID in your PGP key store associated
 * with the public-key used to encrypt the file. These IDs can be seen using:
 *
 *  gpg --list-secret-keys --keyid-format short
 *
 * Which outputs something like this:
 *
 *  pub   rsa2048/78CBFD16 2020-10-23 [SC] [expires: 2022-10-23]
 *        8E65525AA703040EB0E37BD61626BF0478CBFD16
 *  uid         [ unknown] David Michaels <dmichaels@cartera.com>
 *  sub   rsa2048/9CC8AB43 2020-10-23 [E] [expires: 2022-10-23]
 *
 * So in this case the pgp-key-id is 9CC8AB43.
 *
 * KEYS
 * ----
 * For testing, the recommended way to generate suitable public and private keys using gpg is:
 *
 *  gpg --gen-key
 *  gpg -r 78CBFD16 -a --export > public-key.pgp
 *  gpg -r 78CBFD16 -a --export-secret-key > private-key.pgp
 *
 * And a given PGP public-key or private-key can be imported into your PGP key store thus:
 *
 *  gpg --import public-key.pgp
 *  gpg --import private-key.pgp
 *
 * You may also need to:
 *
 *  gpg --edit-key gpg-key-id
 *  trust
 *  5
 *  y
 *
 * RESOURCES
 * ---------
 * These resources (among others) were helpful in this implementation:
 * https://raw.githubusercontent.com/bcgit/bc-java/master/pg/src/main/java/org/bouncycastle/openpgp/examples/PGPExampleUtil.java
 * https://raw.githubusercontent.com/bcgit/bc-java/master/pg/src/main/java/org/bouncycastle/openpgp/examples/KeyBasedFileProcessor.java
 * https://stackoverflow.com/questions/19131723/bouncycastle-openpgp-issue
 *
 * EXAMPLE
 * -------
 * import com.cartera.dig.common.security.DecryptedInputStreamPGP;
 * import java.io.FileInputStream;
 * import java.io.FileOutputStream;
 * import java.io.InputStream;
 * import java.io.OutputStream;
 * import org.apache.commons.io.IOUtils;
 *
 * // Note that the privateKeyFile below can also
 * // alternatively be the ~/.gnupg/secring.gpg file.
 * // This DOES NOT YET handle the newer (gpg 2.1) pubringkbx file format.
 * //
 * String privateKeyFile = "your-private-key.pgp";
 * String encryptedFile  = "your-encrypted-file.txt";
 * String decryptedFile  = "your-decrypted-file.txt";
 *
 * InputStream  encryptedFileInput  = new FileInputStream(encryptedFile);
 * OutputStream decryptedFileOutput = new FileOutputStream(decryptedFile);
 * InputStream  decryptedInput      = new DecryptedInputStreamPGP(encryptedFileInput, privateKeyFile);
 *
 * IOUtils.copy(decryptedInput, decryptedFileOutput);
 *
 * decryptedInput.close();
 * decryptedFileOutput.close();
 */
public class DecryptedInputStreamPGP extends InputStream {

    static { new JceSetupHack(); }

    private InputStream decryptedInputStream;

    /**
     * Constructor for input from the given InputStream with decryption
     * using the given PGP privateKeyFile.
     *
     * Note that the privateKeyFile may be either the actual .pgp file or
     * the/a GPG key-ring file, i.e. e.g. ~/.gnupg/secring.gpg. BUT this
     * does NOT (yet) handle the newer (gpg 2.1) pubring.kbx file format.
     *
     * Other constructors are variations of this allowing the caller to omit
     * the password, or to use filename (String) values for inputStream or privateKeyFile.
     *
     * @param inputStream The underlying InputStream from which we decrypt the input.
     * @param privateKeyFile The private-key file (or secring.gpg file).
     * @param password An optional password.
     */
    public DecryptedInputStreamPGP(InputStream inputStream, File privateKeyFile, String password) throws Exception {

        Security.addProvider(new BouncyCastleProvider());

        JcaPGPObjectFactory inputObjects =
            new JcaPGPObjectFactory(PGPUtil.getDecoderStream(inputStream));

        // First object might be a PGP marker packet.

        PGPEncryptedDataList encryptedDataList;
        Object inputFirstObject = inputObjects.nextObject();
        if (inputFirstObject instanceof PGPEncryptedDataList) {
            encryptedDataList = (PGPEncryptedDataList) inputFirstObject;
        } else {
            encryptedDataList = (PGPEncryptedDataList) inputObjects.nextObject();
        }

        // Find the private-key.

        PGPSecretKeyRingCollection secretKeyRings =
            new PGPSecretKeyRingCollection
                (PGPUtil.getDecoderStream(new FileInputStream(privateKeyFile)),
                                          new JcaKeyFingerprintCalculator());
        PGPPrivateKey privateKey = null;
        PGPPublicKeyEncryptedData encryptedData = null;
        Iterator encryptedDataIterator = encryptedDataList.getEncryptedDataObjects();
        while ((privateKey == null) && encryptedDataIterator.hasNext()) {
            encryptedData = (PGPPublicKeyEncryptedData) encryptedDataIterator.next();
            privateKey = getPrivateKey(secretKeyRings, encryptedData.getKeyID(), password);
        }
        if (privateKey == null) {
            throw new IllegalArgumentException("Private-key for message not found.");
        }

        JcaPGPObjectFactory messageObjects = new JcaPGPObjectFactory(
            encryptedData.getDataStream
                (new JcePublicKeyDataDecryptorFactoryBuilder().setProvider("BC").build(privateKey)));

        Object message = messageObjects.nextObject();

        if (message instanceof PGPCompressedData) {
            PGPCompressedData compressedMessage = (PGPCompressedData) message;
            JcaPGPObjectFactory compressedMessageObjects =
                new JcaPGPObjectFactory(compressedMessage.getDataStream());
            message = compressedMessageObjects.nextObject();
        }
        if (message instanceof PGPLiteralData) {
            PGPLiteralData literalMessage = (PGPLiteralData) message;
            decryptedInputStream = literalMessage.getInputStream();

        } else {
            throw new IllegalArgumentException("Cannot create DecryptedInputStreamPGP.");
        }
    }

    public DecryptedInputStreamPGP(InputStream inputStream, String privateKeyFile, String password) throws Exception {
        this(inputStream, new File(privateKeyFile), password);
    }

    public DecryptedInputStreamPGP(File inputFile, File privateKeyFile, String password) throws Exception {
        this(new FileInputStream(inputFile), privateKeyFile, password);
    }

    public DecryptedInputStreamPGP(File inputFile, String privateKeyFile, String password) throws Exception {
        this(new FileInputStream(inputFile), new File(privateKeyFile), password);
    }

    public DecryptedInputStreamPGP(String inputFile, File privateKeyFile, String password) throws Exception {
        this(new FileInputStream(inputFile), privateKeyFile, password);
    }

    public DecryptedInputStreamPGP(String inputFile, String privateKeyFile, String password) throws Exception {
        this(new FileInputStream(inputFile), new File(privateKeyFile), password);
    }

    public DecryptedInputStreamPGP(InputStream inputStream, File privateKeyFile) throws Exception {
        this(inputStream, privateKeyFile, null);
    }

    public DecryptedInputStreamPGP(InputStream inputStream, String privateKeyFile) throws Exception {
        this(inputStream, new File(privateKeyFile), null);
    }

    public DecryptedInputStreamPGP(File inputFile, File privateKeyFile) throws Exception {
        this(new FileInputStream(inputFile), privateKeyFile, null);
    }

    public DecryptedInputStreamPGP(File inputFile, String privateKeyFile) throws Exception {
        this(new FileInputStream(inputFile), new File(privateKeyFile), null);
    }

    public DecryptedInputStreamPGP(String inputFile, File privateKeyFile) throws Exception {
        this(new FileInputStream(inputFile), privateKeyFile, null);
    }

    public DecryptedInputStreamPGP(String inputFile, String privateKeyFile) throws Exception {
        this(new FileInputStream(inputFile), new File(privateKeyFile), null);
    }

    @Override
    public int read() throws IOException {
        return decryptedInputStream.read();
    }

    @Override
    public int read(byte[] bytes) throws IOException {
        return decryptedInputStream.read(bytes);
    }

    @Override
    public int read(byte[] bytes, int offset, int length) throws IOException {
        return decryptedInputStream.read(bytes, offset, length);
    }

    @Override
    public long skip(long n) throws IOException {
        return decryptedInputStream.skip(n);
    }

    @Override
    public int available() throws IOException {
        return decryptedInputStream.available();
    }

    @Override
    public void close() throws IOException {
        decryptedInputStream.close();
    }

    @Override
    public void mark(int limit) {
        decryptedInputStream.mark(limit);
    }

    @Override
    public boolean markSupported() {
        return decryptedInputStream.markSupported();
    }

    private static PGPPrivateKey getPrivateKey(PGPSecretKeyRingCollection secretKeyRings, long privateKeyId, String password) throws PGPException, NoSuchProviderException {
        PGPSecretKey secretKey = secretKeyRings.getSecretKey(privateKeyId);
        if (secretKey == null) {
            return null;
        }
        char[] passwordChars = password != null ? password.toCharArray() : "".toCharArray();
        return secretKey.extractPrivateKey(new JcePBESecretKeyDecryptorBuilder().setProvider("BC").build(passwordChars));
    }
}
