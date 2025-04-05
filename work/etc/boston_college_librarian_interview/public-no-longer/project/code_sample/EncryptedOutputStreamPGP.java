package com.cartera.dig.common.crypto;

import java.io.OutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.IOException;
import java.security.SecureRandom;
import java.util.Date;
import java.util.Iterator;
import java.io.FileOutputStream;
import java.security.Security;
import org.bouncycastle.bcpg.ArmoredOutputStream;
import org.bouncycastle.gpg.keybox.bc.BcKeyBox;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.gpg.keybox.KeyBox;
import org.bouncycastle.gpg.keybox.KeyBlob;
import org.bouncycastle.gpg.keybox.BlobType;
import org.bouncycastle.gpg.keybox.PublicKeyRingBlob;
import org.bouncycastle.openpgp.PGPCompressedData;
import org.bouncycastle.openpgp.PGPCompressedDataGenerator;
import org.bouncycastle.openpgp.PGPEncryptedData;
import org.bouncycastle.openpgp.PGPEncryptedDataGenerator;
import org.bouncycastle.openpgp.PGPException;
import org.bouncycastle.openpgp.PGPLiteralData;
import org.bouncycastle.openpgp.PGPLiteralDataGenerator;
import org.bouncycastle.openpgp.PGPPublicKey;
import org.bouncycastle.openpgp.PGPPublicKeyRing;
import org.bouncycastle.openpgp.PGPPublicKeyRingCollection;
import org.bouncycastle.openpgp.PGPUtil;
import org.bouncycastle.openpgp.operator.jcajce.JcaKeyFingerprintCalculator;
import org.bouncycastle.openpgp.operator.jcajce.JcePGPDataEncryptorBuilder;
import org.bouncycastle.openpgp.operator.jcajce.JcePublicKeyKeyEncryptionMethodGenerator;

/**
 * This is an OutputStream implementation which encrypts its output to the specified
 * underlying OutputStream using the given PGP public-key.
 *
 * IMPORTANT NOTE
 * --------------
 * See the DecryptedInputStreamPGP module for an important note on the requirement
 * for Java Cryptography Extension (JCE) Unlimited Strength Jurisdiction Policy Files.
 *
 * DECRYPTION
 * ----------
 * The recipient of a file encrypted using this class can of course be decrypted
 * using the accompanying DecryptedInputStreamPGP class, but can also be
 * decrypted using the gpg command like the below:
 *
 *  gpg -r pgp-key-id -o decrypted-file -e encrypted-file
 *
 * where the pgp-key-id is the ID in your PGP key store associated
 * with the public-key used to encrypt the file. These IDs can be seen using:
 *
 *  gpg --list-keys --keyid-format short
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
 * import com.cartera.dig.common.security.EncryptedOutputStreamPGP;
 * import java.io.FileInputStream;
 * import java.io.FileOutputStream;
 * import java.io.InputStream;
 * import java.io.OutputStream;
 * import org.apache.commons.io.IOUtils;
 *
 * // Note that the publicKeyFile below can also
 * // alternatively be the ~/.gnupg/pubring.gpg file.
 * // This also handles the newer (gpg 2.1) pubring.kbx file format.
 * //
 * String publicKeyFile  = "your-public-key.pgp";
 * String plaintextFile  = "your-plaintext-file.txt";
 * String encryptedFile  = "your-encrypted-file.txt";
 * int encryptionOptions = EncryptedOutputStreamPGP.OPTION_INTEGRITY | EncryptedOutputStreamPGP.OPTION_ARMOR;
 *
 * InputStream  plaintextFileInput  = new FileInputStream(plaintextFile);
 * OutputStream encryptedFileOutput = new FileOutputStream(encryptedFile);
 * OutputStream encryptedOutput     = new EncryptedOutputStreamPGP(encryptedFileOutput, publicKeyFile, encryptionOptions);
 *
 * IOUtils.copy(plaintextFileInput, encryptedOutput);
 *
 * plaintextFileInput.close();
 * encryptedOutput.close();
 */
