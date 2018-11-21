/**
 * MIT License
 *
 * Copyright (c) 2018 Infineon Technologies AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 *
 * Demonstrates use of the
 * Infineon Technologies AG OPTIGA™ Trust X Arduino library
 */

#include "OPTIGATrustX.h"
#include "debug.h"

//Max length of DER Encoded ECDSA signature using 256-bit EC Key
#define SIGN_MAX_LENGTH		72
#define MESSAGE_LENGTH    16
//SHA-256 Hash length
const uint16_t HASH_LENGTH = 32;
//IFX TLV + DER Octet tag uncompressed public key format
uint16_t PUBKEY_LENGTH = 68;

uint8_t *hash = new uint8_t[HASH_LENGTH];
uint8_t *Signature = new uint8_t[SIGN_MAX_LENGTH];

#define ASSERT(ret)   if(ret){debug_print("\r\nCheck:%d: %s\r\n", __LINE__, __func__);return 0;}

//#define REPLAY_ATTACK

uint8_t sys_init =0;
void setup()
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
  delay(100);

  if(reset()==1){
    sys_init=1;
  }else{
    sys_init=0;
  }
}

uint8_t calculateSignVerifySign_IFXkey()
{
  uint16_t ret = 0;
  uint8_t  Message[MESSAGE_LENGTH] = {'T', 'R', 'U', 'S', 'T', ' ', 'X', ' ', 'B', 'O', 'O', 'T', 'C', 'A', 'M', 'P'};
  uint8_t  ifxPublicKey[PUBKEY_LENGTH];

  uint16_t Signature_Len =0; //The length of the signature will be returned from signature function

  /*
   * Extract public key of the device certificate
   */
  Serial.println("IFX Public Key:");
  ret = trustX.getPublicKey(ifxPublicKey);
  output_result(ret, ifxPublicKey, PUBKEY_LENGTH);
  ASSERT(ret);

  /*
   * Calculate a hash of the given Message
   */
  Serial.println("Hash:");
  ret = trustX.sha256(Message, MESSAGE_LENGTH, hash);
  output_result(ret, hash, HASH_LENGTH);
  ASSERT(ret);

  /*
   * Sign hash
   */
  Serial.println("Signature:");
  ret = trustX.calculateSignature(hash, HASH_LENGTH, Signature, Signature_Len);
  debug_print("Signature length: %d", Signature_Len);
  output_result(ret, Signature, Signature_Len);
  ASSERT(ret);

  /*
   * Verify a signature
   */
  Serial.println("Verify Signature:");
  ret = trustX.verifySignature(hash, HASH_LENGTH, Signature , Signature_Len, ifxPublicKey, PUBKEY_LENGTH);
  if(ret==0)
  {Serial.println("+++++++++++++ Verify Signature successful +++++++++++++");}
  else{
    Serial.println("------------- Verify Signature failed -------------");
  }
#ifdef REPLAY_ATTACK
    //This is an example signature.
    //Execute the sign operation and generate a set of signature.
    //Replace the generated signature in this array.
    uint8_t replay_signature[] = {0x02, 0x21, 0x00, 0xbc, 0x3b, 0x5b, 0xc0, 0x55, 0x0e, 0x1e, 0x63, 0x78, 0x3f, 0xcb, 0x3a, 0xe4,
                                  0x6f, 0xd0, 0x71, 0x93, 0x3e, 0xf1, 0x5c, 0x75, 0x74, 0xab, 0x7c, 0xc1, 0x71, 0x7a, 0x6a, 0x46,
                                  0x18, 0x2f, 0x7a, 0x02, 0x20, 0x6a, 0x37, 0x22, 0xb6, 0x5e, 0x75, 0xc2, 0x96, 0x5b, 0xfc, 0x6c,
                                  0xdc, 0x47, 0x0a, 0xfe, 0x73, 0xc2, 0xc9, 0x52, 0x96, 0xdf, 0x10, 0x8d, 0xca, 0xed, 0x2e, 0x57,
                                  0xe4, 0xdb, 0x78, 0x25, 0xc8
                                 };
    uint16_t replay_signature_Length=69;

    ret = trustX.verifySignature(hash, HASH_LENGTH, replay_signature, replay_signature_Length, ifxPublicKey, PUBKEY_LENGTH);
    if(ret==0)
    {Serial.println("+++++++++++++ Verify Replay Signature successful +++++++++++++");}
    else{
      Serial.println("------------- Verify Replay Signature failed -------------");
      return 0;
      }
#endif
  return 1;
}

uint8_t calculateSignVerifySign_Newkey()
{
  uint32_t ret = 0;
  uint8_t  Message[MESSAGE_LENGTH] = {'T', 'R', 'U', 'S', 'T', ' ', 'X', ' ', 'B', 'O', 'O', 'T', 'C', 'A', 'M', 'P'};
  uint8_t *PublicKey = new uint8_t[PUBKEY_LENGTH];

  uint16_t Signature_Len = 0;

  /*
   * Generate a key pair and store private key inside the security chip
   */
  Serial.println("New Public Key:");
  ret = trustX.generateKeypair(PublicKey, PUBKEY_LENGTH, eSESSION_ID_2);
  output_result(ret, PublicKey, PUBKEY_LENGTH);
  ASSERT(ret);

  /*
   * Calculate a hash of the given Message
   */
  Serial.println("Hash:");
  ret = trustX.sha256(Message, MESSAGE_LENGTH, hash);
  output_result(ret, hash, HASH_LENGTH);
  ASSERT(ret);

  /*
   * Sign hash with the newly generated private key
   */
  Serial.println("Signature:");
  ret = trustX.calculateSignature(hash, HASH_LENGTH, eSESSION_ID_2, Signature, Signature_Len);
  debug_print("Signature length: %d", Signature_Len);
  output_result(ret, Signature, Signature_Len);
  ASSERT(ret);

  /*
   * Verify a signature
   */
  Serial.println("Verify Signature:");
  ret = trustX.verifySignature(hash, HASH_LENGTH, Signature, Signature_Len, PublicKey, PUBKEY_LENGTH);
  if(ret){
    Serial.println("------------- Verify Signature failed -------------");
    return 0;
  }else{
   Serial.println("+++++++++++++ Verify Signature successful +++++++++++++");
   return 1;
   }
}

void loop()
{
uint8_t ret=0;

  if(sys_init)
  {
    Serial.println("Sign Message and verify a signature using IFX public key");
    ret = calculateSignVerifySign_IFXkey();
    if(ret==0){
      Serial.println("Sign and Verify IFX key failed");
    }

    Serial.println("Sign Message and verify a signature using a new generated public key");
    ret = calculateSignVerifySign_Newkey();
    if(ret==0){
       Serial.println("Sign and Verify new key failed");
    }
  }

  Serial.println("\r\nPress i to re-initialize.. other key to loop...");
  while (Serial.available()==0){} //Wait for user input
  String input = Serial.readString();  //Reading the Input string from Serial port.
  input.trim();
  if(input=="i")
  {
    if(reset()==0)
    {
      //Do not execute
      sys_init=0;
      //close the connection
      trustX.end();
    }else
    {
      sys_init=1;
      }
  }

}

uint8_t reset()
{
  uint32_t ret = 0;
  Serial.println("Initialize Trust X");
  ret = trustX.begin();
  ASSERT(ret);

   /*
   * Speedup the board (from 6 mA to 15 mA)
   */
  Serial.println("Limiting Current consumption (15mA - means no limitation)");
  ret = trustX.setCurrentLimit(15);
  ASSERT(ret);

  return 1;
}
