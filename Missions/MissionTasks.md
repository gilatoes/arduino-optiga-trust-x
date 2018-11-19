# Mission Tasks:
## Task 0: Setup your Arduino environment for XMC and Trust X
- [ ]  Download and setup all the required software and make sure that E00_Minimal example can be executed without any error.

## Task 1: HelloBootCamp
- [ ]  Attach the Trust X to your platform and execute the E01_HelloBootcamp example.
- [ ]  What is your XMC and Trust X library version?

## Task 2: I2C and GPIO Debug and Analysis Exercise
In this task, there will be some changes required for your Trust X library. You are expected to take reference from Trust X Datasheet, read up I2C fundamentals and connect up I2C analyzer for this task.
- [ ]  Configure GPIO to enable warm reset.
- [ ]  Modify the I2C bus frequency. Monitor the update using either an I2C analyzer, logic analyzer or oscilloscope.
- [ ]  Modify the I2C address of Trust X to 0x40.

## Task 3: Trust X Object IDs
In this task, it consists of 2 parts. First, read and decode the Trust X UID. Second, using your issued Trust X, determine the Public Key issued by Infineon PKI.
- [ ] Read the UID Object.
  * Decode the UID to determine the hardware token identity.
- [ ] Determine the Public key issued by Infineon PKI from your Trust X.

## Task 4: Concepts of Digital Signature using ECC Asymmetric keys
**Note:** In this task, only OpenSSL is used.

Digital signature is a mathematical scheme for presenting the authenticity of digital messages or documents.
A valid digital signature gives a recipient reason to believe that the message was created by a known sender
(authentication), that the sender cannot deny having sent the message (non-repudiation), and that the message
was not altered in transit (integrity).

Find a partner and role play Alice (She has dual roles) and Bob in this task.

Alice wants to sends Bob a Private Message (PM). "I am joining Infineon Trust X Boot Camp."
Bob received the PM and wonders about its integrity.

**Alice message**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/task4/Alice-Message.txt

  - [ ] Alice generates a set of ECC keypair using ECC NIST P384 curve.
[//]: # (This may be the most platform independent comment)
  ```
  openssl ecparam -name secp384r1 -genkey -noout -out private.key.pem

  openssl ec -in private.key.pem -outform PEM -pubout -out public.key.pem
  ```

  - [ ] Alice signs the message using her private key.
  - [ ] Keeping her secret key private, Alice will email/transmit her public key, signature, and message hash to Bob.
  - [ ] Bob verifies the message using the received package.

  The use of public verification process has successfully prevented Eve (Evil twin of Alice) from impersonating Alice from sending a "fake" message to Bob. Without Alice's Secret Key, all Eve's message will be rejected.
  - [ ] As a negative use case, Eve send a message "I am not joining Infineon Boot camp" to Bob and see if the verification works.
  - [ ] Bob received the second message. He verifies the new message again using the previous signature and public key.
  - [ ] If time permits, Alice and Bob should switch roles.
  - [ ] Discussion: This approach seems to work? What is the weakness using this approach?
  - [ ] After verifying the message, can Bob really trust its contents?

## Task 5: Certificate Create and Verify (Server verification)
  Digital certificate is most commonly used to verify a identity in a Public Key Infrastructure (PKI). Certificate enables public key cryptography to address the problem of impersonation.

  Certificate Authority issues the certificate which binds a public key to it. Certificate helps to prevents fake public keys for impersonation. Only public key that is certified by the CA will work with the corresponding private key that is owned by the certificate owner.

  **Configuration file openssl.cnf for OpenSSL CA**<br/>
  https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/openssl.cnf

  - [ ]  Create Boot Camp Certificate Authority.

  ### Server Certificate Authority
  - [ ]  Create and export Public Key and private key.
  - [ ]  Format the Public key and Secret key to PEM format.
  - [ ]  Create a CSR with the Public Key.
  - [ ]  Sign the CSR with a self-sign CA.
  - [ ]  Inject Root CA certificate to "remote" Trust X device
  - [ ]  Store the Root CA Certificate (a.k.a Trust Anchor) into 0xE0EF.

  ### Authenticate Server
  - [ ]  Server sign a message and provide the message to device.
  - [ ]  Verify the signature, message against the root CA.

## Task 6: A very special accessory authentication use case
A One-way authentication is a process where the an entity is trying to identify the true identity of the communicating party. Prior to the authentication process, both parties had already established some common understanding. For example, they will need to agree upon common ECC parameters, type of hash process and message format.

In this task, there are 2 parts. First, implement asymmetric cryptography (modify the One-Way Authentication example) such that only your Trust X can work on your board or host code. Second, implement the One-way authentication with the least time.

**Hint:** There is something to do with the Public Key. :-)

### Condition:
  - [ ] Your XMC2Go will only work with your own Trust X.
  - [ ] Attaching other Trust X should fail on your board.

### Memory footprints
  - [ ] Compare hardware and software verification approach.
  - [ ] Compete with your camp mates for the least XMC2Go memory foot print. Check the compiled memory footprint, least memory usage wins!
  - [ ] Compete the speed of the 1-way Authentication. Least time wins!

## Task 7: Let's look at replay attack
Study the SignVerify example carefully. Turn on the "Replay Attack" macro and check the status.
- [ ] Why does the verification passes when replay attack is switched on?
- [ ] How can such replay attack be prevented?

## Task 8: Simplify Firmware Update
Study the example of PseudoTLS. It is an example implemented within a single Trust X device.
- In this scenario, let us implement Secure Firmware update using 2 Trust X(s) and OpenSSL.

### Firmware Update Server
The firmware update image has been prepared for the server.<br/>
**Firmware Image:**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/XMC2Go_FWUpdate_2.hex

- [ ] Exchange Infineon issued Public Key Pair (0xE0E0) with Receiver.

- [ ] Establish shared secrets and derived key.
- [ ] Display the share secrets to proof that ECDH is successful.
- [ ] Generate the AES Key from the derived key and display the key.
- [ ] Select an Initialization vector and use AES Key to encrypt the firmware.
```
openssl enc -nosalt -aes-256-cbc -in XMC2Go_FWUpdate_2.hex -out XMC2Go_FWUpdate_2.hex.enc -base64 -K <key> -iv <iv>
```
- [ ] Hash the encrypted firmware.
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
openssl enc -d -nosalt -aes-256-cbc -in XMC2Go_FWUpdate_2.hex.enc -in XMC2Go_FWUpdate_2.hex -base64 -K <key> -iv <iv>
```

- [ ] Perform firmware update on XMC2Go. (Use JFlash in this step)
- [ ] Use a terminal program to see the output of the updated firmware.
