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
#include "third_crypto/uECC.h"
#include "debug.h"

uint8_t sys_init =0;
#define TLV_PADDING    4
#define TLV_OFFSET     4

void setup()
{
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
  return 0;
}


void loop()
{
  uint32_t ret = 0;
  uint8_t PublicKey[64+TLV_PADDING]=
  {
  0x03, 0x42, 0x00, 0x04, 0x20, 0xb6, 0x11, 0x90, 0x3e, 0x0a, 0x2e, 0x86, 0x02, 0x5e, 0xb4, 0x84,
  0x82, 0x66, 0x93, 0x7a, 0x73, 0x4a, 0x38, 0x2b, 0xa4, 0x32, 0xf0, 0xfd, 0xcb, 0x9c, 0xf7, 0xea,
  0xbf, 0x03, 0xbc, 0x20, 0xec, 0x73, 0x4d, 0x3e, 0x8f, 0x77, 0x2a, 0x6f, 0x94, 0x98, 0x93, 0x8f,
  0x40, 0x32, 0x55, 0xa0, 0xe8, 0x70, 0xd1, 0x8e, 0xda, 0x33, 0xf0, 0xf9, 0x3a, 0xc6, 0x3d, 0x95,
  0x3d, 0xe7, 0x2e, 0xfa};

  if(sys_init)
  {
    Serial.println("Checking Public Key:");
	
   ret = uECC_valid_public_key(PublicKey + TLV_OFFSET, uECC_secp256r1());
   if(ret==1)
   {Serial.println("Valid Public Key. EC Point on Curve.");}
   else
   {Serial.println("Error: Invalid Public key.");}
  

  }
  Serial.println("\r\nPress i for an iteration...");
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