# Tasks and Mission:
It is recommended that using a Windows OS computer (Windows 10 64-bit preferred) with at least 5GB of free disk space for all the software tools and documentation. You must have administration rights for software installation and execution.

## Task 0: Boot Camp Environment Setup

 - Follow XMC-for-Arduino GitHub for Arduino.<br/>
 https://github.com/Infineon/XMC-for-Arduino<br/>

- SEGGER J-Link (J-Link Software and Documentation pack for Windows)<br/>
https://www.segger.com/downloads/jlink</br>

- Arduino IDE version 1.8.8<br/>
https://www.arduino.cc/en/main/software<br/>

### Trust X and Library
Download this GitHub contents to get the Trust X integration library and examples.<br/>

To install Trust X Library in the Arduino IDE:<br/>
Click on the Clone or download button on GitHub. Select Download ZIP and download the GitHub repository as a Zip file. The zip file will be saved as **arduino-optiga-trust-x-master.zip** in your local disk drive.<br/>

**Sketch** > **Include Library** > **Add .ZIP Library...** in the Arduino IDE and navigate to the downloaded .ZIP file of this repository. The library will be installed in your Arduino sketch folder in libraries and you can select as well as include this one to your project under **Sketch** > **Include Library** > **OPTIGATrustX**.

![Install Library](https://raw.githubusercontent.com/infineon/assets/master/Pictures/Library_Install_ZIP.png)

### OpenSSL for Windows:<br/>
Windows version of OpenSSL can be downloaded from the following website.<br/>
https://slproweb.com/products/Win32OpenSSL.html<br/>
The latest OpenSSL version tested with this is 1.1.0j.

Alternative 64-bit OpenSSL version 1.1.0j<br/>
https://drive.google.com/open?id=1_vJvLmCBT-J4bUEO13knatdqlXihJlxA

Alternative 32-bit OpenSSL version 1.1.0j<br/>
https://drive.google.com/open?id=14hmdRW_pZ-m2mHYHeaOgdNiHAnBf8aeZ

After installing OpenSSL, the binary path location needs to be added to the Environment Path variable. This allows OpenSSL to be accessible from any folder from the computer.

### Test driving OpenSSL
Open the Command Prompt window from the Windows menu, type in "openssl version".
If OpenSSL has been installed correctly with the Path setting, it will shows the current OpenSSL version.

Apart from the simple test, another option is the "openssl speed" command that will run some cryptographic commands and benchmarking to verify that OpenSSL is operational.

### Other softwares and tools
There is a list of other software and tools useful for Boot Camp. They are fairly straight-forward to install therefore there shall be no further explaination.
1. Terminal software e.g MobaXterm (free), Tera Terminal or Putty (https://mobaxterm.mobatek.net/download.html)
2. Saleae Logic Analyzer software (https://www.saleae.com/downloads/)
3. BP Tools (https://www.eftlab.co.uk/index.php/downloads/bp-tools)
4. Source Code editor e.g Eclipse (https://www.eclipse.org/downloads/)
5. Source Code editor e.g Notepad++ (https://notepad-plus-plus.org/download/v7.6.2.html)
5. PDF reader e.g. Adobe PDF reader
6. Decompression tool eg. Zip-7

## Hardware for Boot Camp
An Exerciser board will be used as the MCU I2C host controller for Trust X.
It has a detachable but non-reversible magnetic Shield2Go token. Unless stated otherwise, all tasks should be accomplished using the Exerciser.

The Exerciser comes with a JTAG programmer that needs to be connected to a Windows based PC for flashing the firmware.

* Exerciser (Infineon Technologies, XMC1100, Cortex M0)
[XMC2Go]<br/>
(https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc_2go_xmc1100_v1/).

Once you are issued with an Exerciser, ensure that Segger, XMC and Trust X library are installed on your PC. You can try to open the helloBootCamp example.

From the menu, **Preferences**->"Show verbose output during compilation and upload." As the compilation and uploading can takes a few minutes enabling verbose output enables monitoring and debugging the software build process.

With the Exerciser connected to the host PC, check the device manager for the **Port->"JLink CDC UART Port"**.
Take note of the communication port number.
Going back to the Arduino IDE, select from the menu **Tool**->**Port**->Com number from the device manager.

The default serial monitor of all the programs used in this boot camp is **115200 baud, 8 bit None parity and 1 stop bit**.
When the serial monitor is activating the serial monitor, make sure that the baud rate is set correctly.

Select from the menu **Tool**->**Boards**->**XMC2Go** so that it will select the correct Infineon toolchain used for the software compilation.
Click on the **Verify** button and check sketch is compiling using the Infineon toolchain. If there is no print message, make sure that you have enabled verbose output.

Click on the **Upload** button to download the compiled binary to Exerciser.

Click on the **Serial Monitor** button to show the serial output from Exerciser. If there is no output, make sure that the baud rate is set to 115200 at the bottom of the serial monitor.

There should be no error during Verify and Upload.

### Task 0 Checklist:
- [ ]  Download and setup all the required software and tools.<br/>
- [ ]  After installing the Boot Camp Trust X repo, start E00_Minimal example and ensure that it can be compiled without any error.
- [ ]  Open the Windows Powershell. Alternatively, on Windows 10, there is a more advance interface calls "ise". Open ise from the Powershell prompt.
- [ ]  Ensure that OpenSSL is installed and can be accessed from the *powershell*. Use the "openssl version" to check the path accessibility. If OpenSSL is installed but cannot be found, please update the Windows System path.
- [ ]  Access the OpenSSL sandbox folder. Try out some of the OpenSSL commands from the Trust X BootCamp reference.<br/> https://github.com/gilatoes/arduino-optiga-trust-x/blob/master/doc/BootCampReference.pdf<br/>

## Task 1: HelloBootCamp
- [ ]  BootCamp orientation of hardware and software (Arduino and OpenSSL sandbox).
- [ ]  Logistics and distribution of available resources.
- [ ]  Attach the Trust X with magnetic connector to the Boot Camp Exerciser and execute the E01_HelloBootcamp example. Please make sure that the Trust X LED is lighted up.
- [ ] Trust X library is installed in the following default folder. ***C:\Users\xxxx\Documents\Arduino\libraries\arduino-optiga-trust-x\***</br>

**Questions & Discussion:**

- [ ]  What is the current XMC and Trust X library version?
- [ ]  Find out which file stores the Trust X library version number?
*Hint:* You might need to use a search tool to speed up the process.

## Task 2: Understanding I2C and GPIO
*Note:* Get your Exerciser COM port using Windows Device Manager.

In this task, there will be some changes required for your *Trust X library*.</br>
You are expected to take reference from Trust X Datasheet located in the doc folder, read up I2C fundamentals and connect up Logic analyzer and/or I2C analyzer for this task.</br>
Use E02_ReadMaxCommBufferSize sketch example as your starting point.</br>
- [ ]  Refer to Trust X datasheet V2.5 section 5.1.6.2
- [ ]  Implement warm reset using GPIO (P0.0).</br>

**Questions & Discussion:**

- [ ]  Download the pre-captured Saleae Logic file and decode the Trust X address.</br>
Right-Click and use the "Save link as..." option.</br>
https://github.com/gilatoes/arduino-optiga-trust-x/blob/master/missions/mission_files/task2/ReadUID.logicdata
- [ ]  What is the decoded I2C bus speed?
- [ ]  Modify and increase your host I2C driver to increase the I2C bus frequency. Monitor the updated bus frequency using either an I2C analyzer or logic analyzer.
- [ ]  Replace a new Trust X. Determine its address. Modify the I2C host library to communicate with this Trust X and read out its UID.
- [ ]  Restore the host library Trust X address to the original value.
- [ ]  Make sure that your host library is using the default I2C value.  
1. Implement the reset control using GPIO. Reset control improves the system stability.
2. Execute the H02_ChangeI2CAddress example which will temporarily change the I2C address.
3. If the reset is successfully implemented, the I2C address will be restored to the default value during initialization. You can use other examples such as E03_GetUniqueID to check. In the event of no proper reset, I2C address will not be found.  
4. Manually, power down and up the device will also restore the default Trust X address.  

## Task 3: Trust X Object IDs
This task consists of 2 parts. First, read and decode the Trust X UID. Second, using your issued Trust X, determine the Public Key issued by Infineon CA.
- [ ] Read the Trust X UID Object.
  * Decode the Trust X UID to determine the Trust X identity.
- [ ] Read Meta data of the Device Public Key certificate issued by Infineon data object.
- [ ] Read and decode Trust X factory default certificate. Determine the signature algorithm, certificate serial number, issue date, valid period, public key and signature value issued by Infineon CA.
*Hint:* Use E05_GetCertificate example as a starting point. You will need to convert the Hex value to Base64 value then decode it using certificate decoder tools.

## Task 4: Concepts of Digital Signature using ECC Asymmetric keys
*Note:* In this task, please use Windows Powershell and OpenSSL program.

Digital signature is a mathematical scheme for presenting the authenticity of digital messages or documents.
A valid digital signature gives a recipient reason to believe that the message was created by a known sender
(authentication), that the sender cannot deny having sent the message (non-repudiation), and that the message
was not altered in transit (integrity).

This task can be completed alone. However, it will be more fun to get a partner role play Bob (He could played dual roles) and Alice in this task.

Bob wants to sends Alice a Private Message (PM). "I am joining Infineon Trust X Boot Camp."
Alice received the PM and wonders about its integrity.

**Alice message**<br/>
https://github.com/gilatoes/arduino-optiga-trust-x/blob/master/missions/mission_files/task4/Alice-Message.txt

  - [ ] Bob generates a set of ECC keypair using ECC NIST P384 curve.
  ```
  openssl ecparam -name secp384r1 -genkey -noout -out private.key.pem

  openssl ec -in private.key.pem -outform PEM -pubout -out public.key.pem
  ```

  - [ ] Bob signs the message using his private key. Show the trainer how the message is signed using OpenSSL command.
  - [ ] Keeping his secret key private, Bob will email/transmit his public key, signature, and message hash to Alice.
  - [ ] Alice verifies the message using the received package. Show the trainer using OpenSSL command how the Bob's public key, digest and message is used to verify the signature.

  The use of public key verification process has successfully prevented Eve (Evil Robot) from impersonating Bob from sending a "fake" message to Alice. Without Bob's Secret Key, all Eve's message will be rejected by Alice.
  - [ ] As a negative use case, Eve send a message "I am not joining Infineon Boot camp" to Alice and see if the verification works.
  - [ ] Alice received the second message. She verifies the new message again using the previous signature and public key.
  - [ ] If time permits, Alice and Bob should switch roles.</br>

  **Questions & Discussion:**

  - [ ]  If we purely use the public key and secret key to perform verification of data message what is the potential problem?
  - [ ]  What is the potential weakness using such approach?
  - [ ]  After verifying the message, can Bob really trust its contents?

## Task 5: Replay attack

Study the E07_SignVerify sketch example carefully.
- [ ] Print out (using output_result()) the signature generated by Trust X for IFX public key.
- [ ] Execute a few loops and observe the output.
- [ ] Turn on the "Replay Attack" macro and recompile and flash the image.
- [ ] Replace the signature using your own replay attack.</br>
*Hint:* Use different print function to ease the cut-and-paste.

**Questions & Discussion:**

- What is the significance of Trust X generating the signature?
- Why does the verification passes even on different signature?
- What is the ECC signature? What are the components of ECC signature?
- Is it possible to prevent such replay attack?

## Task 6: Simplified Server Authentication
*Note:* In this task, only OpenSSL is used.

  Digital certificate is most commonly used to verify a identity in a Public Key Infrastructure (PKI). Certificate enables public key cryptography to address the problem of impersonation.

  Certificate Authority issues the certificate which binds a public key to it. Certificate helps to prevents fake public keys for impersonation. Only public key that is certified by the CA will work with the corresponding private key that is owned by the certificate owner. In this task, we are creating a self-signed certificate signed by a private entity rather than a trusted certificate authority.

  **Configuration file openssl.cnf for OpenSSL CA**<br/>
  https://github.com/gilatoes/arduino-optiga-trust-x/blob/master/missions/mission_files/openssl.cnf

### Create a Certificate Authority (CA)
- [ ]  Create a pair of ECC 384 Public Key and secret key.
- [ ]  Create a CA CSR with the Public Key. The CSR is signed using the CA secret key.
- [ ]  Create a CA root certificate.
- [ ]  Display the CA root certificate and store the certificate in all clients.

  ```
  mkdir certs ; mkdir crl ; mkdir newcerts ; mkdir private
  fsutil file createnew index.txt 0
  fsutil file createnew index.txt.attr 0
  fsutil file createnew serial 0
  echo "1000" >serial

  #Create a Bootcamp public secret key pair
  openssl ecparam -genkey -name secp384r1 -out private\ca.secretkey.pem

  #Create a CA CSR and CA cert
  openssl req -config openssl.cnf -key private\ca.secretkey.pem -new -x509 -days 7300 -sha256 -extensions v3_ca -out certs\ca.cert.pem -subj "/C=SG/ST=Singapore/L=Singapore/O=Infineon Technologies/OU=DSS/CN=TrustXBootCampCA"

  #Display the RootCA certificate
  openssl x509 -noout -text -in certs\ca.cert.pem
  ```

### Server Preparation
- [ ]  Create ECC 256 Public Key and secret key.
- [ ]  Create a Server CSR signed using the Server secret key.
- [ ]  Request a Server certificate using the CSR.

  ```
  #Create server public and secret keypair
  openssl ecparam -genkey -name prime256v1 -out private\server.secretkey.pem

  #Create CSR using server private key
  openssl req -new -key private\server.secretkey.pem -out server.csr.pem -subj "/C=SG/ST=Singapore/L=Singapore/O=Infineon Technologies/OU=DSS/CN=Server"

  #Create a PEM (Base64 encoded ASCII) format server certificate signed by CA
  openssl x509 -req -in server.csr.pem -CA certs\ca.cert.pem -CAkey private\ca.secretkey.pem -CAcreateserial -out server.cert.pem

  #Display the server certificate
  openssl x509 -noout -text -in server.cert.pem
  ```
### Server Sending information
- [ ] Server wants to send a message.
- [ ] Hash the message.
- [ ] Sign the message with the server secret key.
- [ ] Server will provides the server certificate, message and signature to client.
*Important*: Use Windows Powershell to execute the commands below. Alternatively, you can to manually create the message file.

```
#Message=OPTIGA Trust X BootCamp
"OPTIGA Trust X BootCamp" | Out-File -filePath .\message.txt -Encoding ASCII -NoNewline

#hash a message in file
openssl dgst -sha256 message.txt

#****************************************
#Data:			OPTIGA Trust X BootCamp
#Hash type:		SHA-256
#----------------------------------------
#Hash:			B090013953CBB65A3999110F31C249DD7A1C35D259A99EF785813B846911C593

#sign the message using signature
openssl dgst -sha256 -sign private\server.secretkey.pem -out signature.der message.txt

#show signature
openssl asn1parse -inform der -in signature.der
```

### Client receiving the information
  - [ ]  Client makes sure that CA certificate is provisioned.
  - [ ]  Client verifies the Server certificate using the root CA certificate.
  - [ ]  Client extract the public key from the server certificate.
  - [ ]  Client hash the message and checks the hash.
  - [ ]  Client verifies the message using the signature, server public key and message hash.

  ```
  #Verify server identity using Root CA
  #4 items are required.
  #1. RootCA
  #2. Server certificate
  #2. Message
  #3. Server signature

  #Get server certificate
  openssl verify -CAfile certs\ca.cert.pem server.cert.pem

  #hash a message in file
  openssl dgst -sha256 message.txt

  #Extract server public key from the RootCA
  openssl x509 -in .\server.cert.pem -pubkey -noout | Out-File -filePath .\publickey.pem -Encoding ASCII

  #Verify the signature using server public key
  openssl dgst -sha256 -verify publickey.pem -signature signature.der message.txt

  ```

**Questions & Discussion:**

  - [ ]  Create a new message "I want to go home early!". Sign the message and Client verify.

## Task 7: Unique authentication and optimization
One-way authentication is a process where an entity is trying to identify the true identity of the communicating party. Prior to the authentication process, both parties should have already established some common understanding. For example, they will need to agree upon common ECC parameters, type of hash process and message format.

In this task, there are 2 parts 7A and 7B.

#### Part A
Study E08_OneWay_Auth example carefully. This example works on any Trust X. Try switching Trust X with your fellow camp mate. In this part of the exercise, implement *asymmetric cryptography* your Trust X works uniquely on your Exerciser.

### Condition:
  - [ ] Personalize your Exerciser such that it will only works uniquely with a single Trust X.
  - [ ] Attaching other Trust X device should fail on your Exerciser.

  *Hint:* There is something to do with the Public Key. :-)

#### Part B
Implements the One-way authentication with the least amount of time.

### Memory footprints
  - [ ] Compare hardware and software verification approach.
  - [ ] Compete with your camp mates for the least Exerciser memory foot print. Check the compiled memory footprint, least memory usage wins!
  - [ ] Compete the speed of the 1-way Authentication. Least time wins!

# Mission: Simplified Firmware Update
Study the example of E10_PseudoTLS where the ECDH operation occurs with a single Trust X.
- Get a partner for this mission. For this mission, implements a simplified Secure Firmware update of Exerciser using 2 Trust X(s) and OpenSSL.
One of the Trust X operates the role of firmware update server and the other will be firmware update client.
The server will prepare the firmware package for the client to be updated.

### Firmware Update Server
The firmware update image has been prepared for the server.<br/>
**Firmware Image:**<br/>
https://github.com/gilatoes/arduino-optiga-trust-x/blob/master/missions/mission_files/XMC2Go_FWUpdate_2.hex

- [ ] Generate a new key pair.
 *Hint:* Refer to E07_SignVerify to get the public key.
- [ ] Exchange the newly generated public key.
- [ ] Use H04_CheckValidPublicKey example to ensure that the public key is valid.
- [ ] Both parties calculate the shared secrets and export derived key.
- [ ] Display the share secrets to proof that ECDH is successful using 2 Trust X(s).
- [ ] Generate the AES Key from the derived key and display the key.
- [ ] Select an Initialization vector and use AES Key to encrypt the firmware.<br/>
  *Note:* In block cipher mode operation such as AES, IV is used to initialize the block operation. IV can be a random and unpredictable value. However, it is not necessary to maintain its secrecy.
  For more secure communication, the same firmware can be encrypted with different IVs. This will yield completely different ciphertext.
  For simplicity, use the following IV in this exercise.
  iv=61A813408638CCCD67DA288B4142BF10

```
openssl enc -nosalt -aes-256-cbc -in XMC2Go_FWUpdate_2.hex -out XMC2Go_FWUpdate_2.hex.enc -base64 -K <key> -iv 61A813408638CCCD67DA288B4142BF10
```
- [ ] Hash the encrypted firmware.
- [ ] Optionally, include the signing process.
```
openssl dgst -sha256 XMC2Go_FWUpdate_2.hex.enc
```
- [ ] Send the hash and encrypted firmware to the receiver.

### Firmware update client
- [ ] Receive the Server Public Key.
- [ ] Establish shared secrets and derived key.
- [ ] Display the share secrets to proof that ECDH is successful.
- [ ] Generate the AES Key from the derived key.
- [ ] Receive the Initialization vector, XMC2Go_FWUpdate_2.hex.enc and hash.
- [ ] Hash the encrypted firmware and check the hash matches the server sent hash.
```
openssl dgst -sha256 XMC2Go_FWUpdate_2.hex.enc
```
- [ ] Use AES Key to decrypt the firmware.
```
openssl enc -d -nosalt -aes-256-cbc -in XMC2Go_FWUpdate_2.hex.enc -out XMC2Go_FWUpdate_2.hex -base64 -K <key> -iv 61A813408638CCCD67DA288B4142BF10
```

- [ ] Perform firmware update on Exerciser. (Use JFlash in this step)
- [ ] Use a terminal program to see what does the updated firmware do.

___
 **That's all folks!**
