/*
 * Copyright (C) 2019 Siara Logics (cc)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @author Arundale R.
 *
 */
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned int word;

unsigned int  c_95[95] = {16384, 15552, 16128, 15520, 65408, 65472, 12160, 15456, 65152, 65280, 16064, 64896, 64256, 64000, 63488, 64512, 14720, 14848, 14976, 15040, 15104, 15168, 15232, 15296, 15328, 14592, 15744, 15616, 15872, 64768, 16000, 15424, 15360, 10752, 8640, 8320, 8448, 9728, 8992, 8672, 8512, 11264, 9056, 9040, 8192, 8608, 11520, 11008, 8576, 9048, 12032, 11776, 10240, 8544, 9024, 8688, 9064, 9008, 9072, 16320, 15568, 16352, 15488, 15536, 15608, 40960, 7168, 2048, 4096, 24576, 12800, 7680, 5120, 49152, 13824, 13568,   0, 6656, 53248, 45056, 6144, 13696, 61440, 57344, 32768, 5632, 13312, 7936, 13952, 13056, 14080, 16192, 15584, 16256, 15600};
unsigned char l_95[95] = {    3,    12,    10,    12,    10,    10,     9,    11,     9,     9,    10,     9,     8,     8,     7,     8,     9,     9,    10,    10,    10,    10,    10,    11,    11,     9,     9,     9,     9,     9,    10,    11,    10,     8,   11,   10,   10,    7,   12,   12,   11,     8,   13,   13,    9,   11,     8,     8,   11,   13,     9,     8,     7,   11,   12,   12,   13,   12,   13,    11,    12,    11,    11,    12,    13,     4,    7,    6,    6,     3,     8,    8,    7,     4,     9,     9,   5,    7,     4,     4,    7,     9,     5,     4,     3,    7,     8,    8,     9,     8,     9,    10,    12,    10,    13};
//unsigned char c[]    = {  ' ',   '!',   '"',   '#',   '$',   '%',   '&',  '\'',   '(',   ')',   '*',   '+',   ',',   '-',   '.',   '/',   '0',   '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   ':',   ';',   '<',   '=',   '>',   '?',   '@',   'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',   'I',  'J',  'K',  'L',  'M',   'N',   'O',  'P',  'Q',   'R',   'S',   'T',  'U',  'V',  'W',  'X',  'Y',  'Z',   '[',  '\\',   ']',   '^',   '_',   '`',   'a',  'b',  'c',  'd',   'e',   'f',  'g',  'h',   'i',   'j',   'k', 'l',  'm',   'n',   'o',  'p',   'q',   'r',   's',   't',  'u',   'v',  'w',   'x',   'y',   'z',   '{',   '|',   '}',   '~'};
char *SET2_STR = "9012345678.-,/=+ ()$%;:<>*\"{}[]@?'^#_!\\|~`";

enum {SHX_STATE_1 = 1, SHX_STATE_2};

unsigned int mask[] = {0x8000, 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00};
long append_bits(char *out, long ol, unsigned int code, int clen, byte state) {

   byte cur_bit;
   byte blen;
   unsigned char a_byte;

   if (state == SHX_STATE_2) {
      if ((code & 0xF800) == 0xF800) {
         code <<= 2;
         code &= 0x1FFF;
         clen -= 2;
      } else
      if ((code & 0x3C00) != 0x3C00) {
         code <<= 6;
         clen -= 6;
      }
      if (code == 8960 && clen == 11) {
         code = 12762;
         clen = 12;
      }
   }
   while (clen > 0) {
     cur_bit = ol%8;
     blen = (clen>8?8:clen);
     a_byte = (code & mask[blen-1])>>8;
     a_byte >>= cur_bit;
     if (blen+cur_bit > 8)
        blen = (8-cur_bit);
     if (cur_bit == 0)
        out[ol/8] = a_byte;
     else
        out[ol/8] |= a_byte;
     code <<= blen;
     ol += blen;
     clen -= blen;
   }
   return ol;
}

typedef struct {
  char term_rem_bits;
  char state;
  byte is_all_upper;
} C_CONTEXT;

