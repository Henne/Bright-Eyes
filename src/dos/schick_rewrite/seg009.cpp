/* Code from Heikki Orsila's amigadepack 0.02 to replace previous
 * PowerPack depacker with license issues.
 *
 * You'll probably want to use ppcrack stand-alone to crack encrypted
 * powerpack files once instead of using brute force at each replay.
 *
 * Modified for xmp by Claudio Matsuoka, 08/2007
 * - merged mld's checks from the old depack sources. Original credits:
 *   - corrupt file and data detection
 *     (thanks to Don Adan and Dirk Stoecker for help and infos)
 *   - implemeted "efficiency" checks
 *   - further detection based on code by Georg Hoermann
 */

#include "../schick.h"

static inline unsigned int val(const unsigned char *p) {
	return (p[0]<<16 | p[1] << 8 | p[2]);
}

static unsigned long depackedlen(const unsigned char *p, unsigned long plen) {
/*	DSA1/ROA1 doesn't use the first bytes as a signature "PP20".
 *	It's used instead for the lenght of the packed data. */

	if (p[0] == 'P' && p[1] == 'P' && p[2] == '2' && p[3] == '0')
		return val(p+plen-4);

	if (host_readd((Bit8u*)p) == plen)
		return val(p+plen-4);

	if (host_readd((Bit8u*)p) + 8 == plen)
		return val(p+plen-4);

	return 0; /* not a powerpacker file */
}

typedef unsigned char uint8;
typedef unsigned int uint32;

#define PP_READ_BITS(nbits, var) do {                          \
  bit_cnt = (nbits);                                           \
  while (bits_left < bit_cnt) {                                \
    if (buf_src < src) return 0; /* out of source bits */      \
    bit_buffer |= (*--buf_src << bits_left);                   \
    bits_left += 8;                                            \
  }                                                            \
  (var) = 0;                                                   \
  bits_left -= bit_cnt;                                        \
  while (bit_cnt--) {                                          \
    (var) = ((var) << 1) | (bit_buffer & 1);                   \
    bit_buffer >>= 1;                                          \
  }                                                            \
} while(0)

#define PP_BYTE_OUT(byte) do {                                 \
  if (out <= dest) return 0; /* output overflow */              \
  *--out = (byte);                                             \
  written++;                                                   \
} while (0)

int ppDecrunch(uint8 *src, uint8 *dest, uint8 *offset_lens,
               uint32 src_len, uint32 dest_len, uint8 skip_bits)
{
  uint8 *buf_src, *out, *dest_end, bits_left = 0, bit_cnt;
  uint32 bit_buffer = 0, x, todo, offbits, offset, written=0;

  if (src == NULL || dest == NULL || offset_lens == NULL) return 0;

  /* set up input and output pointers */
  buf_src = src + src_len;
  out = dest_end = dest + dest_len;

  /* skip the first few bits */
  PP_READ_BITS(skip_bits, x);

  /* while there are input bits left */
  while (written < dest_len) {
    PP_READ_BITS(1, x);
    if (x == 0) {
      /* 1bit==0: literal, then match. 1bit==1: just match */
      todo = 1; do { PP_READ_BITS(2, x); todo += x; } while (x == 3);
      while (todo--) { PP_READ_BITS(8, x); PP_BYTE_OUT(x); }

      /* should we end decoding on a literal, break out of the main loop */
      if (written == dest_len) break;
    }

    /* match: read 2 bits for initial offset bitlength / match length */
    PP_READ_BITS(2, x);
    offbits = offset_lens[x];
    todo = x+2;
    if (x == 3) {
      PP_READ_BITS(1, x);
      if (x==0) offbits = 7;
      PP_READ_BITS(offbits, offset);
      do { PP_READ_BITS(3, x); todo += x; } while (x == 7);
    }
    else {
      PP_READ_BITS(offbits, offset);
    }
    if ((out + offset) >= dest_end) return 0; /* match overflow */
    while (todo--) { x = out[offset]; PP_BYTE_OUT(x); }
  }

  /* all output bytes written without error */
  return 1;
  /* return (src == buf_src) ? 1 : 0; */
}

void decomp_pp20(Bit8u *src, Bit8u *dst, Bit8u *p3, unsigned int plen) {
	size_t unplen;

	if (plen < 4)
		D1_ERR("PP20: Length argument is below 4\n");

	unplen = depackedlen(src, plen);

	if (unplen == 0) {
		D1_ERR("PP20: No PP20 file\n");
	}
	ppDecrunch(&src[8],  dst, &src[4], plen - 12, unplen, src[plen -1]);
//	ppdepack(src, dst, plen, unplen);

	return;
}

