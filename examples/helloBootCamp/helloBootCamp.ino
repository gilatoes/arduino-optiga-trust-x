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

#define SUPPRESSCOLLORS
#include "fprint.h"

// Linker symbols to get falsh/RAM usage
#if( UC_FAMILY == XMC1 )
extern char VeneerStart, VeneerEnd, eROData;
#endif
extern char __ram_code_start, __ram_code_end, __bss_end, __bss_start;
extern char Heap_Bank1_Start, Heap_Bank1_End;
extern char sText, __initial_sp, __stack_start, __data_start, __data_end;

#define str(x)  Serial.println( #x )
#define str1(x) str(x)
#define str2(x) Serial.print( #x )

uint32_t temp, code_size;

uint8_t sys_init =0;

void setup()
{
  uint32_t ret = 0;

  /*
   * Initialise a serial port for debug output
   */
  Serial.begin(115200, SERIAL_8N1);
  Serial.println("Initializing XMC... ");

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

void sys_test()
{
  delay( 60 );
  Serial.println( "PC Services - XMC-for-Arduino useful defines example" );
  #ifdef XMC_BOARD_NAME
  Serial.read();
  str2( XMC_BOARD_NAME );
  Serial.write( '\t' );
  str1( XMC_BOARD_NAME );
  #endif
  Serial.print( "Family\t\t" );
  str1( UC_FAMILY );
  Serial.print( "Series\t\t" );
  str1( UC_SERIES );
  Serial.print( "Device\t\t" );
  str1( UC_DEVICE );
  Serial.print( "Package\t\t" );
  str1( UC_PACKAGE );
  Serial.print( "Flash (kB)\t" );
  Serial.println( UC_FLASH );
  #if defined ARDUINO_ARM_XMC
  str2( ARDUINO_ARM_XMC );
  Serial.write( '\t' );
  str1( ARDUINO_ARM_XMC );
  #endif
  #if defined UC_RAM
  str2( UC_RAM );
  Serial.print( "\t\t" );
  str1( UC_RAM );
  str2( UC_ALL_RAM );
  Serial.write( '\t' );
  str1( UC_ALL_RAM );
  #endif
  str2( F_CPU );
  Serial.print( "\t\t" );
  str1( F_CPU );
  str2( ARDUINO );
  Serial.print( "\t\t" );
  str1( ARDUINO );
  str2( ARDUINO_ARCH_ARM );
  Serial.write( ' ' );
  str1( ARDUINO_ARCH_ARM );

  Serial.print( "Library Version\t" );
  Serial.print( XMC_LIB_MAJOR_VERSION );
  Serial.print( '.' );
  Serial.print( XMC_LIB_MINOR_VERSION );
  Serial.write( '.' );
  Serial.println( XMC_LIB_PATCH_VERSION );

  Serial.println( "\nMemory usage-" );
  Serial.print( "Code Size:\t" );
  #if( UC_FAMILY == XMC1 )
  code_size = &eROData - &sText;
  #else
  code_size = &__data_end - &sText;
  #endif
  Serial.println( code_size );

  #if( UC_FAMILY == XMC1 )
  Serial.print( "Veneer:\t\t" );
  temp = &VeneerEnd - &VeneerStart;
  code_size += temp;
  Serial.println( temp );
  #endif
  Serial.print( "Stack\t\t" );
  temp = &__initial_sp - &__stack_start;
  Serial.println( temp );
  Serial.print( "Data\t\t" );
  temp =  &__data_end - &__data_start;
  code_size += temp;
  Serial.println( temp );
  Serial.print( "Ram Code\t" );
  temp =  &__ram_code_end - &__ram_code_start;
  code_size += temp;
  Serial.println( temp );
  Serial.print( "bss\t\t" );
  temp =  &__bss_end - &__bss_start;
  Serial.println( temp );
  Serial.print( "Heap or unused\t" );
  temp =  &Heap_Bank1_End - &Heap_Bank1_Start;
  Serial.println( temp );

  Serial.print( "\nTotal Flash\t" );
  Serial.println( code_size );

  Serial.print( "Ram less Stack\t" );
  temp = &Heap_Bank1_Start - &__data_start;
  Serial.println( temp );
}

void loop()
{
  uint32_t ret = 0;

  String message = "\r\nWelcome to Trust X Bootcamp!";

  if(sys_init)
  {
    sys_test();
    Serial.println(message);
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
    }else
    {
      sys_init=1;
      }
  }
}

uint8_t reset()
{
  uint32_t ret = 0;

#if( UC_FAMILY == XMC1 )
  led1Off();
  led2Off();
#endif
  return ret;
}
