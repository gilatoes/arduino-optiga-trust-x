# Boot Camp for Infineon OPTIGA&trade; Trust X

## Infineon OPTIGA&trade; Trust X <Mission Arduino>

Infineon's [OPTIGA&trade; Trust X] is a security controller (https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/optiga-trust-x-sls-32aia/)

## Objectives of the OPTIGA&trade; Trust X Boot Camp
The Boot Camp aims to guide its participant the fundamental concepts and principles of information security using Infineon OPTIGA&trade; Trust X (hereafter called Trust X). These concepts and principles are relevant in all computing environments regardless of hardware platform or operating systems. Apart from the theory, the Boot Camp emphasis on kinesthetic learning and activities  which further instil confidence and product knowledge.

A hardware platform with Infineon XMC2Go MCU and Trust X magnetic connector (hereafter called Exerciser) together with Arduino environment will be used as the embedded platform. Example codes a.k.a sketches in Arduino are provided with the Trust X integration library (hereafter called Library). These sample codes perform some useful tasks using the Library API are good starting point for the Mission and Tasks section. Apart from Arduino development, participants will also be utilizing the OpenSSL - most popular cryptography library to complete some of the more complex tasks.

At the end of this Boot Camp session, participants will understand basic security concepts, capabilities of the Library and applying Trust X hardware in real-world application.

***Important***: The Trust X driver source code used in this Boot Camp is forked from Infineon Github, frozen and modified for training purposes. Hence, it is neither updated regularly nor qualify for production. Please refer to official Infineon Github https://github.com/Infineon/arduino-optiga-trust-x for the updated and latest driver release.

## Getting Started

The Boot Camp is designed based on the assumption that its trainee had little or no prior knowledge on information security.

It is recommended that using a Windows OS computer (Windows 10 64-bit preferred) with at least 5GB of free disk space for all the software tools and documentation. You must have administration rights for software installation and execution.

### Exerciser Information
For make-it-yourself board, please refer to schematic and PCB information [Mission Arduino - XMC2Go with Shield2Go]<br/>
(https://easyeda.com/gilatoes/mission-1-xmc2go)

### kinesthetic Tasks
Follow the detail setup here:
https://github.com/gilatoes/arduino-optiga-trust-x/blob/master/Missions/MissionTasks.md

## Examples and sample codes
The Library is equipped with eleven examples which can be found on the following path: **File**->**Examples**>**OPTIGATrustX**.

The following section describe all the examples in detail.
### E00_Minimal
The minimal example contains less than 10 lines of codes needed for a sketch to compile in the Arduino Software (IDE). There are only setup() and loop() methods.

### E01_HelloBootCamp
HelloBootCamp example helps to verify that the XMC and Trust X library are compiled correctly. Compiling and uploading the code should display both version number of the libraries indicating that the library has been executed correctly.

### E02_ReadMaxCommBufferSize
ReadMaxCommBufferSize example is a skeleton example program which calls the Trust X I2C library. As the I2C is already working on the platform, you will be task to perform certain modification such that you will need to explore the underlying library and its APIs.

### E03_GetUniqueID
GetUniqueID example demonstrate reading of Trust X unique chip ID from data object 0xE0C2. The chip ID consists of 27 bytes and can be decoded using the Data structure Coprocessor UID table found in the solution reference manual.

### E04_GetRandom
GetRandom example uses Trust X to generate true random numbers. This example output is configurable for various random numbers length (**16**, **32**, **64**, **128**, **256**).
High quality random number is very important for cryptography application.

### E05_GetCertificate
GetCertificate example reads a digital certificate stored within Trust X data Object 0xE0E0. The certificate is encoded in X.509 format and dump out as hex output.
A X.509 certificate contains a public key and an identity that is signed by Infineon Certificate Authority (CA). The certificate can be copy-and-paste into a Hex to Base64 convertor.
Once the certificate is encoded into Base64 format, it should be enclosed with the following text.

```certificate
"-----BEGIN CERTIFICATE----- "
....
....
"-----END CERTIFICATE----- ".
```

[Hex and PEM (Base64) Converter] https://holtstrom.com/michael/tools/hextopem.php

Either use a local OpenSSL software or an online openssl tool to decode the certificate.
[Certificate Decoder] https://www.sslshopper.com/certificate-decoder.html

OpenSSL command:
```
openssl x509 -in certificate.pem -text -noout
```

### E06_CalculateHash
CalculateHash is an example that demonstrates the usage of Trust X SHA256 toolbox function. There is also a simple hash benchmarking time for your microcontroller. The performance of this benchmark test greatly depends on I2C bus frame size (it affects mainly big blocks of data transmitted to the Trust X chip for hashing), which was limited by default down to 32 bytes (in case of 32 bytes the library will perform fragmentation).

### E07_SignVerify
SignVerify demonstrates signature generation and verification methods of the library.
This example shows two modes of operation:
1) Calculate a signature using manufacturer (in this case, Infineon) private key, the resulted value is verified against the public key.
2) Generate a new public-private keypair and store the latter inside one of Object IDs of the Trust X,
then sign the digest giving only the latter Object ID, the result value is verified against the public key.  

### E08_OneWayAuth
One-way authentication example is used to verify the authenticity of Trust X chip. It makes use of Trust X chip crypto functions to implement the authentication function.

### E09_GenerateKeypair
GenerateKeypair example is a Trust X toolbox feature that demonstrates ECC256 keypair generation. The private key can either be exported in plaintext or held within the security controller.
If the private key is to be stored in the security controller, the example should be modified to specify the destination Object ID.
The generated key pair can be used for cryptographic operations.

### E10_PseudoTLS
PseudoTLS is a simplified example of a TLS session. This example establishes secure channel by generating 2 public-private key pairs within the same Trust X. The public keys are "exchanged" and used to generate common shared secret. This process of establishing common shared secret is known as ECDH. The shared secrets can be used to compute a secret key using the Key derivation function (KDF). KDF can be used to stretch keys into longer keys or converting into symmetric key application. In this case, the derived secret key is used as ephemeral AES encryption-decryption key to securely transmit encrypted messages over insecure medium.

## Helper Routines
### H01ObjectDump
objectDump is a helper routine that displays all Trust X objects. These objects including all data objects and status objects.
This is especially useful for certificates locations.

## Troubleshooting
1. There are many ways of getting the Arduino platform to work. However, it is advisable to counter check with the original Arduino IDE to duplicate the issue.
2. Ensure that you can execute the HelloBootCamp example before moving to the crypto examples.
3. Some serial terminals might have different setup. When in doubt, fall back to Arduino serial monitor.  


## Contribution

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process of submitting pull requests to us.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
