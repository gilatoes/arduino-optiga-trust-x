# Mission Tasks:
## Task 1: Object ID
Read the UID.
Decode the UID to determine the hardware identity.
Read the Infineon device Public key
Program a short message into the memory location 0xE0
Read Meta data of the data object

## Task 2: Accessories Authentication
* Using Example of One-Way Authentication
* Implement your own Asymmetric Key 1-way Authentication.
  - Only Your Trust X works with your board.
  - Other Trust X should fail on your board.
* Check the difference between Software vs Hardware Verification
  - Reduces the host memory foot print to the minimum
  - Increase the speed of the 1-way Authentication

## Task 3: Firmware Update
### Server
* Generate Public and Private Key Pair
* Private Key is stored in the key storage.
* Hash the Public Key and a Small Firmware.
* Send the 4 items via web download.
* Trust X token, Public Key, Key store address, Firmware and Hash.

### Receiver
* Check the Public Key, Firmware and Hash matches.
* Public Key + Message + Hash used to sign against Key store
* Verify Signature.
* Flash the firmware.

## Task 4: Secure token preventing man-in-the-middle attack
### Sender
* Write some information in NVM.
* Generate Public and Private Key Pair
* Private Key is stored in the key storage.
* Hash the Public Key and a Message.
* Send the 4 items via secondary channel.
* Trust X token, Public Key, Key store address, Message and Hash.

### Receiver
* Check the Public Key, Message and Hash matches.
* Public Key + Message + Hash used to sign against Key store
* Verify Signature.
* Read NVM contents.

## Task 5: Certificate Create and Verify
### Self-signed CA (Raspberry Pi)

* Producer (Secure environment)
* Create Public Key and private key. Export both.
* Format the Public key and Secret key to PEM format.
* Create a CSR with the Public Key.
* Sign the CSR with a self-sign CA. (located on AWS EC2)
* Create the device CA.
* Send device CA and root CA to Consumer
* Store the Root CA Public Key into 0xE0EF

### Consumer
* Verify the device CA against the root CA

## Task 6: I2C
* Changes the Trust X library.
* Modify the I2C address of Trust X
