/* Copyright (C) 2002 Free Software Foundation, Inc.

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, if you include this header file into source
   files compiled by GCC, this header file does not by itself cause
   the resulting executable to be covered by the GNU General Public
   License.  This exception does not however invalidate any other
   reasons why the executable file might be covered by the GNU General
   Public License.  */

/* Implemented from the specification included in the Intel C++ Compiler
   User Guide and Reference, version 5.0.  */

#ifndef _MMINTRIN_H_INCLUDED
#define _MMINTRIN_H_INCLUDED

#ifndef __MMX__
# error "MMX instruction set not enabled"
#else
/* The data type intended for user use.  */
typedef int __m64 __attribute__ ((__mode__ (__V2SI__)));

/* Internal data types for implementing the intrinsics.  */
typedef int __v2si __attribute__ ((__mode__ (__V2SI__)));
typedef int __v4hi __attribute__ ((__mode__ (__V4HI__)));
typedef int __v8qi __attribute__ ((__mode__ (__V8QI__)));

/* Empty the multimedia state.  */
static __inline void
_mm_empty (void)
{
  __builtin_ia32_emms ();
}

/* Convert I to a __m64 object.  The integer is zero-extended to 64-bits.  */
static __inline __m64 
_mm_cvtsi32_si64 (int __i)
{
  long long __tmp = (unsigned int)__i;
  return (__m64) __tmp;
}

#ifdef __x86_64__
/* Convert I to a __m64 object.  */
static __inline __m64 
_mm_cvtsi64x_si64 (long long __i)
{
  return (__m64) __i;
}

/* Convert I to a __m64 object.  */
static __inline __m64 
_mm_set_pi64x (long long __i)
{
  return (__m64) __i;
}
#endif

/* Convert the lower 32 bits of the __m64 object into an integer.  */
static __inline int
_mm_cvtsi64_si32 (__m64 __i)
{
  long long __tmp = (long long)__i;
  return __tmp;
}

#ifdef __x86_64__
/* Convert the lower 32 bits of the __m64 object into an integer.  */
static __inline long long
_mm_cvtsi64_si64x (__m64 __i)
{
  return (long long)__i;
}
#endif

/* Pack the four 16-bit values from M1 into the lower four 8-bit values of
   the result, and the four 16-bit values from M2 into the upper four 8-bit
   values of the result, all with signed saturation.  */
static __inline __m64
_mm_packs_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_packsswb ((__v4hi)__m1, (__v4hi)__m2);
}

/* Pack the two 32-bit values from M1 in to the lower two 16-bit values of
   the result, and the two 32-bit values from M2 into the upper two 16-bit
   values of the result, all with signed saturation.  */
static __inline __m64
_mm_packs_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_packssdw ((__v2si)__m1, (__v2si)__m2);
}

/* Pack the four 16-bit values from M1 into the lower four 8-bit values of
   the result, and the four 16-bit values from M2 into the upper four 8-bit
   values of the result, all with unsigned saturation.  */
static __inline __m64
_mm_packs_pu16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_packuswb ((__v4hi)__m1, (__v4hi)__m2);
}

/* Interleave the four 8-bit values from the high half of M1 with the four
   8-bit values from the high half of M2.  */
static __inline __m64
_mm_unpackhi_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_punpckhbw ((__v8qi)__m1, (__v8qi)__m2);
}

/* Interleave the two 16-bit values from the high half of M1 with the two
   16-bit values from the high half of M2.  */
static __inline __m64
_mm_unpackhi_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_punpckhwd ((__v4hi)__m1, (__v4hi)__m2);
}

/* Interleave the 32-bit value from the high half of M1 with the 32-bit
   value from the high half of M2.  */
static __inline __m64
_mm_unpackhi_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_punpckhdq ((__v2si)__m1, (__v2si)__m2);
}

/* Interleave the four 8-bit values from the low half of M1 with the four
   8-bit values from the low half of M2.  */
