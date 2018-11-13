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

#define UID_LENGTH        27

//"Provision me" enables 2 new pair of public and private key being generated.
//The purpose of this is to calculate share secret between the 2 parties.
#define PROVISION_ME
//#define READBACK_TEST

//Choice of Public Key storage
uint16_t PubKeyStore_OID_1 = 0xF1D0;   
uint16_t PubKeyStore_OID_2 = 0xF1D1;
uint16_t PrivateKeyStore_1 = 0xE0F1;
uint16_t PrivateKeyStore_2 = 0xE0F2;
uint16_t PUBLICKEY_LENGTH=68;

uint16_t SHAREDSECRET_LEN = 32;
#define ExportSharedSecret   0x0000 //Export ShareSecret

//Configure the choice of Shared Secret storage
//#define ARBITARY_SHAREDSECRET
#define CONTEXT_SHAREDSECRET

#ifdef ARBITARY_SHAREDSECRET //Arbitrary data object
uint16_t ShareSecretStore_OID_1 = 0xF1D2;   
uint16_t ShareSecretStore_OID_2 = 0xF1D3;
#endif

#ifdef CONTEXT_SHAREDSECRET //Session context object
uint16_t ShareSecretStore_OID_1 = 0xE101;   
uint16_t ShareSecretStore_OID_2 = 0xE102;
#endif

#define ExportDeriveKey   0x0000
uint16_t DERIVEDKEY_LEN = 16;

#include "debug.h"

uint8_t sys_init =0;

void setup()
{
    uint32_t ret = 0;
    uint8_t  uid[UID_LENGTH];
    uint16_t uidLength = UID_LENGTH;
        
    Serial.begin(115200, SERIAL_8N1);
    delay(1000);
    Serial.println("Initializing Trust X ... ");
       
    if(reset()==0){
      sys_init=1;
    }else{
      sys_init=0;
    }

    Serial.println("Check Unique ID to make sure that FW supports arbitrary data object key derivation.");
    ret = trustX.getUniqueID(uid, uidLength);
    if (ret) {
      Serial.println("Error: Unable to read Trust X UID.");
      Serial.println(ret,HEX);  
      //close the connection
      trustX.end();         
    }else{  
    HEXDUMP(uid, uidLength);
    }

#ifdef ARBITARY_SHAREDSECRET
    if(uid[25]!= 0x11 && uid[26] != 0x18)
    {
      Serial.println("Error: Trust X FW version must be at least 0x1118");
      while(1);
    }    
#endif    

#ifdef PROVISION_ME
  makekeypair(PrivateKeyStore_1, PubKeyStore_OID_1);
  makekeypair(PrivateKeyStore_2, PubKeyStore_OID_2);   
#endif  
}

//Make a new key pair with a defined Private key slot and store export public key data OID
void makekeypair(uint16_t PrivateKeySlot, uint16_t PublicKeyStore_OID)
{
  uint32_t ret = 0;
  uint8_t *PubKey = new uint8_t[PUBLICKEY_LENGTH];

  Serial.println("\r\nGenerate Key Pair and export public key in plaintext.");
  Serial.print("Private KeySlot OID: 0x");
  Serial.println(PrivateKeySlot,HEX);
  Serial.print("Write Public key in Data OID: 0x");
  Serial.println(PublicKeyStore_OID,HEX);
  ret = trustX.generateKeypair(PubKey, PUBLICKEY_LENGTH, PrivateKeySlot);
  
  if (ret) {
    Serial.println("Error generating key pair");
    while (true);
  }
  
  ret = trustX.setArbitaryDataObject(PublicKeyStore_OID, PubKey, PUBLICKEY_LENGTH);    
  if (ret){
    Serial.print("Error: Failed to write Public Key in OID");
  }
  else{
    
#ifdef READBACK_TEST
    uint8_t *readPubKey = new uint8_t[PUBLICKEY_LENGTH];
    get_PubKeyStore(PublicKeyStore_OID, readPubKey);
    HEXDUMP(readPubKey, PUBLICKEY_LENGTH);
#endif    
  }
}

//compare oid utility
uint8_t compare_oid(uint16_t oid1, uint16_t oid2, uint16_t len)
{
  uint32_t ret = 0;
  uint8_t *oid_data1 = new uint8_t[len];
  uint8_t *oid_data2 = new uint8_t[len];

  ret = trustX.getArbitaryDataObject(oid1, oid_data1, len);
  if (ret){  
    Serial.print("Error: Failed to read data from OID 1 parameter");
    Serial.println(ret, HEX);
  }
  
  ret = trustX.getArbitaryDataObject(oid2, oid_data2, len);
  if (ret){  
    Serial.print("Error: Failed to read data from OID 2 parameter");
    Serial.println(ret, HEX);
  }
  
  ret = memcmp(oid_data1,oid_data2, len);
  if (ret==0){
    return 0;
  }
  else{    
    return 1;
  }
}

//compare array utility
uint8_t compare_array(uint8_t *a, uint8_t *b, uint16_t len)
{
     uint16_t n=0;

     for (n=0; n<len; n++) {
      if (a[n]!=b[n]){
        return 1;
      }
     }     
     return 0;
}