long shox96_compress(char *in, long len, char *out, C_CONTEXT *c_ctx) {

  char *ptr;
  byte bits;
  byte state;

  long l, ll, ol;
  char c_in, c_next;
  byte is_upper, is_all_upper;

  ol = 0;
  if (c_ctx == NULL) {
     state = SHX_STATE_1;
     is_all_upper = 0;
  } else {
     state = c_ctx->state;
     is_all_upper = c_ctx->is_all_upper;
     if (c_ctx->term_rem_bits > 0) {
        ol = append_bits(out, ol, 12288<<(7-c_ctx->term_rem_bits), c_ctx->term_rem_bits, 1);
     }
  }
  for (l=0; l<len; l++) {
     c_in = in[l];
     c_next = 0;
     if (state == SHX_STATE_2) {
        if (c_in == ' ' && len-1>l)
           ptr = (char *) memchr(SET2_STR, in[l+1], 42);
        else
           ptr = (char *) memchr(SET2_STR, c_in, 42);
        if (ptr == NULL) {
           state = SHX_STATE_1;
           ol = append_bits(out, ol, 8192, 4, 1);
        }
     }
     is_upper = 0;
     if (c_in >= 'A' && c_in <= 'Z')
        is_upper = 1;
     else {
        if (is_all_upper) {
           is_all_upper = 0;
           ol = append_bits(out, ol, 8192, 4, state);
        }
     }
     if (is_upper && !is_all_upper) {
        for (ll=l+5; ll>=l && ll<len; ll--) {
            if (in[ll] >= 'a' && in[ll] <= 'z')
               break;
        }
        if (ll == l-1) {
           ol = append_bits(out, ol, 8704, 8, state);
           is_all_upper = 1;
        }
     }
     if (state == SHX_STATE_1 && c_in >= '0' && c_in <= '9') {
        ol = append_bits(out, ol, 14336, 6, state);
        state = SHX_STATE_2;
     }
     c_next = 0;
     if (l+1 < len)
        c_next = in[l+1];

     if (c_in >= 32 && c_in <= 126) {
        c_in -= 32;
        if (is_all_upper && is_upper)
           c_in += 32;
        if (c_in == 0 && state == SHX_STATE_2)
           ol = append_bits(out, ol, 65024, 9, state);
        else
           ol = append_bits(out, ol, c_95[c_in], l_95[c_in], state);
     } else
     if (c_in == 13 && c_next == 10) {
        ol = append_bits(out, ol, 3072, 6, state);
        l++;
     } else
     if (c_in == 10) {
        ol = append_bits(out, ol, 8384, 10, state);
     } else
     if (c_in == 13) {
        ol = append_bits(out, ol, 8960, 11, state);
     } else
     if (c_in == '\t') {
        ol = append_bits(out, ol, 9216, 7, state);
     }
  }
  bits = ol%8;
  if (bits >= 1 && bits <= 7) {
     ol = append_bits(out, ol, 12288, (bits>1?8-bits:7), 1);
  }
  if (c_ctx != NULL) {
     c_ctx->state = state;
     c_ctx->is_all_upper = is_all_upper;
     c_ctx->term_rem_bits = 0;
     if (bits > 1)
        c_ctx->term_rem_bits = (7-(8-bits));
  }
  //printf("\n%ld\n", ol);
  return ol/8+(ol%8?1:0);

}
byte get_pos3(byte val, byte bits) {
  switch (val) {
    case  1: if (bits == 2) return 1; break;
    case  3: if (bits == 3) return 2; break;
    case  7: if (bits == 4) return 3; break;
    case 15: if (bits == 4) return 4; break;
  }
  return 99;
}
byte get_pos5(byte val, byte bits) {
  switch (val) {
    case  0: if (bits == 2) return 0; break;
    case  2: if (bits == 3) return 1; break;
    case  6: if (bits == 3) return 2; break;
    case  1: if (bits == 3) return 3; break;
    case  5: if (bits == 4) return 4; break;
    case 13: if (bits == 4) return 5; break;
    case  3: if (bits == 4) return 6; break;
    case 11: if (bits == 4) return 7; break;
    case  7: if (bits == 4) return 8; break;
    case 15: if (bits == 5) return 9; break;
    case 31: if (bits == 5) return 10; break;
  }
  return 99;
}
byte get_bit_val(word w, byte bit_ctr, byte ctr) {
   word mask;
   mask = (1<<(15-bit_ctr));
   if (mask & w)
      mask = (1<<ctr);
   else
      mask = 0;
   return mask;
}

