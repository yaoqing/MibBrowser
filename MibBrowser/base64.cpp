
#include "stdafx.h"
#include "base64.h"

static unsigned char b64_list[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define XX 100

static int b64_index[256] = {
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,62, XX,XX,XX,63,
    52,53,54,55, 56,57,58,59, 60,61,XX,XX, XX,XX,XX,XX,
    XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
    XX,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};

/* |b64_encode| encodes |val| in a printable base 64 format.  A MSB-first
   encoding is generated. */

const char *b64_encode( unsigned long val )
{
   static char   result[7];
   int    i;

   result[0] = b64_list[ (val & 0xc0000000) >> 30 ];
   result[1] = b64_list[ (val & 0x3f000000) >> 24 ];
   result[2] = b64_list[ (val & 0x00fc0000) >> 18 ];
   result[3] = b64_list[ (val & 0x0003f000) >> 12 ];
   result[4] = b64_list[ (val & 0x00000fc0) >>  6 ];
   result[5] = b64_list[ (val & 0x0000003f)       ];
   result[6] = 0;

   for (i = 0; i < 5; i++) if (result[i] != b64_list[0]) return result + i;
   return result + 5;
}

unsigned long b64_decode( const char *val )
{
   unsigned long v = 0;
   int           i;
   int           offset = 0;
   int           len = strlen( val );

   for (i = len - 1; i >= 0; i--) {
      int tmp = b64_index[ (unsigned char)val[i] ];

      v |= tmp << offset;
      offset += 6;
   }

   return v;
}
