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
 * Infineon Technologies AG OPTIGAâ„¢ Trust X Arduino library
 */

#include "OPTIGATrustX.h"
#include <Arduino.h>
#include "debug.h"

uint8_t sys_init =0;

uint16_t OID[] = {0xE0C0, 0xE0C1, 0xE0C2, 0xE0C3, 0xE0C4, 0xE0C5, 0xE0C6, 0xE0E0, 0xE0E1, 0xE0E2, 0xE0E3, 0xE0E8, 0xE0EF};
uint16_t OID_max_size[] = {1, 1, 27, 1, 1, 1, 2, 1728, 1728, 1728, 1728, 1024, 1024};
#define MAX_OID_SIZE_VALUE  1728

#define ENABLE_TRUSTX_CHARACTERISTICS_OBJECT  0
#define ENABLE_IFX_ISSUED_CERT_OBJECT         0
#define ENABLE_PROJECT_SPECIFIC_CERT_OBJECTS  0
#define ENABLE_ROOTCA_CERTIFICATES            0
#define ENABLE_ARBITRARY_OBJECTS              0

#define EASY_CUT_PASTE 1
void setup()
{
  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
  delay(100);

 /*
   * Initialise an OPTIGAâ„¢ Trust X Board
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

void printmetadata(uint8_t *metadata)
{
  uint16_t maxsize = 0;
  uint16_t usedsize = 0;
  
  //Serial.print("OID maxsize:");
  maxsize = metadata[7]<<8 |(metadata[8]);
  //Serial.println(maxsize, DEC);

  //Serial.print("OID usedsize:");
  usedsize = metadata[11]<<8 |(metadata[12]);
  //Serial.println(usedsize, DEC);

  Serial.print("OID: usedsize/maxsize = ");
  Serial.print(usedsize, DEC);
  Serial.print("/");
  Serial.println(maxsize, DEC);

  Serial.print("TLV Change AC: ");
  Serial.print(" "); Serial.print(metadata[15], HEX);
  Serial.print(" "); Serial.print(metadata[16], HEX);
  Serial.print(" "); Serial.println(metadata[17], HEX);

  Serial.print("TLV Read AC: ");
  Serial.print(" "); Serial.print(metadata[19], HEX);
  Serial.print(" "); Serial.println(metadata[20], HEX);
 
}
void printObject(uint8_t object)
{
  uint32_t ret = 0;
  uint16_t MAX_OID_SIZE =1500;
  uint8_t temp[MAX_OID_SIZE_VALUE];

  //Clear temp buffer
  memset(temp, 0x0, MAX_OID_SIZE_VALUE);

  ret = trustX.getArbitaryDataObject(OID[object], temp, OID_max_size[object]);
  if (ret){
    Serial.print("Error: Failed to read object");
    Serial.println(ret, HEX);
  }
  else{
#if (EASY_CUT_PASTE ==1)
      HEXONLYDUMP(temp, OID_max_size[object]);Serial.println("============");
#endif
      HEXDUMP(temp, OID_max_size[object]);
    }
}

void printDataObject(uint16_t object)
{
  uint32_t ret = 0;
  uint16_t MAX_BIG_SIZE =1500;
  uint16_t MAX_SMALL_SIZE =100;
  uint8_t temp[MAX_BIG_SIZE];

  //Clear temp buffer
  memset(temp, 0x0, MAX_BIG_SIZE);

  if((object == 0xF1E0) || ( object ==0xF1E1)){
    //Serial.println("Large Data Object");
    ret = trustX.getArbitaryDataObject(object, temp, MAX_BIG_SIZE);
  }
  else{
    //Serial.println("Small Data Object");
    ret = trustX.getArbitaryDataObject(object, temp, MAX_SMALL_SIZE);
  }
  if (ret){
    Serial.print("Error: Failed to read object");
    Serial.println(ret, HEX);
  }
  else{
    if((object == 0xF1E0) || ( object ==0xF1E1))
    {
#if (EASY_CUT_PASTE ==1)
      HEXONLYDUMP(temp, MAX_BIG_SIZE);Serial.println("============");
#endif
      HEXDUMP(temp, MAX_BIG_SIZE);
    }else{
#if (EASY_CUT_PASTE ==1)
      HEXONLYDUMP(temp, MAX_SMALL_SIZE);Serial.println("============");
#endif
      HEXDUMP(temp, MAX_SMALL_SIZE);
      }
  }
}
void loop()
{
  uint32_t ret = 0;

  if(sys_init)
  {
      uint16_t FirstCert=0xE0E0;
      uint16_t SecondCert=0xE0E1;
      uint16_t ThirdCert=0xE0E2;
      uint16_t FourthCert=0xE0E3;
      uint16_t TrustAnchor1=0xE0E8;
      uint16_t TrustAnchor2=0xE0EF;
      
      uint8_t temp[28];
      
      //Clear temp buffer
      memset(temp, 0x0, 28);

      Serial.println("Cert 1:");
      ret = trustX.getMetaData(FirstCert, temp);
      HEXONLYDUMP(temp, 28);
      printmetadata(temp);

      Serial.println("Cert 2:");
       memset(temp, 0x0, 28);
      ret = trustX.getMetaData(SecondCert, temp);
      HEXONLYDUMP(temp, 28);
      printmetadata(temp);

      Serial.println("Cert 3:");
       memset(temp, 0x0, 28);
      ret = trustX.getMetaData(ThirdCert, temp);
      HEXONLYDUMP(temp, 28);
      printmetadata(temp);

      Serial.println("Cert 4:");
       memset(temp, 0x0, 28);
      ret = trustX.getMetaData(FourthCert, temp);
      HEXONLYDUMP(temp, 28);
      printmetadata(temp);

      Serial.println("TrustAnchor1:");
       memset(temp, 0x0, 28);
      ret = trustX.getMetaData(TrustAnchor1, temp);
      HEXONLYDUMP(temp, 28);
      printmetadata(temp);

      Serial.println("TrustAnchor2:");
       memset(temp, 0x0, 28);
      ret = trustX.getMetaData(TrustAnchor2, temp);
      HEXONLYDUMP(temp, 28);
      printmetadata(temp);

#if (ENABLE_TRUSTX_CHARACTERISTICS_OBJECT == 1)
    // Get Data structure Global (LcsG) = 0xE0C0
    Serial.print("\r\nLscG Object(Default=0x07): 0x");
    Serial.println(OID[0],HEX);
    printObject(0);

    // Get Security State = E0C1
    Serial.print("\r\nSecurity State Object(Default=0x00): 0x");
    Serial.println(OID[1],HEX);
    printObject(1);

    // Get co-processor Unique ID of 27 bytes  = 0xE0C2
    Serial.print("\r\nGet co-processor Unique ID Object: 0x");
    Serial.println(OID[2],HEX);
    printObject(2);

    // Get Sleep mode activation delay  = 0xE0C3
    Serial.print("\r\nSleep mode activation delay Object(Default=0x14): 0x");
    Serial.println(OID[3],HEX);
    printObject(3);

    // Get current limitation  = 0xE0C4
    Serial.print("\r\nCurrent limit Object(Default=0x06): 0x");
    Serial.println(OID[4],HEX);
    printObject(4);

    // Get Security Event Counter  = 0xE0C5
    Serial.print("\r\nSecurity Event Counter Object(Default=0xXX): 0x");
    Serial.println(OID[5],HEX);
    printObject(5);

    // Get Max Com buffer Size  = 0xE0C6
    Serial.print("\r\nMax Comm buffer size Object(Default=0xXX 0xXX): 0x");
    Serial.println(OID[6],HEX);
    printObject(6);
#endif

#if (ENABLE_IFX_ISSUED_CERT_OBJECT == 1)
    // Get IFX issued Device Public Key Certificate  = 0xE0E0
    Serial.print("\r\nIFX issued Device Public Key Certificate Object: 0x");
    Serial.println(OID[7],HEX);
    printObject(7);
#endif

#if (ENABLE_PROJECT_SPECIFIC_CERT_OBJECTS == 1)
    // Get Project specific device Public Key Certificate  = 0xE0E1
    Serial.print("\r\nProject 1 device Public Key Certificate Object: 0x");
    Serial.println(OID[8],HEX);
    printObject(8);

    // Get Project 1 device Public Key Certificate  = 0xE0E2
    Serial.print("\r\nProject 2 device Public Key Certificate Object: 0x");
    Serial.println(OID[9],HEX);
    printObject(9);

    // Get Project specific device Public Key Certificate  = 0xE0E3
    Serial.print("\r\nProject 3 device Public Key Certificate Object: 0x");
    Serial.println(OID[10],HEX);
    printObject(10);
#endif

#if (ENABLE_ROOTCA_CERTIFICATES==1)
    // Get RootCA Public Key Certificate  = 0xE0E8
    Serial.print("\r\nRootCA Public Key Certificate Object: 0x");
    Serial.println(OID[11],HEX);
    printObject(11);

    // Get Platform Integrity Public Key Certificate  = 0xE0EF
    Serial.print("\r\nPlatform Integrity Public Key Certificate Object: 0x");
    Serial.println(OID[12],HEX);
    printObject(12);
#endif

#if (ENABLE_ARBITRARY_OBJECTS == 1)
    uint16_t DATA_OID = 0xF1D0;

    //Get small NVM data objects
    for(uint8_t i=0;i<15;i++){
      Serial.print("\r\nData Object: 0x");
      Serial.println(DATA_OID+i,HEX);
      printDataObject(DATA_OID+i);
      delay(100);
     }

    Serial.println("\r\nData Object: 0xF1E0");
    printDataObject(0xF1E0);

    delay(100);

    Serial.println("\r\nData Object: 0xF1E1");
    printDataObject(0xF1E1);
#endif
  }
  Serial.print("\r\nPress i to re-initialize.. other key to loop...");
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