byte get_code(byte type, char *in, long len, long *pl, byte *bit, byte *is_last_byte, byte last_byte) {

   word w;
   byte is_last;
   byte bit_ctr;
   byte ctr, val;
   byte ret_code;

   is_last = 0;

   if (*pl == 0 && *is_last_byte) {
      w = last_byte<<8;
      w |= (0x00FF & in[0]);
   } else {
      w = ((word)in[*pl])<<8;
      if (len == *pl+1)
         is_last = 1;
      else
         w |= (0x00FF&in[*pl+1]);
   }

   val = get_bit_val(w, *bit, 0);
   if (type == 0 && val == 0) {
      *bit += 1;
      if (*bit > 7) {
         *bit = *bit%8;
         (*pl)++;
      }
      return 0;
   }
   for (ctr=1; ctr<5; ctr++) {
       bit_ctr = ctr+*bit;
       if (is_last && bit_ctr>7) {
          (*pl)++;
          return 99;
       }
       if (type == 0 && ctr > 3)
          return 99;
       val += get_bit_val(w, bit_ctr, ctr);
       if (type == 0)
          ret_code = get_pos3(val, ctr+1);
       else
          ret_code = get_pos5(val, ctr+1);
       if (ret_code != 99) {
          *bit = bit_ctr+1;
          if (*bit > 7) {
             *bit = *bit%8;
             if (*is_last_byte) {
                *is_last_byte = 0;
             } else {
                (*pl)++;
             }
          }
          break;
       }
   }
   // should never happen, but if it does, just skip bits to avoid endless processing
   if (ret_code == 99) {
      *bit = bit_ctr+1;
      if (*bit > 7) {
         *bit = *bit%8;
         (*pl)++;
      }
   }
   return ret_code;
}

#define st_out 0
#define st_sw 9
enum {SHX_SET1 = 1, SHX_SET1A, SHX_SET1B, SHX_SET2, SHX_SET2A, SHX_SET3, SHX_SET3A};

char *set1  = (char*) "  etaoinsr ";
char *set1a = (char*) "lc dhupmbgw";
char *set1b = (char*) " fyvkqjxz  ";
char *set2  = (char*) " 9012345678";
char *set2a = (char*) ".-,/=+ ()$%";
char *set3  = (char*) " ;:<>*\"{}[]";
char *set3a = (char*) "@?'^#_!\\|~`";

typedef struct {
  byte state;
  byte cur_bit;
  byte is_upper;
  byte is_switch;
  byte last_byte;
  byte prev_state;
  byte is_last_byte;
  byte is_all_upper;
  byte is_set2_sticky;
} D_CONTEXT;