// Calculate a shared secret and store export shared secret in arbitrary data OID
void calculateShareSecretStore(uint16_t PrivateKeySlot, uint16_t PublicKeyStore_OID, uint16_t sharedsecret_len, uint16_t ShareSecretStore_OID)
{
  uint8_t *sharedsecret = new uint8_t[sharedsecret_len];
  uint32_t ret = 0;
  uint8_t *PubKey = new uint8_t[PUBLICKEY_LENGTH];

  //Get Public key from public key store
  get_PubKeyStore(PublicKeyStore_OID, PubKey);

  Serial.print("Store Shared Secret Key in Data OID: 0x");
  Serial.println(ShareSecretStore_OID,HEX);

  if( (ShareSecretStore_OID == 0xE100) ||
      (ShareSecretStore_OID == 0xE101) ||
      (ShareSecretStore_OID == 0xE102) ||
      (ShareSecretStore_OID == 0xE103) )
      {
        ret = trustX.sharedSecret(PrivateKeySlot, PubKey, PUBLICKEY_LENGTH, ShareSecretStore_OID, sharedsecret, sharedsecret_len);
        if (ret) {
          Serial.print("Error code:");
          Serial.print(ret,HEX);     
          while (true);
        }
        Serial.print("Shared Secret stored in context OID:");
        Serial.println(ShareSecretStore_OID,HEX); 
  }
  else
  {
     ret = trustX.sharedSecret(PrivateKeySlot, PubKey, PUBLICKEY_LENGTH, ExportSharedSecret, sharedsecret, sharedsecret_len);
      if (ret) {
        Serial.print("Error code:");
        Serial.println(ret,HEX);     
        while (true);
       }
#ifdef READBACK_TEST   
    Serial.println("----- BEGIN SHARED SECRET KEY -----"); 
    HEXONLYDUMP(sharedsecret, sharedsecret_len);
    Serial.println("----- END SHARED SECRET KEY -----"); 
#endif

  //Store the Share Secret in the data OID
  ret = trustX.setArbitaryDataObject(ShareSecretStore_OID, sharedsecret, sharedsecret_len);    
  if (ret){
    Serial.print("Error: Failed to write Share Secret in data OID");
  }
  else{
    
#ifdef READBACK_TEST
    uint8_t *readsharesecretkey = new uint8_t[sharedsecret_len];
    get_SharedSecretStore(ShareSecretStore_OID, readsharesecretkey, sharedsecret_len);
    HEXDUMP(readsharesecretkey, sharedsecret_len);
#endif    
    }
   }
}

//Returns the provisioned Public key from arbitrary data OID
void get_PubKeyStore(uint16_t PublicKeyStore_OID, uint8_t *publickey)
{
  uint32_t ret = 0;
  uint8_t data_readback[PUBLICKEY_LENGTH];

  memset(data_readback, 0x0, PUBLICKEY_LENGTH);

  Serial.print("Read Public key from Data OID: 0x");
  Serial.println(PublicKeyStore_OID,HEX);
  ret = trustX.getArbitaryDataObject(PublicKeyStore_OID, data_readback, PUBLICKEY_LENGTH);
  if (ret){  
    Serial.print("Error: Failed to read Public Key from OID");
    Serial.println(ret, HEX);
  }
   else{      
#ifdef READBACK_TEST
    HEXDUMP(data_readback, PUBLICKEY_LENGTH);
#endif    
    memcpy(publickey, data_readback, PUBLICKEY_LENGTH);
  }
}

