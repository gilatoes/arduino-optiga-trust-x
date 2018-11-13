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

#define UID_LENGTH        27

#define SUPPRESSCOLLORS
#include "fprint.h"

uint8_t sys_init =0;
void setup()
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
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

#if( UC_FAMILY == XMC1 )
  led1Off();
  led2Off();
#endif
  return 0;
}


void loop()
{
  uint32_t ret = 0;
  uint8_t  cntr = 10;
  uint8_t  uid[UID_LENGTH];
  uint16_t uidLength = UID_LENGTH;

  if(sys_init)
  {
    /*
     * Getting co-processor Unique ID of 27 bytes
     */
    printlnGreen("\r\nGetting co-processor Unique ID...");
    ret = trustX.getUniqueID(uid, uidLength);

    if (ret) {
      printlnRed("Failed");
      Serial.println(ret,HEX);
      //close the connection
      trustX.end();
    }else{
    HEXDUMP(uid, uidLength);
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