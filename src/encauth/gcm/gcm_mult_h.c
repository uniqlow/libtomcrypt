/* LibTomCrypt, modular cryptographic library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/**
   @file gcm_mult_h.c
   GCM implementation, do the GF mult, by Tom St Denis
*/
#include "tomcrypt_private.h"

#if defined(LTC_GCM_MODE)
/**
  GCM multiply by H
  @param gcm   The GCM state which holds the H value
  @param I     The value to multiply H by
 */
void gcm_mult_h(const gcm_state *gcm, unsigned char *I)
{
   unsigned char T[16];
#ifdef LTC_GCM_TABLES
   int x;
#ifdef LTC_GCM_TABLES_SSE2
   __asm__("movdqa (%0),%%xmm0"::"r"(&gcm->PC[0][I[0]][0]));
   for (x = 1; x < 16; x++) {
      __asm__("pxor (%0),%%xmm0"::"r"(&gcm->PC[x][I[x]][0]));
   }
   __asm__("movdqa %%xmm0,(%0)"::"r"(&T));
#else
   int y;
   XMEMCPY(T, &gcm->PC[0][I[0]][0], 16);
   for (x = 1; x < 16; x++) {
#ifdef LTC_FAST
       for (y = 0; y < 16; y += sizeof(LTC_FAST_TYPE)) {
           *(LTC_FAST_TYPE_PTR_CAST(T + y)) ^= *(LTC_FAST_TYPE_PTR_CAST(&gcm->PC[x][I[x]][y]));
       }
#else
       for (y = 0; y < 16; y++) {
           T[y] ^= gcm->PC[x][I[x]][y];
       }
#endif /* LTC_FAST */
   }
#endif /* LTC_GCM_TABLES_SSE2 */
#else
   gcm_gf_mult(gcm->H, I, T);
#endif
   XMEMCPY(I, T, 16);
}
#endif