long shox96_decompress(char *in, long len, char *out, D_CONTEXT *d_ctx) {

long l, ol;
byte code;
byte state;
byte cur_bit;
byte is_upper;
byte is_switch;
byte last_byte;
byte prev_state;
byte is_last_byte;
byte is_all_upper;
byte is_set2_sticky;

ol = 0;
if (d_ctx == NULL) {
  cur_bit = 0;
  is_upper = 0;
  is_switch = 0;
  last_byte = 0;
  state = SHX_SET1;
  is_last_byte = 0;
  is_all_upper = 0;
  is_set2_sticky = 0;
  prev_state = SHX_SET1;
} else {
  state = d_ctx->state;
  cur_bit = d_ctx->cur_bit;
  is_upper = d_ctx->is_upper;
  is_switch = d_ctx->is_switch;
  last_byte = d_ctx->last_byte;
  prev_state = d_ctx->prev_state;
  is_last_byte = d_ctx->is_last_byte;
  is_all_upper = d_ctx->is_all_upper;
  is_set2_sticky = d_ctx->is_set2_sticky;
}

for (l=0; l<len;) {
    if (is_switch) {
       code = get_code(0, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code == 0) {
          if (state == SHX_SET1)
             state = SHX_SET1A;
          else
          if (state == SHX_SET2)
             state = SHX_SET2A;
       } else
       if (code == 1) {
          if (state == SHX_SET1) {
             if (is_all_upper == 1)
                is_all_upper = 0;
             else
             if (is_upper == 0)
                is_upper = 1;
             else
                is_all_upper = 1;
          } else
          if (state == SHX_SET2) {
             state = SHX_SET1;
             is_set2_sticky = 0;
          } else
             state = SHX_SET1;
       } else
       if (code == 2) {
          prev_state = state;
          state = SHX_SET1B;
       } else
       if (code == 3) {
          if (state == SHX_SET2 && is_set2_sticky) {
             is_set2_sticky = 0;
             state = SHX_SET1;
          } else
          if (state == SHX_SET2) {
             is_set2_sticky = 0;
             state = SHX_SET2;
          } else {
             is_set2_sticky = 1;
             state = SHX_SET2;
          }
       } else
       if (code == 4) {
          prev_state = state;
          state = SHX_SET3;
       }
       if (code != 99)
          is_switch = 0;
       continue;
    }
    if (state == SHX_SET1) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code == 1) {
          if (is_upper) out[ol++] = '\t'; else out[ol++] = ' ';
          is_upper = 0;
       } else
       if (code >= 2 && code <= 9) {
          out[ol] = set1[code];
          if (is_upper || is_all_upper)
             out[ol] -= 32;
          if (is_upper) is_upper = 0;
          ol++;
       } else
       if (code == 0)
          is_switch = 1;
       else
       if (code == 10) {
          if (is_upper)
            out[ol++] = '&';
          else
            state = SHX_SET2A;
          is_upper = 0;
       }
    } else
    if (state == SHX_SET1A) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code == 2) {
          if (is_upper == 0) {
             out[ol] = '\r';
             ol++;
          } else
             is_upper = 0;
          out[ol++] = '\n';
       } else
       if (code != 99) {
          out[ol] = set1a[code];
          if (is_upper || is_all_upper)
             out[ol] -= 32;
          if (is_upper) is_upper = 0;
          ol++;
       }
       if (code != 99)
          state = SHX_SET1;
    } else
    if (state == SHX_SET1B) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code == 0) {
          if (is_upper) {
             out[ol++] = '\r';
             is_upper = 0;
          } else {
             if (cur_bit > 6) {
                cur_bit = 0;
                l++;
             }
          }
       } else
       if (code != 99) {
          out[ol] = set1b[code];
          if (is_upper || is_all_upper)
             out[ol] -= 32;
          if (is_upper) is_upper = 0;
          ol++;
       }
       if (code != 99)
          state = prev_state;
    } else
    if (state == SHX_SET2) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code == 0) {
          is_switch = 1;
       } else
       if (code != 99) {
          out[ol++] = set2[code];
          if (is_set2_sticky == 0)
             state = SHX_SET1;
       }
    } else
    if (state == SHX_SET2A) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code != 99) {
          out[ol++] = set2a[code];
          if (is_set2_sticky)
             state = SHX_SET2;
          else
             state = SHX_SET1;
       }
    } else
    if (state == SHX_SET3) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code == 0) {
          state = SHX_SET3A;
       } else
       if (code != 99) {
          out[ol++] = set3[code];
          state = prev_state;
       }
    } else
    if (state == SHX_SET3A) {
       code = get_code(1, in, len, &l, &cur_bit, &is_last_byte, last_byte);
       if (code != 99) {
          out[ol++] = set3a[code];
          state = prev_state;
       }
    }
}
if (d_ctx != NULL) {
  d_ctx->state = state;
  d_ctx->cur_bit = cur_bit;
  d_ctx->is_upper = is_upper;
  d_ctx->is_switch = is_switch;
  d_ctx->prev_state = prev_state;
  d_ctx->is_all_upper = is_all_upper;
  d_ctx->is_set2_sticky = is_set2_sticky;
  d_ctx->is_last_byte = 0;
  if (cur_bit > 0) {
     d_ctx->last_byte = in[len-1];
     d_ctx->is_last_byte = 1;
  }
}
return ol;

}

void print_compressed(char *in, long len) {

   long l;
   byte bit;
   for (l=0; l<len*8; l++) {
       bit = (in[l/8]>>(7-l%8))&0x01;
       printf("%d", bit);
       if (l%8 == 7) printf(" ");
   }

}

