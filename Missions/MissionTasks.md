# Tasks and Mission:
## Task 0: Setup Boot Camp Environment
- [ ]  Download and setup all the required software and tools.<br/>
 - Follow XMC Arduino Github for instruction to install XMC & Arduino IDE.<br/>
https://github.com/Infineon/XMC-for-Arduino<br/>
 - XMC Library version 1.2.1 (Github)<br/>
https://github.com/Infineon/XMC-for-Arduino<br/>
 - Arduino IDE version 1.8.8<br/>
https://www.arduino.cc/en/main/software<br/>
 - SEGGER J-Link (J-Link Software and Documentation pack for Windows)<br/>
https://www.segger.com/downloads/jlink<br/>
 - OpenSSL for Windows v1.1.0j<br/>
https://slproweb.com/products/Win32OpenSSL.html<br/>
 - Saleae Logic Analyzer<br/>
https://www.saleae.com/downloads/<br/>
 - Source editor e.g Eclipse (Optional)<br/>
https://www.eclipse.org/downloads/<br/>
 - Terminal Software e.g MobaXterm(Free edition), Tera Term or Putty (Optional)<br/>
https://mobaxterm.mobatek.net/download.html<br/>
 - Total Phase Beagle Software and driver (Optional)<br/>
https://www.totalphase.com/products/data-center/<br/>
 - BP Tools (Optional)<br/>
https://www.eftlab.co.uk/index.php/downloads/bp-tools<br/>
 - Decompression tool e.g Zip-7 (Optional)<br/>
 - PDF reader e.g Adobe PDF reader (Optional)<br/>

- [ ]  After installing the Boot Camp Trust X repo, start E00_Minimal example and ensure that it can be compiled without any error.
- [ ]  Open the command prompt. On Windows 10, it is call Windows Powershell. For a more advance interface, open ise from powershell.
- [ ]  Ensure that openssl is installed and can be accessed from the powershell. Use the "openssl version" to check the path accessibility.

## Task 1: HelloBootCamp
- [ ]  Attach the Trust X to your platform and execute the E01_HelloBootcamp example.
- [ ]  What is your XMC and Trust X library version?
- [ ]  Find out which file stores the Trust X library version number?

## Task 2: GPIO and I2C Debug and Analysis Exercise
In this task, there will be some changes required for your Trust X library. You are expected to take reference from Trust X Datasheet, read up I2C fundamentals and connect up Logic analyzer and/or I2C analyzer for this task. Use E02_ConfigI2C sketch example as your starting point.
- [ ]  Refer to Trust X datasheet V2.5 section 5.1.6.2
- [ ]  Monitor and enable warm reset using GPIO (P0.0).
Trust X I2C understanding:
- [ ]  What is the default Trust X I2C address?
- [ ]  What is the current I2C bus speed?
- [ ]  Modify and increase the I2C bus frequency. Monitor the update using either an I2C analyzer, logic analyzer or oscilloscope.
- [ ]  Modify the I2C address of Trust X to 0x40.

## Task 3: Trust X Object IDs
This task consists of 2 parts. First, read and decode the Trust X UID. Second, using your issued Trust X, determine the Public Key issued by Infineon PKI.
- [ ] Read the UID Object.
  * Decode the UID to determine the Trust X identity.
- [ ] Determine the Certificate serial number, Issue date and Public key issued by Infineon PKI from your Trust X.
*Hint:* Use E05_GetCertificate example as a starting point. You will need to convert the Hex value to Base64 value then decode it using certificate decoder tools.

## Task 4: Concepts of Digital Signature using ECC Asymmetric keys
*Note:* In this task, only OpenSSL is used.

Digital signature is a mathematical scheme for presenting the authenticity of digital messages or documents.
A valid digital signature gives a recipient reason to believe that the message was created by a known sender
(authentication), that the sender cannot deny having sent the message (non-repudiation), and that the message
was not altered in transit (integrity).

This task can be completed alone. However, it will be more fun to get a partner role play Bob (He could played dual roles) and Alice in this task.

Bob wants to sends Alice a Private Message (PM). "I am joining Infineon Trust X Boot Camp."
Alice received the PM and wonders about its integrity.

**Alice message**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/task4/Alice-Message.txt

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
  - [ ] If time permits, Alice and Bob should switch roles.
  - [ ] Discussion: If we purely use the public key and secret key to perform verification of data message what is the potential problem? What is the potential weakness using such approach?
  - [ ] After verifying the message, can Bob really trust its contents?

## Task 5: Let's look at replay attack

Study the E07_SignVerify sketch example carefully.
- [ ] Print out (using output_result()) the signature generated by Trust X for IFX public key.
- [ ] Execute a few loops and observe the output.
- [ ] Turn on the "Replay Attack" macro and recompile and flash the image.
- [ ] Replace the signature using your own replay attack.</br>

**Discussion:**

- What is the significance of Trust X generating the signature?
- Why does the verification passes even on different signature?
- What is the ECC signature? What are the components of ECC signature?
- Is it possible to prevent such replay attack?

## Task 6: Simplified Server Authentication
*Note:* In this task, only OpenSSL is used.

  Digital certificate is most commonly used to verify a identity in a Public Key Infrastructure (PKI). Certificate enables public key cryptography to address the problem of impersonation.

  Certificate Authority issues the certificate which binds a public key to it. Certificate helps to prevents fake public keys for impersonation. Only public key that is certified by the CA will work with the corresponding private key that is owned by the certificate owner.

  **Configuration file openssl.cnf for OpenSSL CA**<br/>
  https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/openssl.cnf

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

## Task 7: A very special accessory authentication use case
One-way authentication is a process where an entity is trying to identify the true identity of the communicating party. Prior to the authentication process, both parties had already established some common understanding. For example, they will need to agree upon common ECC parameters, type of hash process and message format.

In this task, there are 2 parts 7A and 7B.

#### Part A
Study E08_OneWay_Auth example carefully. This example works on any Trust X. Try switching Trust X with your fellow camp mate. In this part of the exercise, implement *asymmetric cryptography* such that only your Trust X can work on your board or host code.

### Condition:
  - [ ] Your XMC2Go will only work with your own Trust X.
  - [ ] Attaching other Trust X device should fail on your board.

  *Hint:* There is something to do with the Public Key. :-)

#### Part B
Implements the One-way authentication with the least amount of time.

### Memory footprints
  - [ ] Compare hardware and software verification approach.
  - [ ] Compete with your camp mates for the least XMC2Go memory foot print. Check the compiled memory footprint, least memory usage wins!
  - [ ] Compete the speed of the 1-way Authentication. Least time wins!

# Mission: Simplified Firmware Update
Study the example of E10_PseudoTLS where the ECDH operation occurs with a single Trust X.
- Find a partner for this mission. In this mission, implements Secure Firmware update of XMC2Go using 2 Trust X(s) and OpenSSL.
One of the Trust X operates the role of firmware update server and the other will be firmware update client.
The server will prepare the firmware package for the client to be updated.

### Firmware Update Server
The firmware update image has been prepared for the server.<br/>
**Firmware Image:**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/XMC2Go_FWUpdate_2.hex

- [ ] Generate a new key pair.
 *Hint:* Refer to E07_SignVerify to get the public key.
- [ ] Exchange the newly generated public key.
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

- [ ] Perform firmware update on XMC2Go. (Use JFlash in this step)
- [ ] Use a terminal program to see what does the updated firmware do.

___
 **That's all folks!**
