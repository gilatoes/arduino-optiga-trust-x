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
#include "debug.h"

#define RND_MAXLENGTH     256

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

static void output_result(char* tag, uint32_t tstamp, uint8_t* in, uint16_t in_len)
{
  debug_print("[OK] | Time(ms): %d", tstamp);
  debug_print("Tag: %s", tag);

  HEXDUMP(in, in_len);
}

void loop()
{
  uint32_t ret = 0;
  uint8_t  *rnd = new uint8_t[RND_MAXLENGTH];
  uint32_t ts = 0;

  /* Initialise Memmory Area */
  memset(rnd, 0, RND_MAXLENGTH);

  if(sys_init)
  {

    /*
     * Generate Random values of different sizes
     */
    Serial.println("\r\nGet 16 bytes random number");
    ts = millis();
    ret = trustX.getRandom(16, rnd);
    ts = millis() - ts;
    if (ret) {
      Serial.println("Failed");
      while (true);
    }
    output_result("Random", ts, rnd, 16);

    Serial.println("\r\nGet 32 bytes random number");
    ts = millis();
    ret = trustX.getRandom(32, rnd);
    ts = millis() - ts;
    if (ret) {
      Serial.println("Failed");
      while (true);
    }
    output_result("Random", ts, rnd, 32);

    Serial.println("\r\nGet 64 bytes random number");
    ts = millis();
    ret = trustX.getRandom(64, rnd);
    ts = millis() - ts;
    if (ret) {
      Serial.println("Failed");
      while (true);
    }
    output_result("Random", ts, rnd, 64);

    Serial.println("\r\nGet 128 bytes random number");
    ts = millis();
    ret = trustX.getRandom(128, rnd);
    ts = millis() - ts;
    if (ret) {
      Serial.println("Failed");
      while (true);
    }
    output_result("Random", ts, rnd, 128);

    Serial.println("\r\nGet 256 bytes random number");
    ts = millis();
    ret = trustX.getRandom(256, rnd);
    ts = millis() - ts;
    if (ret) {
      Serial.println("Failed");
      while (true);
    }
    output_result("Random", ts, rnd, 256);

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

  /*
   * Remove dynamically allocated data
   */
  delete rnd;
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


  Serial.println("Initializing setting");
  ret = trustX.setCurrentLimit(6);
  
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("OK");
  return 0;
}
