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
#include "third_crypto/uECC.h"

#define MAXCMD_LEN    255
#define CERT_LENGTH   512
#define RND_LENGTH    16
#define HASH_LENGTH   32
#define SIGN_LENGTH   80
#define PUBKEY_LENGTH 70

#define SUPPRESSCOLLORS
#include "fprint.h"

#define ASSERT(err)   if (ret) { printlnRed("Failed"); while (true); }
#define SOFTWARE_VERIFY      1
/*
 * Allocating buffers for further use in loop()
 */
uint8_t *cert = new uint8_t[CERT_LENGTH];
uint16_t certLen = CERT_LENGTH;
uint8_t *rnd = new uint8_t[RND_LENGTH];
uint16_t rndLen = RND_LENGTH;
uint8_t *hash = new uint8_t[HASH_LENGTH];
uint16_t hashLen = HASH_LENGTH;
uint8_t *rawSign = new uint8_t[SIGN_LENGTH];
uint8_t *formSign = new uint8_t[SIGN_LENGTH];
uint16_t signLen = SIGN_LENGTH;
uint8_t *pubKey = new uint8_t[PUBKEY_LENGTH];
uint16_t pubKeyLen = PUBKEY_LENGTH;

#define SIGNATURE_LENGTH  64
#define TLV_PADDING    4
#define TLV_OFFSET     4
  
uint8_t sys_init =0;
void setup() 
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(38400);
  delay(1000);
  Serial.println("Initializing ... ");

 /*
   * Initialise an OPTIGA™ Trust X Board
   */
  if(reset()==0){
    sys_init=1;
  }else{
    sys_init=0;
  }
#if( UC_FAMILY == XMC1 )
  led1On();
  led2On();
#endif  
}


static void output_result(char* tag, uint8_t* in, uint16_t in_len)
{
  printlnGreen("OK"); 
  printMagenta(tag); 
  printMagenta(" Length: ");
  Serial.println(in_len);
  printMagenta(tag); 
  printlnMagenta(":");
  HEXDUMP(in, in_len);
}

void loop() 
{
  uint32_t ret = 0;
  uint8_t ifxPublicKey[64+TLV_PADDING];

  if(sys_init)
  { 
    /* 
     * Get random value of RND_LENGTH length
     */
    Serial.println("Trust X generate random number...");
    ret = trustX.getRandom(RND_LENGTH, rnd);
    ASSERT(ret);
    output_result("Random Message", rnd, RND_LENGTH);
  
    /* 
     * Calculate SHA256 value
     */
    Serial.println("Calculate Hash on the message...");
    ret = trustX.sha256(rnd, RND_LENGTH, hash);
    hashLen = 32;
    ASSERT(ret);
    output_result("SHA256", hash, hashLen);
  
    /* 
     * Generate a signature NIST-P256 on the message
     */
    Serial.println("Calculate signature from Trust X...");
    ret = trustX.calculateSignature(hash, hashLen, formSign, signLen);
    ASSERT(ret);
    output_result("Signature", formSign, signLen);
  
    /* 
     * Use the Public key to check against the Signature
     */
    trustX.getPublicKey(ifxPublicKey);    
    output_result("Public Key", ifxPublicKey, 64 + TLV_PADDING);

   //Drop off public key TLV
   ret = uECC_valid_public_key(ifxPublicKey + TLV_OFFSET, uECC_secp256r1());
   if(ret==1)
   {Serial.println("Valid Public key.");}
   else
   {Serial.println("Error: Invalid Public key.");}
   
#if SOFTWARE_VERIFY      
    //ECC signature pair (r,s) is encoded as two DER "Integer"   
    Serial.println("Processing Signature...");
    uint8_t signature[SIGNATURE_LENGTH];

    if (formSign[1] == 0x21)
    {
      memcpy(signature, formSign+3, 32);
      
      if (formSign[32 + 4] == 0x21) 
      {
        memcpy(signature+32, formSign+ 32 + 6, 32);
      } 
      else 
      {
        memcpy(signature+32, formSign +32 + 5, 32);
      }
    }
    else
    {
      memcpy(signature, formSign+2, 32);
      
      if (formSign[32+3] == 0x21) 
      {
        memcpy(signature+32, formSign+32 + 5, 32);
      } 
      else 
      {
        memcpy(signature+32, formSign+32 + 4, 32);
      }
    }
    
    output_result("Signature", signature, SIGNATURE_LENGTH);    
    ret = uECC_verify(ifxPublicKey+TLV_OFFSET,
                      hash,
                      hashLen,
                      signature,
                      uECC_secp256r1());
    if(ret==1){
    Serial.println("\r\nPassed Verification");
    }
    else if(ret==0){
     Serial.println("\r\nFailed Verifrication");
    }else{
      Serial.println("\r\nInvalid result");
      }
#else      
    printGreen("Verify Signature using Trust X");
    ret = trustX.verifySignature(hash, hashLen, formSign, signLen, ifxPublicKey,
    sizeof(ifxPublicKey) / sizeof(ifxPublicKey[0]));
    
    if(ret==0){
    Serial.println("\r\nPassed Verification");
    }
    else 
    {
      Serial.print("\r\nInvalid result, Error code: ");
      Serial.println(ret, HEX);
      }
#endif

  }

  printlnGreen("\r\nPress i to re-initialize.. other key to loop...");   
  while (Serial.available()==0){} //Wait for user input  
  String input = Serial.readString();  //Reading the Input string from Serial port.
  input.trim();
  if(input=="i") 
  {
    if(reset()!=0)
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
  printGreen("Begin to trust ... ");
  ret = trustX.begin();
  if (ret) {
    printlnRed("Failed");
    return -1;   
  }
  printlnGreen("OK");
  
   /*
   * Speedup the board (from 6 mA to 15 mA)
   */
  printGreen("Limiting Current consumption (15mA - means no limitation) ... ");
  ret = trustX.setCurrentLimit(15);
  if (ret) {
    printlnRed("Failed");
    return -1;
  }
  printlnGreen("OK");

#if( UC_FAMILY == XMC1 )
  led1Off();
  led2Off();
#endif
  return 0;
}