public class EncryptedOutputStreamPGP extends OutputStream {

    static { new JceSetupHack(); }

    public static final int OPTION_INTEGRITY  = 0x0010;
    public static final int OPTION_COMPRESS   = 0x0020;
    public static final int OPTION_ARMOR      = 0x0040;
    public static final int OPTION_KBX        = 0x0080;
    public static final int OPTION_DEFAULT    = OPTION_INTEGRITY;

    private int                         encryptionOptions          = OPTION_DEFAULT;
    private OutputStream                encryptedOutputStream      = null;
    //
    // For some reason it seems necessary to individually close each OutputStream we
    // use; one would think that just closing the final enclosing one would be enough;
    // this is the only reason we have to save away these individual output streams,
    // i.e. to explicitly individually close on close. Similarly we close each of the
    // data generators for thoroughness, though found examples don't necessarily do.
    //
    private OutputStream               compressedDataOutputStream = null;
    private OutputStream               encryptedDataOutputStream  = null;
    private OutputStream               armoredOutputStream        = null;
    private PGPLiteralDataGenerator    literalDataGenerator       = null;
    private PGPCompressedDataGenerator compressedDataGenerator    = null;
    private PGPEncryptedDataGenerator  encryptedDataGenerator     = null;

    private static final int BUFFER_SIZE = 4096;

    public boolean hasIntegrity() { return (encryptionOptions & OPTION_INTEGRITY) != 0; }
    public boolean isCompressed() { return (encryptionOptions & OPTION_COMPRESS)  != 0; }
    public boolean isArmored()    { return (encryptionOptions & OPTION_ARMOR)     != 0; }

    /**
     * Constructor for output to the given OutputStream with encryption using the
     * given PGP publicKeyFile, and optional keyId, and any options ORed
     * together (OPTION_INTEGRITY | OPTION_COMPRESS | OPTION_ARMOR).
     *
     * Note that the publicKeyFile may be either the actual .pgp file or
     * the/a GPG key-ring file, i.e. e.g. ~/.gnupg/pubring.gpg. And this
     * also handles the newer (gpg 2.1) pubring.kbx file format.
     *
     * Other constructors are variations of this allowing the caller to omit keyId,
     * options, or to use filename (String) values for outputStream or publicKeyFile.
     *
     * @param outputStream The underlying OutputStream to which we encrypt the output.
     * @param publicKeyFile The PGP public-key file (or pubring.gpg file).
     * @param keyId The (8-16 hex character) key-id or null to pick the first public-key.
     * @param options Any of these options ORed together: OPTION_INTEGRITY | OPTION_COMPRESS | OPTION_ARMOR
     */
    public EncryptedOutputStreamPGP(OutputStream outputStream, File publicKeyFile, String keyId, int options) throws Exception {

        Security.addProvider(new BouncyCastleProvider());

        if ((encryptionOptions = options) == 0) {
            encryptionOptions = OPTION_DEFAULT;
        }

        PGPPublicKey publicKey = getPublicKey(publicKeyFile, keyId);

        encryptedOutputStream = outputStream;

        if (isArmored()) {
            armoredOutputStream = new ArmoredOutputStream(outputStream);
            encryptedOutputStream = armoredOutputStream;
        }

        // Seems that CAST5 is recommended for maximum compatibility.
        // Ran into problems at least with PGPEncryptedData.AES_256.
        //
        encryptedDataGenerator =
            new PGPEncryptedDataGenerator
             // (new JcePGPDataEncryptorBuilder(PGPEncryptedData.AES_256).
                (new JcePGPDataEncryptorBuilder(PGPEncryptedData.CAST5).
                     setWithIntegrityPacket(hasIntegrity()).
                     setSecureRandom(new SecureRandom()).setProvider("BC"));
        encryptedDataGenerator.addMethod(new JcePublicKeyKeyEncryptionMethodGenerator(publicKey).setProvider("BC"));
        encryptedDataOutputStream =
            encryptedDataGenerator.open
                (encryptedOutputStream, new byte[BUFFER_SIZE]);
        encryptedOutputStream = encryptedDataOutputStream;

        if (isCompressed()) {
            compressedDataGenerator = new PGPCompressedDataGenerator(PGPCompressedData.ZIP);
            compressedDataOutputStream =
                compressedDataGenerator.open
                    (encryptedOutputStream, new byte[BUFFER_SIZE]);
            encryptedOutputStream = compressedDataOutputStream;
        }

        literalDataGenerator = new PGPLiteralDataGenerator();
        //
        // Note the third argument here; it is supposedly a filename,
        // but thankfully it is not needed and doesn't seem to do anything.
        // We just want an OutputStream without reference to any filename.
        //
        encryptedOutputStream =
            literalDataGenerator.open
                (encryptedOutputStream, PGPLiteralData.BINARY, "", new Date(), new byte[BUFFER_SIZE]);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, String publicKeyFile, String keyId, int options) throws Exception {
        this(outputStream, new File(publicKeyFile), keyId, options);
    }

