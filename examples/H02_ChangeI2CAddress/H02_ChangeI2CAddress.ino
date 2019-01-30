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

uint8_t sys_init =0;
void setup() 
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
  Serial.println("Initializing ... ");
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
  Serial.print("Begin to trust ... ");
  ret = trustX.begin();
  if (ret) {
    Serial.print("Failed");
    return -1;   
  }
  
  Serial.println("Initializing setting");
  ret = trustX.setCurrentLimit(6);  
  
  Serial.println("Done");
  
  return 0;
}

void loop()
{
   uint8_t  security_event_counter[1];
   uint32_t ret = 0; 
   uint16_t SEC_OID = 0xE0C5;
   uint16_t SEC_SIZE =1;
     
  if(sys_init)
  {
    //Make sure that the driver is configured with a valid current address.
	//The set_i2c_address will change the Trust X address register. 
	// If the persistent mode is set, the change will be committed into the NVM.
    Serial.println("I2C address is changed.");
    trustX.set_i2c_address(0x20);

    ret = trustX.getArbitaryDataObject(SEC_OID, security_event_counter, SEC_SIZE);

    Serial.print("Security event counter:");  
    Serial.println(security_event_counter[0], HEX);
    
  }
  Serial.println("\r\nPress i to re-initialize.. other key to loop...");   
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
