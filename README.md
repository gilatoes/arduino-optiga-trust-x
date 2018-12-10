# Boot Camp for Infineon OPTIGA&trade; Trust X

## Infineon OPTIGA&trade; Trust X <Mission Arduino>

Infineon's [OPTIGA&trade; Trust X] is a security controller (https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/optiga-trust-x-sls-32aia/)

## Objectives of the Boot Camp Mission Arduino
The section of the Boot Camp aims to instil confidence and basic product knowledge of Infineon OPTIGA&trade; Trust X (hereafter called Trust X). The sample codes a.k.a sketches in Arduino allows you to work on the OPTIGA&trade; Trust X integration library (hereafter called Library) and perform some useful tasks using the library API. At the end of this Boot Camp session, you will be able to understand the capabilities of the Library and exercise Trust X hardware.

## Getting Started

You should have administrator right for installing software.
Download and install the latest Arduino IDE version.

## XMC Host Library for Arduino IDE
Install Infineon XMC library support for Arduino platform.
Follow the instruction on the official XMC-for-Arduino Github.

[XMC-for-Arduino]<br/>
https://github.com/Infineon/XMC-for-Arduino

DAVE 4.4.2 (64-bit) download link:<br/>
https://infineoncommunity.com/dave-download_ID645

Alternate DAVE 4.4.2 download link (Valid on during the Boot Camp training only):<br/>
https://drive.google.com/file/d/1l_TAdm1e6Q3ZJnSK5oMYVVpKxm5e4ZUW/view?usp=sharing

### Mission Arduino with Trust X and Library
Clone or download this Github to get the Trust X integration library and examples.

To install Trust X Library in the Arduino IDE several options can be taken:
**Sketch** > **Include Library** > **Add .ZIP Library...** in the Arduino IDE and navigate to the downloaded .ZIP file of this repository. The library will be installed in your Arduino sketch folder in libraries and you can select as well as include this one to your project under **Sketch** > **Include Library** > **OPTIGATrustX**.

![Install Library](https://raw.githubusercontent.com/infineon/assets/master/Pictures/Library_Install_ZIP.png)

## Boot Camp Hardware
For this part of training, a XMC2Go board will be used as the MCU I2C host controller for Trust X. The MCU board comes with a JTAG programmer needs to be connected to a Windows based PC for flashing the firmware.

* XMC2Go (Infineon Technologies, XMC1100, Cortex M0)
[XMC2Go]<br/>
(https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc_2go_xmc1100_v1/).

For the ease of use, you will be issued with a Boot Camp Edition of XMC2Go with Sheidl2Go token. All the exercises should be accomplished using this mission board.

For manual wire jumping of XMC2Go board with a Trust X Shield2Go board, please refer to schematic and PCB information [Mission Arduino - XMC2Go with Shield2Go]<br/>
(https://easyeda.com/gilatoes/mission-1-xmc2go)

Once the XMC and Trust X Library has been installed on the PC, you can try to open the helloBootCamp example. Perform the **Verify** and **Upload** to flash the MCU firmware to make sure that the environment has been properly setup.

## Examples and sample codes
The Library is equipped with eleven examples which can be found on the following path: **File**->**Examples**>**OPTIGATrustX**.

The following section describe all the examples in detail.
### E00_Minimal
The minimal example contains less than 10 lines of codes needed for a sketch to compile in the Arduino Software (IDE). There are only setup() and loop() methods.

This is a good time for you to setup the environment.

From the menu, **Preferences**->"Show verbose output during compilation and upload." As the compilation and uploading can takes a few minutes enabling verbose output enables monitoring and debugging the software build process.

With the XMC2Go connected to the host PC, check the device manager for the Port->"JLink CDC UART Port".
Take note of the communication port number.
Going back to the Arduino IDE, select from the menu **Tool**->**Port**->Com number from the device manager

The default serial monitor of all the programs used in this boot camp is 115200 baud, 8 bit None parity and 1 stop bit.
When the serial monitor is activating the serial monitor, make sure that the baud rate is set correctly.

Select from the menu **Tool**->**Boards**->**XMC2Go** so that it will select the correct Infineon toolchain used for the software compilation.
Click on the Verify button and check sketch is compiling using the Infineon toolchain. If there is no print message, make sure that you have enabled verbose output.

Click on the Upload button to download the compiled binary to XMC2Go.

Click on the Serial Monitor button to show the serial output from XMC2Go. If there is no output, make sure that the baud rate is set to 115200 at the bottom of the serial monitor.

### E01_HelloBootCamp
HelloBootCamp example helps to verify that the XMC and Trust X library are compiled correctly. Compiling and uploading the code should display both version number of the libraries indicating that the library has been executed correctly.

### E02_ConfigI2C
ConfigI2C example is a skeleton example program which calls the Trust X I2C library. As the I2C is already working on the platform, you will be task to perform certain modification such that you will need to explore the underlying library and its APIs.

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
