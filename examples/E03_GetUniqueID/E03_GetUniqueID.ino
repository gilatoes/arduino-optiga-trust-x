/* This source code is forked from Infineon GitHub, frozen and modified for training purposes. 
 * Therefore, it is neither updated regularly nor qualify for production. 
 * Please refer to official Infineon GitHub https://github.com/Infineon/arduino-optiga-trust-x 
 * for the updated and latest release. 
 */
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
#include <Arduino.h>
#include "debug.h"

uint16_t UID_LENGTH=27;

uint8_t sys_init =0;
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
  uint32_t ret = 0;
  Serial.println("Initialize Trust X");
  ret = trustX.begin();
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("OK");
  return 0;
}


void loop()
{
  uint32_t ret = 0;
  uint8_t  uid[UID_LENGTH];

  if(sys_init)
  {
    /*
     * Getting co-processor Unique ID of 27 bytes
     */
    Serial.println("\r\nGetting co-processor Unique ID:");
    ret = trustX.getUniqueID(uid, UID_LENGTH);

    if (ret) {
      Serial.println("Failed");
      Serial.println(ret,HEX);
      //close the connection
      trustX.end();
    }else{
    HEXDUMP(uid, UID_LENGTH);
    }

  }
  Serial.println("\r\nPress i to re-initialize..");
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
