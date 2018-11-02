#ifndef FPRINT_H
#define FPRINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h"

#define MAXCMD_LEN      255
#define HEXDUMP_COLS      16

#ifndef SUPPRESSHEXDUMP
#define SUPPRESSHEXDUMP   0
#else
#define SUPPRESSHEXDUMP   1
#endif
#define HEXDUMP(a, b)   (SUPPRESSHEXDUMP==0) ? __hexdump__(a,b) : (void) 0;

#define HEXONLYDUMP(a, b)   __hexonlydump__(a,b)

/**
 *
 * Printout data in a standard hex view
 *
 * @param[in] p_buf   Pointer to data which should be printed out.
 * @param[in] l_len   Length of a data
 *
 * @retval  None
 * @example  
 0x000000: 2e 2f 68 65 78 64 75 6d ./hexdum
 0x000008: 70 00 53 53 48 5f 41 47 p.SSH_AG
 0x000010: 45 4e 54 5f             ENT_
 */
inline void __hexdump__(const void* p_buf, uint32_t l_len) {
  unsigned int i, j;
  static char str[MAXCMD_LEN];
  for (i = 0; i < l_len + ((l_len % HEXDUMP_COLS) ?
          ( HEXDUMP_COLS - l_len % HEXDUMP_COLS) : 0);
      i++) {
    /* print offset */
    if (i % HEXDUMP_COLS == 0) {
      sprintf(str, "0x%06x: ", i);
      Serial.print(str);
    }

    /* print hex data */
    if (i < l_len) {
      sprintf(str, "%02x ", 0xFF & ((char*) p_buf)[i]);
      Serial.print(str);
    } else /* end of block, just aligning for ASCII dump */
    {
      sprintf(str, "   ");
      Serial.print(str);
    }

    /* print ASCII dump */
    if (i % HEXDUMP_COLS == ( HEXDUMP_COLS - 1)) {
      for (j = i - ( HEXDUMP_COLS - 1); j <= i; j++) {
        if (j >= l_len) /* end of block, not really printing */
        {
          Serial.print(' ');
        } else if (isprint((int) ((char*) p_buf)[j])) /* printable char */
        {
          Serial.print(((char*) p_buf)[j]);
        } else /* other char */
        {
          Serial.print('.');
        }
      }
      Serial.print('\r');
      Serial.print('\n');
      delay(10);
    }
  }
}

/**
 *
 * Printout data for easy cut and paste
 *
 * @param[in] p_buf   Pointer to data which should be printed out.
 * @param[in] l_len   Length of a data
 *
 * @retval  None
 * @example  
2e 2f 68 65 78 64 75 6d 
70 00 53 53 48 5f 41 47 
45 4e 54 5f             
 */
inline void __hexonlydump__(const void* p_buf, uint32_t l_len) {
  unsigned int i, j;
  static char str[MAXCMD_LEN];
  for (i = 0; i < l_len + ((l_len % HEXDUMP_COLS) ?
          ( HEXDUMP_COLS - l_len % HEXDUMP_COLS) : 0);
      i++) {
    /* print offset */
    if (i % HEXDUMP_COLS == 0) {
      //sprintf(str, "0x%06x: ", i);
      //Serial.print(str);
    }

    /* print hex data */
    if (i < l_len) {
      sprintf(str, "%02x ", 0xFF & ((char*) p_buf)[i]);
      Serial.print(str);
    } else /* end of block, just aligning for ASCII dump */
    {
      sprintf(str, "   ");
      Serial.print(str);
    }

    /* print ASCII dump */
    if (i % HEXDUMP_COLS == ( HEXDUMP_COLS - 1)) {
      for (j = i - ( HEXDUMP_COLS - 1); j <= i; j++) {
        if (j >= l_len) /* end of block, not really printing */
        {
          Serial.print(' ');
        } else if (isprint((int) ((char*) p_buf)[j])) /* printable char */
        {
          //Serial.print(((char*) p_buf)[j]);
        } else /* other char */
        {
          //Serial.print('.');
        }
      }
      Serial.print('\r');
      Serial.print('\n');
    }
    delay(10);
  }
}
#ifdef __cplusplus
}
#endif
#endif
