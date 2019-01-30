/* This source code is forked from Infineon GitHub, frozen and modified for training purposes. 
 * Therefore, it is neither updated regularly nor qualify for production. 
 * Please refer to official Infineon GitHub https://github.com/Infineon/arduino-optiga-trust-x 
 * for the updated and latest release. 
 */
/**
 * MIT License
 *
 * Copyright (c) 2019 Infineon Technologies AG
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
#include <Arduino.h>
#include "debug.h"

#define VERIFY     0
uint8_t sys_init =0;
uint16_t UID_LENGTH=27;

//Max length of DER Encoded ECDSA signature using 256-bit EC Key
#define SIGN_MAX_LENGTH    72
#define MESSAGE_LENGTH    16
//SHA-256 Hash length
const uint16_t HASH_LENGTH = 32;
//IFX TLV + DER Octet tag uncompressed public key format
uint16_t PUBKEY_LENGTH = 68;

uint8_t *hash = new uint8_t[HASH_LENGTH];
uint8_t *Signature = new uint8_t[SIGN_MAX_LENGTH];

#define ASSERT(ret)   if(ret){debug_print("\r\nCheck:%d: %s\r\n", __LINE__, __func__);}

uint8_t  ifxPublicKey[68];
uint8_t  Message[16] = {'T', 'R', 'U', 'S', 'T', ' ', 'X', ' ', 'B', 'O', 'O', 'T', 'C', 'A', 'M', 'P'};

void setup()
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
  delay(100);
 /*
   * Initialise an OPTIGA™ Trust X Board
   */
  if(reset()==0){
    sys_init=1;
  }else{
    sys_init=0;
  }
}

uint8_t reset()
{
/*  
  uint32_t ret = 0;
  //Serial.println("Initialize Trust X");
  //ret = trustX.begin();
  if (ret) {
    Serial.print("Failed");
    return -1;
  }  
*/
  return 0;
}

//Setup the Public key and Hash message
uint32_t setup_PublickeyandHash()
{
  uint32_t ret = 0;
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

  return ret;

}

uint8_t calculateSignVerifySign_IFXkey()
{
  uint16_t ret = 0;  
  uint16_t Signature_Len =0; //The length of the signature will be returned from signature function

  /*
   * Exercise Secret key during the ECC Sign operation
   */
  uint32_t ts = 0;   
  ts = millis();
  ret = trustX.calculateSignature(hash, HASH_LENGTH, Signature, Signature_Len);
  ts = millis() - ts;  
  debug_print("Calculate Signature Time (ms): %d", ts);
  //debug_print("Signature length: %d", Signature_Len);
  //output_result(ret, Signature, Signature_Len);
  //ASSERT(ret);

  /*
   * (Optional step for Security Counter testing as secret key is not used during the verification step)
   * Verify a signature
   */  
#if (VERIFY==1)   
  ret = trustX.verifySignature(hash, HASH_LENGTH, Signature , Signature_Len, ifxPublicKey, PUBKEY_LENGTH);
  if(ret==0){
    //Serial.println("Verify Signature Ok");
  }else{
    Serial.println("Verify Signature failed");
  }
#endif  

  return 1;
}

void loop()
{
   uint32_t ret=0;
   uint8_t  uid[UID_LENGTH];
   uint16_t passed=0;
   uint16_t test_count=100;
   uint16_t exercise_secret_count=300;//Use Trust X Secret Key

   uint8_t  security_event_counter[1]={0};
   uint16_t SEC_OID=0xE0C5;
   uint16_t SEC_SIZE=1;

  if(sys_init)
  {
    Serial.println("Initialize Trust X");

    //Prepare the Public Key and hash message as signature input parameters
    ret = trustX.begin(); 
    ret = setup_PublickeyandHash();
    trustX.end();     

    if(ret==0){
      //Exercise the secret key by performing signature operation
      //Should observe performance throttling down when counter reaches certain threshold.
      for(int j=0;j<exercise_secret_count; j++){

        Serial.print("loop: ");
        Serial.print((j+1), DEC);
        Serial.print(" ");
        
        ret = trustX.begin();   
        //Serial.println("Exercise Trust X Secret Key");
        ret = calculateSignVerifySign_IFXkey();
        if(ret==0){
          Serial.println("Failed to Sign and Verify.");
        }
        
        ret = trustX.getArbitaryDataObject(SEC_OID, security_event_counter, SEC_SIZE);
        //Serial.print("Security event counter:");
        //Serial.println(security_event_counter[0], DEC);
        
        trustX.end(); 
      }    
      
      while(security_event_counter[0]){
      ret = trustX.begin();    
      ret = trustX.getArbitaryDataObject(SEC_OID, security_event_counter, SEC_SIZE);
      Serial.print("Security event counter:");      
  
      if (ret) {
        Serial.println("Failed");
        Serial.println(ret,HEX);
        //close the connection
        trustX.end();
      }else{
      Serial.println(security_event_counter[0], DEC);
      }    
       
      trustX.end(); 
      delay(3000);    
      }
    }

    //Serial.print("Test Completed (passed/total): ");
    //Serial.print(passed, DEC);
    //Serial.print("/");
    //Serial.println(test_count, DEC);

  }
  Serial.println("\r\nPress i to re-initialize.. other key to loop...");
  while (Serial.available()==0){} //Wait for user input
  String input = Serial.readString();  //Reading the Input string from Serial port.
  input.trim();
  if(input=="i"){
    if(reset()!=0){
      //Do not execute
      sys_init=0;
      //close the connection
      trustX.end();
    }else{
      sys_init=1;
      }
  }
}