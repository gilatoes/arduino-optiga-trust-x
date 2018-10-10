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

#define DATA_LENGTH		10
#define HASH_LENGTH		32
#define SIGN_LENGTH		80
#define PUBKEY_LENGTH	70

#define SUPPRESSCOLLORS
#include "fprint.h"

uint8_t *hash = new uint8_t[HASH_LENGTH];
uint8_t *rawSign = new uint8_t[SIGN_LENGTH];
uint8_t *formSign = new uint8_t[SIGN_LENGTH ];
uint8_t *pubKey = new uint8_t[PUBKEY_LENGTH];

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

static void output_result(char* tag, uint32_t tstamp, uint8_t* in, uint16_t in_len)
{
  printGreen("[OK] | Command executed in "); 
  Serial.print(tstamp); 
  Serial.println(" ms");
  printMagenta(tag); 
  printMagenta(" Length: ");
  Serial.println(in_len);
  printMagenta(tag); 
  printlnMagenta(":");
  HEXDUMP(in, in_len);
}

uint8_t  calculateSignVerifySign_ownkey()
{
  uint32_t ret = 0;
  uint8_t  data[DATA_LENGTH] = {'T', 'R', 'U', 'S', 'T', 'X', 'T', 'E', 'S', 'T'};
  uint8_t  ifxPublicKey[68];
  uint32_t ts = 0;
  
  uint16_t hashLen = HASH_LENGTH;
  uint16_t signLen = SIGN_LENGTH;
  uint16_t pubKeyLen = PUBKEY_LENGTH;

  /*
   * Extract public key of the device certificate
   */
  trustX.getPublicKey(ifxPublicKey);
   
  output_result("My Public Key", ts, ifxPublicKey, sizeof(ifxPublicKey));

  /*
   * Calculate a hash of the given data
   */
  printlnGreen("\r\nCalculate Hash ... ");
  ts = millis();
  ret = trustX.sha256(data, DATA_LENGTH, hash);
  ts = millis() - ts;
  hashLen = HASH_LENGTH;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }

  output_result("Hash", ts, hash, hashLen);

  /*
   * Sign hash
   */
  printlnGreen("\r\nGenerate Signature ... ");
  ts = millis();
  ret = trustX.calculateSignature(hash, hashLen, formSign, signLen);
  ts = millis() - ts;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }

  output_result("Signature #1", ts, formSign, signLen);

  /*
   * Verify a signature generated before
   */
  printlnGreen("\r\nVerify Signature ... ");
  ts = millis();
  ret = trustX.verifySignature(hash, hashLen, formSign, signLen, ifxPublicKey, sizeof(ifxPublicKey));
  ts = millis() - ts;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }
  
  printGreen("[OK] | Command executed in "); 
  Serial.print(ts); 
  Serial.println(" ms");
  return 1;
}

uint8_t calculateSignVerifySign_newkey()
{
  uint32_t ret = 0;
  uint8_t  data[DATA_LENGTH] = {'T', 'R', 'U', 'S', 'T', 'X', 'T', 'E', 'S', 'T'};
  uint8_t  ifxPublicKey[68];
  uint32_t ts = 0;
  
  uint16_t hashLen = HASH_LENGTH;
  uint16_t signLen = SIGN_LENGTH;
  uint16_t pubKeyLen = PUBKEY_LENGTH;

  /*
   * Calculate a hash of the given data
   */
  printlnGreen("\r\nCalculate Hash ... ");
  ts = millis();
  ret = trustX.sha256(data, DATA_LENGTH, hash);
  ts = millis() - ts;
  hashLen = HASH_LENGTH;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }

  output_result("Hash", ts, hash, hashLen);

  /*
   * Generate a key pair and store private key inside the security chip
   */
  printGreen("Generate Key Pair ... ");
  ts = millis();
  ret = trustX.generateKeypair(pubKey, pubKeyLen, eSESSION_ID_2);
  ts = millis() - ts;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }
  output_result("Public key", ts, pubKey, pubKeyLen);

  /*
   * Sign hash with the newly generated private key
   */
  printlnGreen("\r\nGenerate Signature ... ");
  ts = millis();
  ret = trustX.calculateSignature(hash, hashLen, eSESSION_ID_2, formSign, signLen);
  ts = millis() - ts;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }

  output_result("Signature #2", ts, formSign, signLen);

  /*
   * Verify a signature generated before
   */
  printlnGreen("\r\nVerify Signature ... ");
  ts = millis();
  ret = trustX.verifySignature(hash, hashLen, formSign, signLen, pubKey, pubKeyLen);
  ts = millis() - ts;
  if (ret) {
    printlnRed("Failed");
    return 0;
  }

  printGreen("[OK] | Command executed in "); 
  Serial.print(ts); 
  Serial.println(" ms");
  return 1;
}

void loop()
{
uint8_t ret=0;

  if(sys_init)
  { 
    /* Sign data and verify a signature with the embedded certificate */
    ret = calculateSignVerifySign_ownkey();
    if(ret==1)
    {
      Serial.println("Sign own key failed");
    }
    else
    {    
      /* Sign data and verify a signature with a newly generated keypair */
      ret = calculateSignVerifySign_newkey();

      if(ret==1)
      {
         Serial.println("Sign a new key failed");      
      }      
    }
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

