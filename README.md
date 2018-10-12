<<<<<<< HEAD
﻿# OPTIGA&trade; Trust X Arduino library for BootCamp
=======
﻿# Infineon OPTIGA&trade; Trust X Boot Camp <Mission Arduino>
>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e
![TrustXPackage](https://github.com/Infineon/Assets/raw/master/Pictures/OPTIGA-Trust-X.png)

Infineon's [OPTIGA&trade; Trust X] Boot Camp (https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/optiga-trust-x-sls-32aia/) security solution library for Arduino platform

## Purpose of the Boot Camp
The Boot Camp aims to instil confidence and product knowledge of Infineon OPTIGA&trade; Trust X. The sample codes allow you to hands on with the integration library and perform some useful tasks. At the end of this Boot Camp session, you will be able to understand the capabilities of the Library and appreciate hardware security that can protects your system.    

## Hardware
In this training, XMC1100 2Go (Infineon Technologies, XMC1100, Cortex M0), unless otherwise stated, will be used as the security host controller.

The wiring to your arduino board depends on the [evaluation board](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-x-eval-kit/) or the
Shield2GO (link pending) you are using.

[Mission Arduino - XMC2Go with Shield2Go]
(https://easyeda.com/gilatoes/mission-1-xmc2go)

## Getting Started
Install Arduino IDE 1.8.1.
[Arduino V1.8.1]https://www.arduino.cc/download_handler.php?f=/arduino-1.8.1-windows.zip


## Library Installation

### Integration of Library
Please download this repository from GitHub either from the latest [release](https://github.com/Infineon/arduino-optiga-trust-x/releases) of directly [here](https://github.com/Infineon/arduino-optiga-trust-x/archive/v1.0.3.zip).
## Mission Arduino Installation
Clone or download this github.

To install the OPTIGA&trade; Trust X library in the Arduino IDE several options can be taken:
1. via [Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3) you can find and control this library. Simply type in the Search Field **OPTIGATrustX**
1. **Sketch** > **Include Library** > **Add .ZIP Library...** in the Arduino IDE and navigate to the downloaded .ZIP file of this repository. The library will be installed in your Arduino sketch folder in libraries and you can select as well as include this one to your project under **Sketch** > **Include Library** > **OPTIGATrustX**.

![Install Library](https://raw.githubusercontent.com/infineon/assets/master/Pictures/Library_Install_ZIP.png)

<<<<<<< HEAD
## Supported Devices
In general, the library should be compatible with any Arduino board, however it has been tested for the following platforms:
* XMC1100 2Go (Infineon Technologies, XMC1100, Cortex M0)
=======
>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e

## Usage
The library is equipped with eight groups of examples which can be found on the following path: File->Examples>OPTIGATrustX
The following sections describe all the examples in more detail.

<<<<<<< HEAD
### helloBootCamp
helloBootCamp example helps to verify the I2C host controller board is working by printing some information on the serial monitor and getting keyboard input.
The default serial monitor is using 38400 baud, 8 bit None parity and 1 stop bit.
Using other serial terminal program such as Tera Term, the XMC1100 serial port will shows up as "JLink CDC UART Port".

### getUniqueID
getUniqueID example demonstrate simple reading of OPTIGA™ Trust X unique chip ID. The chip ID consists of 27 bytes and can be decoded using the Data structure Coprocessor UID table found in the solution reference manual.

### getCertificate
getCertificate example reads a digital certificate stored within OPTIGA™ Trust X Chip Object ID location 0xE0E0. This certificate is encoded in X.509 format. A X.509 certificate contains a public key and an identity and is signed by Infineon Certificate Authority (CA).

### getRandom
getRandom example uses OPTIGA™ Trust X to generate true random numbers. This example output is configurable for various random numbers length (16, 32, 64, 128, 256). High quality random number is very important for cryptography application.

=======
>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e

### calculateHash
calculateHash demonstrates example usage of the SHA256 hash, as well as a simple benchamarking for your microcontroller.
The performance of this benchmark test greatly depends on I2C  bus frame size (it affects mainly big blocks of data transmitted to the OPTIGA™ Trust X chip for hashing),
which was limited by default down to 32 bytes (in case of 32 bytes the library will perform fragmentation).

<<<<<<< HEAD
=======


>>>>>>> 432b7a5016f7bef23a40852a328edda857c7766e
### calculateSignVerifySign  
calculateSignVerifySign demonstrates signature generation and signature verification methods of the library.
This example shows two modes of operation:
1) Calculate a signature using manufacturer private key, the result value is then verified  against the public key
2) Generate a public-private keypair and store the latter inside one of Object IDs of the OPTIGA™ Trust X,
then sign the digest giving only the latter  Object ID, the result value is then verified  against the public key.  

For the verification three methods are available:
1) with a given raw public key
2) with Object ID pointing to the memory slot where the public key is located,
3) if neither Object ID nor raw public key were specified, the function will use a default Object ID with manufacturer public key certificate.

### generateKeypair
calculateSignVerifySign demonstrates methods for keypair generation, either with a private key export, or without.
In the latter case the developer should specify the Object ID of the private key.

### testFullAPI
testFullAPI is used to briefly test major API calls to the library. The expected output of this function can be found in Figure below.
*Certificate output might be different

![testFullAPI expected result](https://github.com/Infineon/Assets/raw/master/Pictures/OPTIGA%20Trust%20X%20testFullAPI.png)

## Available functions
Available API is discribed in Wiki of the repository

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process of submitting pull requests to us.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