    public EncryptedOutputStreamPGP(File outputFile, File publicKeyFile, String keyId, int options) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, keyId, options);
    }

    public EncryptedOutputStreamPGP(File outputFile, String publicKeyFile, String keyId, int options) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), keyId, options);
    }

    public EncryptedOutputStreamPGP(String outputFile, File publicKeyFile, String keyId, int options) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, keyId, options);
    }

    public EncryptedOutputStreamPGP(String outputFile, String publicKeyFile, String keyId, int options) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), keyId, options);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, File publicKeyFile, String keyId) throws Exception {
        this(outputStream, publicKeyFile, keyId, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, String publicKeyFile, String keyId) throws Exception {
        this(outputStream, new File(publicKeyFile), keyId, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(File outputFile, File publicKeyFile, String keyId) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, keyId, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(File outputFile, String publicKeyFile, String keyId) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), keyId, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(String outputFile, File publicKeyFile, String keyId) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, keyId, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(String outputFile, String publicKeyFile, String keyId) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), keyId, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, File publicKeyFile, int options) throws Exception {
        this(outputStream, publicKeyFile, null, options);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, String publicKeyFile, int options) throws Exception {
        this(outputStream, new File(publicKeyFile), null, options);
    }

    public EncryptedOutputStreamPGP(File outputFile, File publicKeyFile, int options) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, null, options);
    }

    public EncryptedOutputStreamPGP(File outputFile, String publicKeyFile, int options) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), null, options);
    }

    public EncryptedOutputStreamPGP(String outputFile, File publicKeyFile, int options) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, null, options);
    }

    public EncryptedOutputStreamPGP(String outputFile, String publicKeyFile, int options) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), null, options);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, File publicKeyFile) throws Exception {
        this(outputStream, publicKeyFile, null, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(OutputStream outputStream, String publicKeyFile) throws Exception {
        this(outputStream, new File(publicKeyFile), null, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(File outputFile, File publicKeyFile) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, null, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(File outputFile, String publicKeyFile) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), null, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(String outputFile, File publicKeyFile) throws Exception {
        this(new FileOutputStream(outputFile), publicKeyFile, null, OPTION_DEFAULT);
    }

    public EncryptedOutputStreamPGP(String outputFile, String publicKeyFile) throws Exception {
        this(new FileOutputStream(outputFile), new File(publicKeyFile), null, OPTION_DEFAULT);
    }

    @Override
    public void write(int value) throws IOException {
        encryptedOutputStream.write(value);
    }

    @Override
    public void write(byte[] bytes) throws IOException {
        encryptedOutputStream.write(bytes);
    }

    @Override
    public void write(byte[] bytes, int offset, int length)  throws IOException {
        encryptedOutputStream.write(bytes, offset, length);
    }

    @Override
    public void flush() throws IOException {
        encryptedOutputStream.flush();
    }

    @Override
    public void close() throws IOException {
        encryptedOutputStream.flush();
        encryptedOutputStream.close();
        if (compressedDataOutputStream != null) {
            compressedDataOutputStream.close();
        }
        encryptedDataOutputStream.close();
        if (armoredOutputStream != null) {
            armoredOutputStream.close();
        }
        literalDataGenerator.close();
        if (compressedDataGenerator != null) {
            compressedDataGenerator.close();
        }
        encryptedDataGenerator.close();
    }

    private PGPPublicKey getPublicKey(File publicKeyFile) throws IOException, PGPException {
        return getPublicKey(publicKeyFile, null);
    }

    private PGPPublicKey getPublicKey(File publicKeyFile, String keyId) throws IOException, PGPException {
        if (keyId != null) {
            //
            // The given key-id, if given at all, should be at least 8 (hex) characters,
            // and will match a key in the key-ring if the right-most (hex) characters
            // of the key-ring key key-id matches the given key-id (hex) characters.
            //
            if (keyId.length() < 8) {
                keyId = null;
            } else {
                keyId = keyId.toUpperCase();
            }
        }
        try (InputStream publicKeyInputStream = new FileInputStream(publicKeyFile)) {
            if ((encryptionOptions & OPTION_KBX) != 0) {
                //
                // This is to handle the new style gpg (version 2.1) pubring.kbx file
                // rather than the pubring.gpg file. AFAIK Cartera servers use gpg 2.0
                // so this is not an issue, but on MacOS (laptop), we have gpg 2.2.21,
                // and so we have this pubring.kbx file to work with. And this works.
                //
                // HOWEVER, we have not yet been able to support this newer (gpg 2.1)
                // pubring.kbx file format in the DecryptedInputStreamPGP module.
                // But a file encrypted with this module with a pubring.kbx can
                // be decrypted with using the private key file explicitly.
                //
                PGPPublicKey selectedPublicKey = null;
                KeyBox keyBox = new BcKeyBox(publicKeyInputStream);
                for (KeyBlob keyBlob : keyBox.getKeyBlobs()) {
                    if (keyBlob.getType() == BlobType.OPEN_PGP_BLOB) {
                        if (keyBlob instanceof PublicKeyRingBlob) {
                            PGPPublicKeyRing publicKeyRing = ((PublicKeyRingBlob) keyBlob).getPGPPublicKeyRing();
                            PGPPublicKey publicKey = getPublicKey(publicKeyRing, keyId);
                            if (publicKey != null) {
                                return publicKey;
                            }
                        }
                    }
                }
            }
            PGPPublicKeyRingCollection publicKeyRings =
                new PGPPublicKeyRingCollection(PGPUtil.getDecoderStream(publicKeyInputStream),
                                               new JcaKeyFingerprintCalculator());
            for (PGPPublicKeyRing publicKeyRing : publicKeyRings) {
                PGPPublicKey publicKey = getPublicKey(publicKeyRing, keyId);
                if (publicKey != null) {
                    return publicKey;
                }
            }
            return null;
        }
    }

    private PGPPublicKey getPublicKey(PGPPublicKeyRing publicKeyRing, String keyId) throws IOException, PGPException {
        Iterator<PGPPublicKey> publicKeyIterator = publicKeyRing.getPublicKeys();
        while (publicKeyIterator.hasNext()) {
            PGPPublicKey publicKey = publicKeyIterator.next();
            if (publicKey.isEncryptionKey()) {
                if (keyId != null) {
                    String publicKeyId = String.format("%X", publicKey.getKeyID());
                    if (publicKeyId.endsWith(keyId)) {
                        return publicKey;
                    }
                } else {
                    //
                    // If no key-id is given then just return the first key.
                    //
                    return publicKey;
                }
            }
        }
        return null;
    }
}