#ifndef ARDUINO
int main(int argv, char *args[]) {

char cbuf[6000];
char dbuf[5000];
long len, tot_len, clen, ctot, dlen, l;
float perc;
FILE *fp, *wfp;
int bytes_read;
C_CONTEXT c_ctx;
D_CONTEXT d_ctx;
char c_in;
time_t tStart;

tStart = time(NULL);

c_ctx.state = SHX_STATE_1;
c_ctx.is_all_upper = 0;
c_ctx.term_rem_bits = 0;

d_ctx.state = SHX_STATE_1;
d_ctx.cur_bit = 0;
d_ctx.is_upper = 0;
d_ctx.is_switch = 0;
d_ctx.last_byte = 0;
d_ctx.prev_state = 1;
d_ctx.is_last_byte = 0;
d_ctx.is_all_upper = 0;
d_ctx.is_set2_sticky = 0;

if (argv == 4 && strcmp(args[1], "c") == 0) {
   tot_len = 0;
   ctot = 0;
   fp = fopen(args[2], "rb");
   if (fp == NULL) {
      perror(args[2]);
      return 1;
   }
   wfp = fopen(args[3], "wb+");
   if (wfp == NULL) {
      perror(args[3]);
      return 1;
   }
   do {
     bytes_read = fread(cbuf, 1, sizeof(cbuf), fp);
     if (bytes_read > 0) {
        clen = shox96_compress(cbuf, bytes_read, dbuf, &c_ctx);
        ctot += clen;
        tot_len += bytes_read;
        if (clen > 0) {
           if (clen != fwrite(dbuf, 1, clen, wfp)) {
              perror("fwrite");
              return 1;
           }
        }
     }
   } while (bytes_read > 0);
} else
if (argv == 4 && strcmp(args[1], "d") == 0) {
   fp = fopen(args[2], "rb");
   if (fp == NULL) {
      perror(args[2]);
      return 1;
   }
   wfp = fopen(args[3], "wb+");
   if (wfp == NULL) {
      perror(args[3]);
      return 1;
   }
   do {
     memset(dbuf, 0, sizeof(dbuf));
     bytes_read = fread(dbuf, 1, sizeof(dbuf)/3, fp);
     if (bytes_read > 0) {
        dlen = shox96_decompress(dbuf, bytes_read, cbuf, &d_ctx);
        if (dlen > 0) {
           if (dlen != fwrite(cbuf, 1, dlen, wfp)) {
              perror("fwrite");
              return 1;
           }
        }
     }
   } while (bytes_read > 0);
} else
if (argv == 4 && strcmp(args[1], "g") == 0) {
   fp = fopen(args[2], "r");
   if (fp == NULL) {
      perror(args[2]);
      return 1;
   }
   wfp = fopen(args[3], "w+");
   if (wfp == NULL) {
      perror(args[3]);
      return 1;
   }
   tot_len = 0;
   ctot = 0;
   fputs("#ifndef __SHOX96_0_1_0_COMPRESSED__\n", wfp);
   fputs("#define __SHOX96_0_1_0_COMPRESSED__\n", wfp);
   while (fgets(cbuf, sizeof(cbuf), fp) != NULL) {
      len = strlen(cbuf);
      if (cbuf[len - 1] == '\n' || cbuf[len - 1] == '\r') {
         len--;
         cbuf[len] = 0;
      }
      if (len > 0) {
        clen = shox96_compress(cbuf, len, dbuf, &c_ctx);
        if (clen > 0) {
            perc = (len-clen);
            perc /= len;
            perc *= 100;
            printf("len: %ld/%ld=", clen, len);
            printf("%.2f %s\n", perc, cbuf);
            tot_len += len;
            ctot += clen;
        }
      }
   }
   perc = (tot_len-ctot);
   perc /= tot_len;
   perc *= 100;
   printf("\nlen: %ld/%ld=", ctot, tot_len);
   printf("%.2f\n", perc);
   fputs("#endif\n", wfp);
} else
if (argv == 2) {
   len = strlen(args[1]);
   memset(cbuf, 0, sizeof(cbuf));
   ctot = shox96_compress(args[1], len, cbuf, NULL);
   print_compressed(cbuf, ctot);
   memset(dbuf, 0, sizeof(dbuf));
   dlen = shox96_decompress(cbuf, ctot, dbuf, NULL);
   printf("\n%s\n", dbuf);
   perc = (len-ctot);
   perc /= len;
   perc *= 100;
   printf("\nlen: %ld/%ld=", ctot, len);
   printf("%.2f\n", perc);
} else {
   printf("Usage: shox96 \"string\"\n");
   printf("  (or) shox96 [c|d|g] in_file out_file\n");
   printf("  where c means compress, d means decompress\n");
   printf("    and g means generate header file (out_file[.h])\n");
   printf("                 from in_file[.txt] for Arduino\n");
   return 1;
}

printf("\nElapsed:%ld\n", (time(NULL)-tStart));

return 0;

}
#endif
