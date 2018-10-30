# Mission Tasks:
## Task 0: Minimal
- [ ]  Setup with software

## Task 1: helloBootCamp
- [ ]  Execute the helloBootcamp example and checks the XMC and Trust X library version.

## Task 2: I2C and GPIO Debug and Analysis Exercise
- [ ]  Changes the Trust X library.
- [ ]  Configure GPIO to enable warm reset.
- [ ]  Modify the I2C bus frequency. Monitor the update on analyzer.
- [ ]  Modify the I2C address of Trust X
## Task 3: Trust X Object IDs
- [ ] Read the UID Object.
  * Decode the UID to determine the hardware token identity.
- [ ] Program the following data into the "Arbitrary data object Type 1" 0xF1D0.
```hash
  f8 79 f9 36 cd d6 d6 e4 f1 3e c3 3f 18 71 1b a1 ed a0 eb 8a de ba 15 13 75 d0 af 75 39 39 9b ff
```
- [ ] Read Meta data of the Device Public Key certificate issued by Infineon data object.
- [ ] Determine the Public key issued by Infineon PKI from your token.

## Task 4: Concepts Digital Signature using ECC Asymmetric keys
Digital signature is a mathematical scheme for presenting the authenticity of digital messages or documents.
A valid digital signature gives a recipient reason to believe that the message was created by a known sender
(authentication), that the sender cannot deny having sent the message (non-repudiation), and that the message
was not altered in transit (integrity).

Find a partner and role play Alice (She has dual roles) and Bob in this task.

Alice wants to sends Bob a Private Message (PM). "I am joining Infineon Trust X Boot Camp."
Bob received the PM and wonders about its integrity.

Note: In this Mission, only OpenSSL is used.

**Alice message**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/task4/Alice-Message.txt

  - [ ] Alice generates a set of ECC keypair using ECC NIST P384 curve.
  ```KeyGen
  openssl ecparam -name secp384r1 -genkey -noout -out private.key.pem

  openssl ec -in private.key.pem -outform PEM -pubout -out public.key.pem
  ```
  - [ ] Alice signs the message using her private key.
  - [ ] Keeping her secret key private, Alice will email/transmit her public key, signature, and message hash to Bob.
  - [ ] Bob verifies the message using the received package.
  - [ ] Eve (Evil twin of Alice) hacked into Alice email account and resend a new message to Bob.
        Without Alice's Secret Key, Eve just send a new message "I am not joining Infineon Trust X Boot Camp."
  - [ ] Bob is confused when he received the second message. He verifies the new message again using the previous signature and public key.
  - [ ] If time permits, Alice and Bob should switch roles.
  - [ ] This approach seems to work? What is the weakness using this approach?
  - [ ] After verifying the message, can Bob really trust its contents?

## Task 5: Accessories Authentication
* Using One-Way Authentication example as a starting point
- In the scenario, implement your own Asymmetric Key 1-way Authentication.
### Condition:
  - [ ] Your XMC2Go will only work with your own Trust X.
  - [ ] Attaching other Trust X should fail on your board.

## Memory footprints
  - [ ] Compare hardware and software verification.
  - [ ] Compete with your camp mates for the least XMC2Go memory foot print. Least memory wins!
  - [ ] Compete the speed of the 1-way Authentication. Least time wins!


## Task 6: Simple Firmware Update
* Using One-Way Authentication example as a starting point...

### Server
- [ ] Use Infineon Public Key Pair.
- [ ] Read the firmware hash value from Task 1 from "Arbitrary data object Type 1" 0xF1D0.
- [ ] Calculate the signature.
- [ ] Verify the signature.
- [ ] Send the 3 items to another group or fellow boot camp mates via secondary channel. (Email, writing, IM or etc)
- [ ] Trust X token, Firmware and Hash.

### Receiver
- [ ] Receive the Trust X token, Firmware and Hash from another group.
- [ ] Verify the Signature.
- [ ] Perform firmware update.

**Firmware Image:**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/XMC2Go_FWUpdate_1.hex

**Firmware SHA-256 Checksum:**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/XMC2Go_FWUpdate_1_Checksum.txt

## Task 7: Certificate Create and Verify
Digital certificate is most commonly used to verify a identity in a Public Key Infrastructure (PKI). Certificates use public key cryptography to address the problem of impersonation.

Certificate Authority issues the certificate which binds a public key to it. Certificate helps to prevents fake public keys for impersonation. Only public key that is certified by the CA will work with the corresponding private key that is owned by the certificate owner.

**Configuration file openssl.cnf for OpenSSL CA**<br/>
https://raw.githubusercontent.com/gilatoes/arduino-optiga-trust-x/master/Missions/Mission_files/openssl.cnf

- [ ]  Create Boot Camp Certificate Authority.

## Certificate Authority
* Using generateKeypair example as a starting point
- [ ]  Create and export Public Key and private key.
- [ ]  Format the Public key and Secret key to PEM format.
- [ ]  Create a CSR with the Public Key.
- [ ]  Sign the CSR with a self-sign CA. (On Raspberry Pi)
- [ ]  Create the device certificate.
- [ ]  Send device certificate and Boot Camp Root CA to Consumer
- [ ]  Store the Root CA Public Key into 0xE0EF

### Consumer
- [ ]  Verify the device CA against the root CA