//Returns the Shared Secret key from arbitrary data OID
void get_SharedSecretStore(uint16_t SharedSecretStore_OID, uint8_t *SharedSecret, uint16_t SharedSecret_Len)
{
  uint32_t ret = 0;
  uint8_t data_readback[SharedSecret_Len];
  memset(data_readback, 0x0, SharedSecret_Len);
  Serial.print("Read Shared Secret Key from Data OID: 0x");
  Serial.println(SharedSecretStore_OID,HEX);

    if((SharedSecretStore_OID!= 0xE100) ||
      (SharedSecretStore_OID!= 0xE101) ||
      (SharedSecretStore_OID!= 0xE102) ||
      (SharedSecretStore_OID!= 0xE103)
      ){

        ret = trustX.getArbitaryDataObject(SharedSecretStore_OID, data_readback, SharedSecret_Len);
        if (ret){  
          Serial.print("Error: Failed to read Shared Secret Key from OID");
          Serial.println(ret, HEX);
        }
         else{      
#ifdef READBACK_TEST    
    HEXDUMP(data_readback, SharedSecret_Len);
#endif    
          memcpy(SharedSecret, data_readback, SharedSecret_Len);
        }
      }
      else{
        Serial.print("Skipped reading context OID.");
        }
}
void loop()
{
  uint32_t ret = 0;

  if(sys_init)
  {       
    uint8_t *derivekey1 = new uint8_t[DERIVEDKEY_LEN];
    uint8_t *derivekey2 = new uint8_t[DERIVEDKEY_LEN];

    memset(derivekey1, 0x0, DERIVEDKEY_LEN);
    memset(derivekey2, 0x0, DERIVEDKEY_LEN);

    calculateShareSecretStore(PrivateKeyStore_1, PubKeyStore_OID_2, SHAREDSECRET_LEN, ShareSecretStore_OID_1);
#ifdef READBACK_TEST
    if((ShareSecretStore_OID_1!= 0xE100) &&
       (ShareSecretStore_OID_1!= 0xE101) &&
       (ShareSecretStore_OID_1!= 0xE102) &&
       (ShareSecretStore_OID_1!= 0xE103)
      )
    {
      Serial.println("Party 1 Shared Secret key from OID: 0x"); 
      Serial.println(ShareSecretStore_OID_1,HEX);
      uint8_t *readsharesecretkey = new uint8_t[SHAREDSECRET_LEN];
      get_SharedSecretStore(ShareSecretStore_OID_1, readsharesecretkey,SHAREDSECRET_LEN);
      HEXDUMP(readsharesecretkey, SHAREDSECRET_LEN);
    }
#endif

    calculateShareSecretStore(PrivateKeyStore_2, PubKeyStore_OID_1, SHAREDSECRET_LEN, ShareSecretStore_OID_2);
#ifdef READBACK_TEST
    if((ShareSecretStore_OID_2!= 0xE100) &&
       (ShareSecretStore_OID_2!= 0xE101) &&
       (ShareSecretStore_OID_2!= 0xE102) &&
       (ShareSecretStore_OID_2!= 0xE103)
      )
    {
      Serial.println("Party 2 Shared Secret key from OID: 0x"); 
      Serial.println(ShareSecretStore_OID_2,HEX);
      uint8_t *readsharesecretkey = new uint8_t[SHAREDSECRET_LEN];
      get_SharedSecretStore(ShareSecretStore_OID_2, readsharesecretkey, SHAREDSECRET_LEN);
      HEXDUMP(readsharesecretkey, SHAREDSECRET_LEN);
    }
#endif    

    //Only exported shared secret can be compared
    if((ShareSecretStore_OID_1!= 0xE100) &&
       (ShareSecretStore_OID_1!= 0xE101) &&
       (ShareSecretStore_OID_1!= 0xE102) &&
       (ShareSecretStore_OID_1!= 0xE103) &&
       (ShareSecretStore_OID_2!= 0xE100) &&
       (ShareSecretStore_OID_2!= 0xE101) &&
       (ShareSecretStore_OID_2!= 0xE102) &&
       (ShareSecretStore_OID_2!= 0xE103) 
     )
    {
      uint8_t result = compare_oid(ShareSecretStore_OID_1, ShareSecretStore_OID_2, SHAREDSECRET_LEN);
      if(result==0)
      { Serial.println("\r\n+++++++++++++ Shared secret is established +++++++++++++.\r\n"); }
      else
      {Serial.println("\r\n---------- Unable to establish share secret -----------"); 
      while(1);
      }
    }

    Serial.println("Export plaintext derive key 1.");
    Serial.print("Shared Secret OID: 0x");
    Serial.println(ShareSecretStore_OID_1, HEX);
    ret = trustX.deriveKey( ShareSecretStore_OID_1, SHAREDSECRET_LEN, ExportDeriveKey, derivekey1, DERIVEDKEY_LEN);
    if (ret) {
      Serial.print("Error code: ");
      Serial.println(ret,HEX);     
      while (true);
    }    
    Serial.println("\r\n-----BEGIN DERIVE KEY 1-----"); 
    HEXONLYDUMP(derivekey1, DERIVEDKEY_LEN);
    Serial.println("-----END DERIVE KEY-----"); 

    Serial.println("Export plaintext derive key 2.");
    Serial.print("Shared Secret OID: 0x");
    Serial.println(ShareSecretStore_OID_1, HEX);
    ret = trustX.deriveKey( ShareSecretStore_OID_2, SHAREDSECRET_LEN, ExportDeriveKey, derivekey2, DERIVEDKEY_LEN);
    if (ret) {
      Serial.print("Error code: ");
      Serial.println(ret,HEX);     
      while (true);
    }    
    Serial.println("\r\n-----BEGIN DERIVE KEY 2-----"); 
    HEXONLYDUMP(derivekey2, DERIVEDKEY_LEN);
    Serial.println("-----END DERIVE KEY-----"); 

    ret = compare_array(derivekey1,derivekey2, DERIVEDKEY_LEN );

    if (ret == 0){
      Serial.println("\r\n+++++++++++++ Derived key is the same +++++++++++++"); 
      }
    else{
      Serial.println("\r\n----------- Derived key is different --------------"); 
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
}

uint8_t reset()
{
  uint32_t ret = 0;
  Serial.println("Begin to trust ... ");
  ret = trustX.begin();
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("OK");

   /*
   * Speedup the board (from 6 mA to 15 mA)
   */
  Serial.println("Limiting Current consumption (15mA - means no limitation) ... ");
  ret = trustX.setCurrentLimit(15);
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("OK");

  return 0;
}