static __inline __m64
_mm_unpacklo_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_punpcklbw ((__v8qi)__m1, (__v8qi)__m2);
}

/* Interleave the two 16-bit values from the low half of M1 with the two
   16-bit values from the low half of M2.  */
static __inline __m64
_mm_unpacklo_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_punpcklwd ((__v4hi)__m1, (__v4hi)__m2);
}

/* Interleave the 32-bit value from the low half of M1 with the 32-bit
   value from the low half of M2.  */
static __inline __m64
_mm_unpacklo_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_punpckldq ((__v2si)__m1, (__v2si)__m2);
}

/* Add the 8-bit values in M1 to the 8-bit values in M2.  */
static __inline __m64
_mm_add_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Add the 16-bit values in M1 to the 16-bit values in M2.  */
static __inline __m64
_mm_add_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Add the 32-bit values in M1 to the 32-bit values in M2.  */
static __inline __m64
_mm_add_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddd ((__v2si)__m1, (__v2si)__m2);
}

/* Add the 64-bit values in M1 to the 64-bit values in M2.  */
static __inline __m64
_mm_add_si64 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddq ((long long)__m1, (long long)__m2);
}

/* Add the 8-bit values in M1 to the 8-bit values in M2 using signed
   saturated arithmetic.  */
static __inline __m64
_mm_adds_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddsb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Add the 16-bit values in M1 to the 16-bit values in M2 using signed
   saturated arithmetic.  */
static __inline __m64
_mm_adds_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddsw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Add the 8-bit values in M1 to the 8-bit values in M2 using unsigned
   saturated arithmetic.  */
static __inline __m64
_mm_adds_pu8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddusb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Add the 16-bit values in M1 to the 16-bit values in M2 using unsigned
   saturated arithmetic.  */
static __inline __m64
_mm_adds_pu16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_paddusw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Subtract the 8-bit values in M2 from the 8-bit values in M1.  */
static __inline __m64
_mm_sub_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Subtract the 16-bit values in M2 from the 16-bit values in M1.  */
static __inline __m64
_mm_sub_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Subtract the 32-bit values in M2 from the 32-bit values in M1.  */
static __inline __m64
_mm_sub_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubd ((__v2si)__m1, (__v2si)__m2);
}

/* Add the 64-bit values in M1 to the 64-bit values in M2.  */
static __inline __m64
_mm_sub_si64 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubq ((long long)__m1, (long long)__m2);
}

/* Subtract the 8-bit values in M2 from the 8-bit values in M1 using signed
   saturating arithmetic.  */
static __inline __m64
_mm_subs_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubsb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Subtract the 16-bit values in M2 from the 16-bit values in M1 using
   signed saturating arithmetic.  */
static __inline __m64
_mm_subs_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubsw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Subtract the 8-bit values in M2 from the 8-bit values in M1 using
   unsigned saturating arithmetic.  */
static __inline __m64
_mm_subs_pu8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubusb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Subtract the 16-bit values in M2 from the 16-bit values in M1 using
   unsigned saturating arithmetic.  */
static __inline __m64
_mm_subs_pu16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_psubusw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Multiply four 16-bit values in M1 by four 16-bit values in M2 producing
   four 32-bit intermediate results, which are then summed by pairs to
   produce two 32-bit results.  */
static __inline __m64
_mm_madd_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pmaddwd ((__v4hi)__m1, (__v4hi)__m2);
}

/* Multiply four signed 16-bit values in M1 by four signed 16-bit values in
   M2 and produce the high 16 bits of the 32-bit results.  */
static __inline __m64
_mm_mulhi_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pmulhw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Multiply four 16-bit values in M1 by four 16-bit values in M2 and produce
   the low 16 bits of the results.  */
static __inline __m64
_mm_mullo_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pmullw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Shift four 16-bit values in M left by COUNT.  */
static __inline __m64
_mm_sll_pi16 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psllw ((__v4hi)__m, (long long)__count);
}

