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

 /*
 * GetCertificate example reads a digital certificate stored within Trust X data Object 0xE0E0. The certificate is encoded in X.509 format and dump out as hex output.
 * A X.509 certificate contains a public key and an identity that is signed by Infineon Certificate Authority (CA). The certificate can be copy-and-paste into a Hex to Base64 convertor.
 * Once the certificate is encoded into Base64 format, it should be enclosed with the following text.
 * certificate format
 * "-----BEGIN CERTIFICATE----- "
 * ....
 * ....
 * "-----END CERTIFICATE----- ".
 * [Hex and PEM (Base64) Converter] https://holtstrom.com/michael/tools/hextopem.php
 * Either use a local OpenSSL software or an online openssl tool to decode the certificate.
 *
 * [Certificate Decoder] https://www.sslshopper.com/certificate-decoder.html
 *
 * OpenSSL command:
 * openssl x509 -in <certificate.pem> -text -noout
 * 
 */
 
#include "OPTIGATrustX.h"
#include "debug.h"

#define CERT_MAXLENGTH   512

uint8_t *cert = new uint8_t[CERT_MAXLENGTH];

uint8_t sys_init =0;
void setup()
{
  uint32_t ret = 0;

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

void loop()
{
  uint32_t ret = 0;
  uint16_t certLen = 0;

 if(sys_init)
  {
    /*
     * Calculate a hash of the given data
     */
    Serial.println("Getting Certificate ... ");
    ret = trustX.getCertificate(cert, certLen);
    if (ret)
    {
      Serial.println("Failed");
    }

    Serial.println("[OK]");
    Serial.println("Certificate Length: ");
    Serial.println(certLen);
    Serial.println("Certificate:");

    HEXDUMP(cert, certLen);
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
	//Retry again
	ret = trustX.begin();
	if (ret) {
		Serial.println("Failed");
		return -1;
	}
  }
  
  Serial.print("Initializing setting: ");
  ret = trustX.setCurrentLimit(6);
  if (ret) {
    Serial.println("Failed");
    return -1;
  }
  Serial.println("Ok");
}
