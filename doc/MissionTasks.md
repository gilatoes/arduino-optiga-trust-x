# Mission Tasks:
## Task 1: Trust X Object IDs
- [ ] Read the UID Object.
  * Decode the UID to determine the hardware token identity.
- [ ] Program the following data into the "Arbitrary data object Type 1" 0xF1D0.
```hash
  f8 79 f9 36 cd d6 d6 e4 f1 3e c3 3f 18 71 1b a1 ed a0 eb 8a de ba 15 13 75 d0 af 75 39 39 9b ff
```
- [ ] Read Meta data of the Device Public Key certificate issued by Infineon data object.
- [ ] Determine the Public key issued by Infineon PKI from your token.

## Task 2: Accessories Authentication
* Using One-Way Authentication example as a starting point
- In the scenario, implement your own Asymmetric Key 1-way Authentication.
### Condition:
  - [ ] Your XMC2Go will only work with your own Trust X.
  - [ ] Attaching other Trust X should fail on your board.

## Memory footprints
  - [ ] Compare hardware and software verification.
  - [ ] Compete with your camp mates for the least XMC2Go memory foot print. Least memory wins!
  - [ ] Compete the speed of the 1-way Authentication. Least time wins!

## Task 3: Firmware Update
* Using One-Way Authentication example as a starting point.

Firmware Image<br/>
https://drive.google.com/file/d/1LNF6uueKFEQ-COgkzWL7grB5JSRCuRxY/view?usp=sharing
AWS S3 link
https://s3-us-west-2.amazonaws.com/trustxbootcamp/XMC2Go_FWUpdate_1.hex

Firmware SHA-256 Checksum
https://drive.google.com/file/d/1hqXIJn_757Nmou5aTbD__R2GEoqOUQpn/view?usp=sharing
AWS S3 link
https://s3-us-west-2.amazonaws.com/trustxbootcamp/XMC2Go_FWUpdate_1_Checksum.txt

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

## Task 4: Certificate Create and Verify
Digital certificate is most commonly used to verify a identity in a Public Key Infrastructure (PKI). Certificates use public key cryptography to address the problem of impersonation.

Certificate Authority issues the certificate which binds a public key to it. Certificate helps to prevents fake public keys for impersonation. Only public key that is certified by the CA will work with the corresponding private key that is owned by the certificate owner.

### Setup for OpenSSL
Download Windows OpenSSL x64
https://drive.google.com/file/d/1yDZC_0CJx1NVSi9sECObcaDUTMwQP7tg/view?usp=sharing

Configuration file openssl.cnf for OpenSSL CA
https://drive.google.com/file/d/1__tOEmDe2zYQSM0RnPZWcExGYKfS0Pef/view?usp=sharing

- [ ]  Create Boot Camp Certificate Authority.

## Producer (Secure environment on the Raspberry Pi)
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

## Task 5: I2C
- [ ]  Changes the Trust X library.
- [ ]  Modify the I2C address of Trust X