static __inline __m64
_mm_slli_pi16 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psllw ((__v4hi)__m, __count);
}

/* Shift two 32-bit values in M left by COUNT.  */
static __inline __m64
_mm_sll_pi32 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_pslld ((__v2si)__m, (long long)__count);
}

static __inline __m64
_mm_slli_pi32 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_pslld ((__v2si)__m, __count);
}

/* Shift the 64-bit value in M left by COUNT.  */
static __inline __m64
_mm_sll_si64 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psllq ((long long)__m, (long long)__count);
}

static __inline __m64
_mm_slli_si64 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psllq ((long long)__m, (long long)__count);
}

/* Shift four 16-bit values in M right by COUNT; shift in the sign bit.  */
static __inline __m64
_mm_sra_pi16 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psraw ((__v4hi)__m, (long long)__count);
}

static __inline __m64
_mm_srai_pi16 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psraw ((__v4hi)__m, __count);
}

/* Shift two 32-bit values in M right by COUNT; shift in the sign bit.  */
static __inline __m64
_mm_sra_pi32 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psrad ((__v2si)__m, (long long)__count);
}

static __inline __m64
_mm_srai_pi32 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psrad ((__v2si)__m, __count);
}

/* Shift four 16-bit values in M right by COUNT; shift in zeros.  */
static __inline __m64
_mm_srl_pi16 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psrlw ((__v4hi)__m, (long long)__count);
}

static __inline __m64
_mm_srli_pi16 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psrlw ((__v4hi)__m, __count);
}

/* Shift two 32-bit values in M right by COUNT; shift in zeros.  */
static __inline __m64
_mm_srl_pi32 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psrld ((__v2si)__m, (long long)__count);
}

static __inline __m64
_mm_srli_pi32 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psrld ((__v2si)__m, __count);
}

/* Shift the 64-bit value in M left by COUNT; shift in zeros.  */
static __inline __m64
_mm_srl_si64 (__m64 __m, __m64 __count)
{
  return (__m64) __builtin_ia32_psrlq ((long long)__m, (long long)__count);
}

static __inline __m64
_mm_srli_si64 (__m64 __m, int __count)
{
  return (__m64) __builtin_ia32_psrlq ((long long)__m, (long long)__count);
}

/* Bit-wise AND the 64-bit values in M1 and M2.  */
static __inline __m64
_mm_and_si64 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pand ((long long)__m1, (long long)__m2);
}

/* Bit-wise complement the 64-bit value in M1 and bit-wise AND it with the
   64-bit value in M2.  */
static __inline __m64
_mm_andnot_si64 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pandn ((long long)__m1, (long long)__m2);
}

/* Bit-wise inclusive OR the 64-bit values in M1 and M2.  */
static __inline __m64
_mm_or_si64 (__m64 __m1, __m64 __m2)
{
  return (__m64)__builtin_ia32_por ((long long)__m1, (long long)__m2);
}

/* Bit-wise exclusive OR the 64-bit values in M1 and M2.  */
static __inline __m64
_mm_xor_si64 (__m64 __m1, __m64 __m2)
{
  return (__m64)__builtin_ia32_pxor ((long long)__m1, (long long)__m2);
}

/* Compare eight 8-bit values.  The result of the comparison is 0xFF if the
   test is true and zero if false.  */
static __inline __m64
_mm_cmpeq_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pcmpeqb ((__v8qi)__m1, (__v8qi)__m2);
}

static __inline __m64
_mm_cmpgt_pi8 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pcmpgtb ((__v8qi)__m1, (__v8qi)__m2);
}

/* Compare four 16-bit values.  The result of the comparison is 0xFFFF if
   the test is true and zero if false.  */
static __inline __m64
_mm_cmpeq_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pcmpeqw ((__v4hi)__m1, (__v4hi)__m2);
}

static __inline __m64
_mm_cmpgt_pi16 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pcmpgtw ((__v4hi)__m1, (__v4hi)__m2);
}

