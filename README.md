# Bootcamp for Infineon OPTIGA&trade; Trust X

﻿## Infineon OPTIGA&trade; Trust X <Mission Arduino>

Infineon's [OPTIGA&trade; Trust X] is a security controller (https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/optiga-trust-x-sls-32aia/)

## Purpose of the Boot Camp
The Boot Camp aims to instil confidence and product knowledge of Infineon OPTIGA&trade; Trust X. The sample codes allow you to hands on with the integration library and perform some useful tasks. At the end of this Boot Camp session, you will be able to understand the capabilities of the Library and appreciate hardware security that can protects your system.    

## Hardware
In this training, XMC2Go (Infineon Technologies, XMC1100, Cortex M0), unless otherwise stated, will be used as the security host controller.

The wiring to your arduino board depends on the [evaluation board](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-x-eval-kit/) or the
Shield2GO (link pending) you are using.

[Mission Arduino - XMC2Go with Shield2Go]
(https://easyeda.com/gilatoes/mission-1-xmc2go)

## Getting Started
Install Arduino IDE 1.8.1.
[Arduino V1.8.1] https://www.arduino.cc/download_handler.php?f=/arduino-1.8.1-windows.zip

## XMC Host Library for Arduino
Infineon XMC support for Arduino platform.
[XMC-for-Arduino] https://github.com/Infineon/XMC-for-Arduino

## Trust X Library Installation

### Mission Arduino with Trust X Integration library
Clone or download this github.

Please download this repository from GitHub either from the latest [release](https://github.com/Infineon/arduino-optiga-trust-x/releases) of directly [here](https://github.com/Infineon/arduino-optiga-trust-x/archive/v1.0.3.zip).

To install the OPTIGA&trade; Trust X library in the Arduino IDE several options can be taken:
1. via [Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3) you can find and control this library. Simply type in the Search Field **OPTIGATrustX**
1. **Sketch** > **Include Library** > **Add .ZIP Library...** in the Arduino IDE and navigate to the downloaded .ZIP file of this repository. The library will be installed in your Arduino sketch folder in libraries and you can select as well as include this one to your project under **Sketch** > **Include Library** > **OPTIGATrustX**.

![Install Library](https://raw.githubusercontent.com/infineon/assets/master/Pictures/Library_Install_ZIP.png)

<<<<<<< HEAD
## Bootcamp hardware
The following microcontroller will be used for this Bootcamp.
* XMC1100 XMC2Go (Infineon Technologies, XMC1100, Cortex M0)
[XMC2Go](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc_2go_xmc1100_v1/).

=======
>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e

## Examples and Exercises
The library is equipped with eight groups of examples which can be found on the following path: File->Examples>OPTIGATrustX
The following sections describe all the examples in more detail.

<<<<<<< HEAD
### helloBootCamp
helloBootCamp example helps to verify the I2C host controller board is working by printing some information on the serial monitor and getting keyboard input.
The default serial monitor is using 115200 baud, 8 bit None parity and 1 stop bit.
Using other serial terminal program such as Tera Term, the XMC1100 serial port will shows up as "JLink CDC UART Port".

From the File->Preferences, it is useful to enable the "Show verbose output during compilation and upload." As the compilation and uploading can takes a few minutes enabling verbose output enables monitoring and debugging the software build process.

### getUniqueID
getUniqueID example demonstrate reading of OPTIGA™ Trust X unique chip ID from data object 0xE0C2. The chip ID consists of 27 bytes and can be decoded using the Data structure Coprocessor UID table found in the solution reference manual.

### getRandom
getRandom example uses OPTIGA™ Trust X to generate true random numbers. This example output is configurable for various random numbers length (16, 32, 64, 128, 256).
High quality random number is very important for cryptography application.

### getCertificate
getCertificate example reads a digital certificate stored within OPTIGA™ Trust X Chip data Object 0xE0E0. The certificate is encoded in X.509 format and dump out as hex output.
A X.509 certificate contains a public key and an identity that is signed by Infineon Certificate Authority (CA). The certificate can be copy-and-paste into a Hex to Base64 convertor.
Once the certificate is encoded into Base64 format, it should be enclosed with "-----BEGIN CERTIFICATE----- " and end with "-----END CERTIFICATE----- ". and saved as PEM format.

[Hex and PEM (Base64) Converter] https://holtstrom.com/michael/tools/hextopem.php

Either use a local OpenSSL software or an online openssl tool to decode the certificate.
[Certificate Decoder] https://www.sslshopper.com/certificate-decoder.html

OpenSSL command:
openssl x509 -in certificate.pem -text -noout

=======
>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e

### calculateHash
calculateHash demonstrates example usage of the SHA256 hash, as well as a simple benchmarking for your microcontroller.
The performance of this benchmark test greatly depends on I2C bus frame size (it affects mainly big blocks of data transmitted to the OPTIGA™ Trust X chip for hashing),
which was limited by default down to 32 bytes (in case of 32 bytes the library will perform fragmentation).

<<<<<<< HEAD
=======

>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e
### calculateSignVerifySign  
calculateSignVerifySign demonstrates signature generation and signature verification methods of the library.
This example shows two modes of operation:
1) Calculate a signature using manufacturer private key, the result value is then verified  against the public key
2) Generate a public-private keypair and store the latter inside one of Object IDs of the OPTIGA™ Trust X,
then sign the digest giving only the latter Object ID, the result value is then verified  against the public key.  

For the verification three methods are available:
1) with a given raw public key
2) with Object ID pointing to the memory slot where the public key is located,
3) if neither Object ID nor raw public key were specified, the function will use a default Object ID with manufacturer public key certificate.

### generateKeypair
generateKeypair example is a OPTIGA™ Trust X toolbox feature that demonstrates ECC256 keypair generation. The private key can either be exported in plaintext or held within the security controller.
If the private key is to be stored in the security controller, the example should be modified to specify the destination Object ID.
The generated key pair can be used for cryptographic operations.

### One-Way Authentication
one-way authentication example is used to verify the authenticity of OPTIGA™ Trust X chip. It makes use of OPTIGA™ Trust X chip crypto functions to implement the authentication function.

## Troubleshooting
1. There are many ways of getting the Arduino platform to work. However, it is advisable to counter check with the original Arduino IDE to duplicate the issue.
2. Ensure that you can execute the helloBootCamp example before moving to the crypto examples.
3. Some serial terminals might have different setup. When in doubt, fall back to Arduino serial monitor.  

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process of submitting pull requests to us.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
