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
#include "sha/sha256.h"
#include "debug.h"

#define MESSAGE_LENGTH    16
#define BIGDATA_LENGTH    1024
#define HASH_LENGTH       32

//Ignore the last character(used for printing format only)
uint8_t  Message[MESSAGE_LENGTH+1] = {'T', 'R', 'U', 'S', 'T', ' ', 'X', ' ', 'B', 'O', 'O', 'T', 'C', 'A', 'M', 'P','\0'};
uint8_t  bigdata[BIGDATA_LENGTH];
uint8_t  hash[HASH_LENGTH];

uint8_t sys_init =0;

#define ASSERT(ret)   if(ret){debug_print("\r\nCheck:%d: %s\r\n", __LINE__, __func__);return 0;}

void printHash(uint8_t* hash) {
  int i;
  for (i=0; i<32; i++) {
    Serial.print("0123456789abcdef"[hash[i]>>4]);
    Serial.print("0123456789abcdef"[hash[i]&0xf]);
  }
  Serial.println();
}
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

static void output_result(char* tag, uint32_t tstamp, uint8_t* in, uint16_t in_len)
{
  debug_print("[OK] | Time(ms): %d", tstamp);
  debug_print("Tag: %s", tag);
  
  HEXDUMP(in, in_len);
}

void loop()
{
  uint32_t ret = 0;
  // Timestamp is used to measure the execution time of a command
  uint32_t ts = 0;

   if(sys_init)
  { 

  Serial.println("Test: FIPS 180-2 B.1");
  Serial.println("Expect:ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
  Serial.print("Result:");
    
  Sha256.init();
  Sha256.print("abc");
  printHash(Sha256.result());
    
  /*
   * Calculate a hash of the given message
   */
  debug_print("Calculate FIPS 180-2 B.1 Hash for %s\r\n", "abc");
  ret = trustX.sha256("abc", 3, hash);    
  HEXDUMP(hash, HASH_LENGTH);
  ASSERT(ret);
    
  /*
   * Calculate a hash of the given message
   */
  debug_print("Calculate One-Time Hash for %s\r\n", Message);
  ts = millis();
  ret = trustX.sha256(Message, MESSAGE_LENGTH, hash);
  ts = millis() - ts;
  ASSERT(ret);
  debug_print("Time(ms): %d", ts);
  //Use an online SHA256 calculator to calculate this string "TRUST X BOOTCAMP"
  //Expected result: EEE166FF6C9432D018729BDEFFA8B9C5695C66839D0BF0542AA699C8CC77F671
  //Compare with Trust X compuation
  HEXDUMP(hash, HASH_LENGTH);

  /*
   * Calculate a hash of the given data (big input)
   */
  Serial.println("Calculate One-Time Hash for 1024 bytes");
  ts = millis();
  ret = trustX.sha256(bigdata, BIGDATA_LENGTH, hash);
  ts = millis() - ts;
  ASSERT(ret);
  debug_print("Time(ms): %d", ts);
  HEXDUMP(hash, HASH_LENGTH);

  /*
   * Benchmarking hash for the current microcontroller
   */
  Serial.println("Benchmarking SHA256 100 times for 16 bytes message\r\n");
  ts = millis();
  for (int i = 0; i < 100; i++) {
     trustX.sha256(Message, MESSAGE_LENGTH, hash);
  }
  ts = millis() - ts;
  ASSERT(ret);

  float ts_float = (float)ts;
  float each_hashrate = ts_float/100;
  float hash_per_sec = 1000/each_hashrate;
  debug_print("Total time taken for 100(16 bytes) hashes (ms): %f",ts_float );
  debug_print("Avg hash rate (ms): %f",each_hashrate ); 
  debug_print("Hashrate (H/sec): %f",hash_per_sec ); 

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


uint8_t reset()
{
  uint32_t ret = 0;
  Serial.println("Initialize Trust X...");
  ret = trustX.begin();
  if (ret) {
    Serial.print("Failed");
    return -1;
  }
  
  Serial.print("Initializing setting: ");
  ret = trustX.setCurrentLimit(6);
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("Ok");
  
  return 0;
}