/* Compare two 32-bit values.  The result of the comparison is 0xFFFFFFFF if
   the test is true and zero if false.  */
static __inline __m64
_mm_cmpeq_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pcmpeqd ((__v2si)__m1, (__v2si)__m2);
}

static __inline __m64
_mm_cmpgt_pi32 (__m64 __m1, __m64 __m2)
{
  return (__m64) __builtin_ia32_pcmpgtd ((__v2si)__m1, (__v2si)__m2);
}

/* Creates a 64-bit zero.  */
static __inline __m64
_mm_setzero_si64 (void)
{
  return (__m64)__builtin_ia32_mmx_zero ();
}

/* Creates a vector of two 32-bit values; I0 is least significant.  */
static __inline __m64
_mm_set_pi32 (int __i1, int __i0)
{
  union {
    __m64 __q;
    struct {
      unsigned int __i0;
      unsigned int __i1;
    } __s;
  } __u;

  __u.__s.__i0 = __i0;
  __u.__s.__i1 = __i1;

  return __u.__q;
}

/* Creates a vector of four 16-bit values; W0 is least significant.  */
static __inline __m64
_mm_set_pi16 (short __w3, short __w2, short __w1, short __w0)
{
  unsigned int __i1 = (unsigned short)__w3 << 16 | (unsigned short)__w2;
  unsigned int __i0 = (unsigned short)__w1 << 16 | (unsigned short)__w0;
  return _mm_set_pi32 (__i1, __i0);
		       
}

/* Creates a vector of eight 8-bit values; B0 is least significant.  */
static __inline __m64
_mm_set_pi8 (char __b7, char __b6, char __b5, char __b4,
	     char __b3, char __b2, char __b1, char __b0)
{
  unsigned int __i1, __i0;

  __i1 = (unsigned char)__b7;
  __i1 = __i1 << 8 | (unsigned char)__b6;
  __i1 = __i1 << 8 | (unsigned char)__b5;
  __i1 = __i1 << 8 | (unsigned char)__b4;

  __i0 = (unsigned char)__b3;
  __i0 = __i0 << 8 | (unsigned char)__b2;
  __i0 = __i0 << 8 | (unsigned char)__b1;
  __i0 = __i0 << 8 | (unsigned char)__b0;

  return _mm_set_pi32 (__i1, __i0);
}

/* Similar, but with the arguments in reverse order.  */
static __inline __m64
_mm_setr_pi32 (int __i0, int __i1)
{
  return _mm_set_pi32 (__i1, __i0);
}

static __inline __m64
_mm_setr_pi16 (short __w0, short __w1, short __w2, short __w3)
{
  return _mm_set_pi16 (__w3, __w2, __w1, __w0);
}

static __inline __m64
_mm_setr_pi8 (char __b0, char __b1, char __b2, char __b3,
	      char __b4, char __b5, char __b6, char __b7)
{
  return _mm_set_pi8 (__b7, __b6, __b5, __b4, __b3, __b2, __b1, __b0);
}

/* Creates a vector of two 32-bit values, both elements containing I.  */
static __inline __m64
_mm_set1_pi32 (int __i)
{
  return _mm_set_pi32 (__i, __i);
}

/* Creates a vector of four 16-bit values, all elements containing W.  */
static __inline __m64
_mm_set1_pi16 (short __w)
{
  unsigned int __i = (unsigned short)__w << 16 | (unsigned short)__w;
  return _mm_set1_pi32 (__i);
}

/* Creates a vector of four 16-bit values, all elements containing B.  */
static __inline __m64
_mm_set1_pi8 (char __b)
{
  unsigned int __w = (unsigned char)__b << 8 | (unsigned char)__b;
  unsigned int __i = __w << 16 | __w;
  return _mm_set1_pi32 (__i);
}

#endif /* __MMX__ */
#endif /* _MMINTRIN_H_INCLUDED */
