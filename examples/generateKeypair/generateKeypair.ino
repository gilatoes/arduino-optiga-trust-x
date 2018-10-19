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

#define KEY_MAXLENGTH    70

#define SUPPRESSCOLLORS
#include "fprint.h"

uint8_t *pubKey = new uint8_t[KEY_MAXLENGTH];
uint8_t *privKey = new uint8_t[KEY_MAXLENGTH];

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

void loop()
{
  uint32_t ret = 0;
  uint8_t  cntr = 10;
  uint32_t ts = 0;
  /* OPTIGA Trust X support up to 4 contexts to store you private key  */
  uint16_t ctx = 0;
  uint16_t pubKeyLen = 0;
  uint16_t privKeyLen = 0;

  if(sys_init)
  {
    /*
     * Generate a keypair#1
     */
    printlnGreen("\r\nGenerate Key Pair. Store Private Key on Board ... ");
    ts = millis();
    ret = trustX.generateKeypair(pubKey, pubKeyLen, ctx);
    ts = millis() - ts;
    if (ret) {
      printlnRed("Failed");
      while (true);
    }

    output_result("Public Key ", ts, pubKey, pubKeyLen);


    /*
     * Generate a keypair#2
     */
    printlnGreen("\r\nGenerate Key Pair. Export Private Key ... ");
    ts = millis();
    ret = trustX.generateKeypair(pubKey, pubKeyLen, privKey, privKeyLen);
    ts = millis() - ts;
    if (ret) {
      printlnRed("Failed");
      while (true);
    }

    output_result("Public Key ", ts, pubKey, pubKeyLen);
    output_result("Private Key ", ts, privKey, privKeyLen);

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
