/* opencl_lift.h
 *
 * Copyright (c) 2011-2012 Brown Deer Technology, LLC.  All Rights Reserved.
 *
 * This software was developed by Brown Deer Technology, LLC.
 * For more information contact info@browndeertechnology.com
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3 (LGPLv3)
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* DAR */

#ifndef _opencl_lift_h
#define _opencl_lift_h

#include <stdio.h>
#include <float.h>

#define GCC_VERSION \
	( __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ )

/* XXX tell compiler, yes, I really meant inline when I said inline -DAR */
#ifndef __always_inline
#define __always_inline __inline __attribute__((always_inline))
#endif

//#define USE_SSE __SSE__

#if defined(__GNUC__) 

/***
 *** included if compiling opencl kernel as c++ with gcc
 ***/

#if !defined(__cplusplus)
#error must be compiled as c++
#endif

#undef _FORTIFY_SOURCE

#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <setjmp.h>
#include <math.h>

#if defined(USE_SSE) 
#include <xmmintrin.h>
#endif


/***
 *** vcore defines and structs 
 ***/

//#define __vc_setjmp  setjmp
//#define __vc_longjmp longjmp
//#define __vc_jmp_buf jmp_buf

//#define VCORE_NC           64       /* number of vcores per engine   */
//#define VCORE_STACK_SZ     16384    /* stack size per vcore          */
//#define VCORE_LOCAL_MEM_SZ 32768 /* local mem size per engine     */

//#define VCORE_STACK_MASK (~(VCORE_STACK_SZ-1))
//#define __fp() __builtin_frame_address(0)
//#define __get_thr_data() (struct thr_data*)(((intptr_t)__fp())&VCORE_STACK_MASK)

/*
#if defined(__x86_64__)
#define __setvcdata(pdata) __asm__ __volatile__ ( \
   "movq %%rbp,%%r14\n\t" \
   "andq $-16384,%%r14\n\t" \
   "movq %%r14,%0\n" \
   : "=m" (pdata) : : "%r14" \
   )
#elif defined(__arm__)
#define __setvcdata(pdata) __asm__ __volatile__ ( \
   "mov %%r3,%%fp\n\t" \
   "bic %%r3,%%r3,#16320\n\t" \
   "bic %%r3,%%r3,#63\n\t" \
   "str %%r3,%0\n" \
   : "=m" (pdata) : : "%r3" \
   )
#else
#error unsupported architecture
#endif
*/

/*
struct work_struct {
   unsigned int tdim;
   size_t ltsz[3];
   size_t gsz[3];
   size_t gtsz[3];
   size_t gid[3];
   size_t gtid[3];
};

struct vc_data {
   int vcid;
   __vc_jmp_buf* vcengine_jbufp;
   __vc_jmp_buf* this_jbufp;
   __vc_jmp_buf* next_jbufp;
   struct work_struct* workp;
   size_t ltid[3];
};
*/

/*
struct workp_entry {
   unsigned int ndr_dim;
   unsigned int ndr_gtdoff[3];
   unsigned int ndr_gtdsz[3];
   unsigned int ndr_ltdsz[3];
   unsigned int ndp_blk_first[3];
   unsigned int ndp_blk_end[3];
   unsigned int ndp_ltd0[3];
};

struct vc_data {
   int vcid;
   __vc_jmp_buf* vcengine_jbufp;
   __vc_jmp_buf* this_jbufp;
   __vc_jmp_buf* next_jbufp;
   struct workp_entry* we;
   size_t blkidx[3];
   size_t gtdidx[3];
   size_t ltdidx[3];
};
*/


#if defined(__FreeBSD__)
typedef unsigned int uint;
#endif

typedef unsigned int uint;
typedef unsigned char uchar;

extern "C" {

/*
static __inline unsigned int get_global_id(unsigned int d) 
{
   struct vc_data* data = __get_thr_data();
   return((unsigned int)data->ltid[d] + data->workp->gtid[d]);
}

static __inline uint get_work_dim() 
{ return((__get_thr_data())->workp->tdim); }

static __inline size_t get_local_size(uint d) 
{ return((__get_thr_data())->workp->ltsz[d]); }

static __inline size_t get_local_id(uint d) 
{ return((__get_thr_data())->ltid[d]); }

static __inline size_t get_num_groups(uint d) 
{ return((__get_thr_data())->workp->gsz[d]); }

static __inline size_t get_global_size(uint d) 
{ return((__get_thr_data())->workp->gtsz[d]); }

static __inline size_t get_group_id(uint d) 
{ return((__get_thr_data())->workp->gid[d]); }

*/

/*
static __inline uint get_work_dim()
{ return((__get_thr_data())->we->ndr_dim); }

static __inline size_t get_num_groups(uint d)
{ 
	size_t g = (__get_thr_data())->we->ndr_gtdsz[d];
	size_t l = (__get_thr_data())->we->ndr_ltdsz[d];
	return(g/l);
}

static __inline size_t get_group_id(uint d)
{ return((__get_thr_data())->blkidx[d]); }

static __inline size_t get_global_size(uint d)
{ return((__get_thr_data())->we->ndr_gtdsz[d]); }

static __inline unsigned int get_global_id(unsigned int d)
{ return((unsigned int)(__get_thr_data())->gtdidx[d]); }

static __inline size_t get_local_size(uint d)
{ return((__get_thr_data())->we->ndr_ltdsz[d]); }

static __inline size_t get_local_id(uint d)
{ return((__get_thr_data())->ltdidx[d]); }
*/


//static __inline void barrier( int flags )
//{
//   struct vc_data* data;
//   __setvcdata(data);
//   if (!(__vc_setjmp(*(data->this_jbufp))))
//      __vc_longjmp(*(data->next_jbufp),flags);
//}
//#define barrier(a)
//void barrier( int flags );

/*
#define barrier(flags) do { \
   struct thr_data* data = __get_thr_data();\
   if (!(setjmp(*(data->this_jbufp))))\
      longjmp(*(data->next_jbufp),flags);\
} while(0)
*/


}

#define CLK_LOCAL_MEM_FENCE 1


/*** adress space qualifiers [6.5] ***/

#define __global
#define __local
#define __constant
#define __private

#define global __global
#define local __local
#define constant __constant
#define private __private



/*** image access qualifiers [6.6] ***/

#define __read_only
#define __write_only


/*** function qualifiers [6.7] ***/

#define __kernel


/*** fix restrict qualifier for GCC ***/
#define restrict __restrict


/*** builtin vector data types [6.1.2] ***/

typedef char __char2 __attribute__((vector_size(2)));
typedef union {
	typedef char type_t;
   __char2 vec;
   struct { char x,y; };
   struct { char s0,s1; };
   char s[2];
} _char2;
typedef _char2 char2;
#define __vector_char2(a,b) (__char2){a,b}
#define _vector_char2(a,b) (_char2){(__char2){a,b}}
#define vector_char2(a,b) _vector_char2(a,b)

typedef unsigned char __uchar2 __attribute__((vector_size(2)));
typedef union {
	typedef unsigned char type_t;
   __uchar2 vec;
   struct { unsigned char x,y; };
   struct { unsigned char s0,s1; };
   unsigned char s[2];
} _uchar2;
typedef _uchar2 uchar2;
#define __vector_uchar2(a,b) (__uchar2){a,b}
#define _vector_uchar2(a,b) (_uchar2){(__uchar2){a,b}}
#define vector_uchar2(a,b) _vector_uchar2(a,b)

#if __cplusplus
extern "C" {
#endif

typedef int __int2 __attribute__((vector_size(8)));
typedef union {
	typedef int type_t;
   __int2 vec;
   struct { int x,y; };
   struct { int s0,s1; };
   int s[2];
} _int2;
typedef _int2 int2;
#define __vector_int2(a,b) (__int2){a,b}
#define _vector_int2(a,b) (_int2){(__int2){a,b}}
#define vector_int2(a,b) _vector_int2(a,b)

#if __cplusplus
}
#endif

typedef unsigned int __uint2 __attribute__((vector_size(8)));
typedef union {
	typedef unsigned int type_t;
   __uint2 vec;
   struct { unsigned int x,y; };
   struct { unsigned int s0,s1; };
   unsigned int s[2];
} _uint2;
typedef _uint2 uint2;
#define __vector_uint2(a,b) (__uint2){a,b}
#define _vector_uint2(a,b) (_uint2){(__uint2){a,b}}
#define vector_uint2(a,b) _vector_uint2(a,b)

typedef long long __long2 __attribute__((vector_size(16)));
typedef union {
	typedef long long type_t;
   __long2 vec;
   struct { long long x,y; };
   struct { long long s0,s1; };
   long long s[2];
} _long2;
typedef _long2 long2;
#define __vector_long2(a,b) (__long2){a,b}
#define _vector_long2(a,b) (_long2){(__long2){a,b}}
#define vector_long2(a,b) _vector_long2(a,b)

typedef unsigned long long __ulong2 __attribute__((vector_size(16)));
typedef union {
	typedef unsigned long long type_t;
   __ulong2 vec;
   struct { unsigned long long x,y; };
   struct { unsigned long long s0,s1; };
   unsigned long long s[2];
} _ulong2;
typedef _ulong2 ulong2;
#define __vector_ulong2(a,b) (__ulong2){a,b}
#define _vector_ulong2(a,b) (_ulong2){(__ulong2){a,b}}
#define vector_ulong2(a,b) _vector_ulong2(a,b)

typedef float __float2 __attribute__((vector_size(8)));
typedef union {
	typedef float type_t;
   __float2 vec;
   struct { float x,y; };
   struct { float s0,s1; };
   float s[2];
} _float2;
typedef _float2 float2;
#define __vector_float2(a,b) (__float2){a,b}
#define _vector_float2(a,b) (_float2){(__float2){a,b}}
#define vector_float2(a,b) _vector_float2(a,b)

typedef double __double2 __attribute__((vector_size(16)));
typedef union {
	typedef double type_t;
   __double2 vec;
   struct { double x,y; };
   struct { double s0,s1; };
   double s[2];
} _double2;
typedef _double2 double2;
#define __vector_double2(a,b) (__double2){a,b}
#define _vector_double2(a,b) (_double2){(__double2){a,b}}
#define vector_double2(a,b) _vector_double2(a,b)

typedef char __char4 __attribute__((vector_size(4)));
typedef union {
	typedef char type_t;
   __char4 vec;
   struct { char x,y,z,w; };
   struct { char s0,s1,s2,s3; };
   struct { char2 xy,zw; };
   struct { char2 s01,s23; };
   char s[4];
} _char4;
typedef _char4 char4;
#define __vector_char4(a,b,c,d) (__char4){a,b,c,d}
#define _vector_char4(a,b,c,d) (_char4){(__char4){a,b,c,d}}
#define vector_char4(a,b,c,d) _vector_char4(a,b,c,d)

typedef unsigned char __uchar4 __attribute__((vector_size(4)));
typedef union {
	typedef unsigned char type_t;
   __uchar4 vec;
   struct { unsigned char x,y,z,w; };
   struct { unsigned char s0,s1,s2,s3; };
   struct { uchar2 xy,zw; };
   struct { uchar2 s01,s23; };
   char s[4];
} _uchar4;
typedef _uchar4 uchar4;
#define __vector_uchar4(a,b,c,d) (__uchar4){a,b,c,d}
#define _vector_uchar4(a,b,c,d) (_uchar4){(__uchar4){a,b,c,d}}
#define vector_uchar4(a,b,c,d) _vector_uchar4(a,b,c,d)

typedef int __int4 __attribute__((vector_size(16)));
typedef union {
	typedef int type_t;
   __int4 vec;
   struct { int x,y,z,w; };
   struct { int s0,s1,s2,s3; };
   struct { int2 xy,zw; };
   struct { int2 s01,s23; };
   int s[4];
} _int4;
typedef _int4 int4;
#define __vector_int4(a,b,c,d) (__int4){a,b,c,d}
#define _vector_int4(a,b,c,d) (_int4){(__int4){a,b,c,d}}
#define vector_int4(a,b,c,d) _vector_int4(a,b,c,d)

typedef unsigned int __uint4 __attribute__((vector_size(16)));
typedef union {
	typedef unsigned int type_t;
   __uint4 vec;
   struct { unsigned int x,y,z,w; };
   struct { unsigned int s0,s1,s2,s3; };
   struct { uint2 xy,zw; };
   struct { uint2 s01,s23; };
   unsigned int s[4];
} _uint4;
typedef _uint4 uint4;
#define __vector_uint4(a,b,c,d) (__uint4){a,b,c,d}
#define _vector_uint4(a,b,c,d) (_uint4){(__uint4){a,b,c,d}}
#define vector_uint4(a,b,c,d) _vector_uint4(a,b,c,d)

typedef float __float4 __attribute__((vector_size(16)));
typedef union {
	typedef float type_t;
   __float4 vec;
   struct { float x,y,z,w; };
   struct { float s0,s1,s2,s3; };
   struct { float2 xy,zw; };
   struct { float2 s01,s23; };
   float s[4];
} _float4;
typedef _float4 float4;
#define __vector_float4(a,b,c,d) (__float4){a,b,c,d}
#define _vector_float4(a,b,c,d) (_float4){(__float4){a,b,c,d}}
#define vector_float4(a,b,c,d) _vector_float4(a,b,c,d)

#define GENERIC_UNARY_PLUS(type) \
static __always_inline _##type operator + ( _##type& a ); \
static __always_inline _##type operator + ( _##type& a ) { return a; }

#define GENERIC_UNARY_OP(type,op) \
static __always_inline _##type operator - ( _##type& a ); \
static __always_inline _##type operator - ( _##type& a ) \
	{ return (_##type){ -a.vec }; }

#define EXPLICIT_UNARY_OP_VEC2(type,op) \
static __always_inline _##type operator op ( _##type& a ); \
static __always_inline _##type operator op ( _##type& a ) \
	{ \
		_##type tmp \
			= _vector_##type( (type::type_t)op a.s0,(type::type_t)op a.s1 ); \
		return tmp; \
	}

#define EXPLICIT_UNARY_OP_VEC4(type,op) \
static __always_inline _##type operator op ( _##type& a ); \
static __always_inline _##type operator op ( _##type& a ) \
	{ \
		_##type tmp = _vector_##type( (type::type_t)op a.s0, \
			(type::type_t)op a.s1,(type::type_t)op a.s2,(type::type_t)op a.s3); \
		return tmp; \
	}

#define GENERIC_OP_ASSIGN(type,op) \
static __always_inline _##type \
	operator op##= ( _##type& lhs, const _##type rhs ); \
static __always_inline _##type \
	operator op##= ( _##type& lhs, const _##type rhs ) \
	{ lhs.vec op##= rhs.vec; return lhs; } 

#define EXPLICIT_OP_ASSIGN_VEC2(type,op) \
static __always_inline _##type \
	operator op##= ( _##type& lhs, const _##type rhs ); \
static __always_inline _##type \
	operator op##= ( _##type& lhs, const _##type rhs ) \
	{  \
		lhs.vec = __vector_##type( lhs.s0 op rhs.s0, lhs.s1 op rhs.s1 );  \
		return lhs;  \
	}

#define EXPLICIT_OP_ASSIGN_VEC4(type,op) \
static __always_inline _##type \
	operator op##= ( _##type& lhs, const _##type rhs ); \
static __always_inline _##type \
	operator op##= ( _##type& lhs, const _##type rhs ) \
	{  \
		lhs.vec = __vector_##type( lhs.s0 op rhs.s0, lhs.s1 op rhs.s1,  \
			lhs.s2 op rhs.s2, lhs.s3 op rhs.s3 );  \
		return lhs;  \
	}


/* unary plus */
GENERIC_UNARY_PLUS(char2)
GENERIC_UNARY_PLUS(uchar2)
GENERIC_UNARY_PLUS(int2)
GENERIC_UNARY_PLUS(uint2)
GENERIC_UNARY_PLUS(long2)
GENERIC_UNARY_PLUS(ulong2)
GENERIC_UNARY_PLUS(float2)
GENERIC_UNARY_PLUS(double2)
GENERIC_UNARY_PLUS(char4)
GENERIC_UNARY_PLUS(uchar4)
GENERIC_UNARY_PLUS(int4)
GENERIC_UNARY_PLUS(uint4)
GENERIC_UNARY_PLUS(float4)


/* unary minus */
#if GCC_VERSION < 40600
EXPLICIT_UNARY_OP_VEC2(char2,-)
EXPLICIT_UNARY_OP_VEC2(uchar2,-)
EXPLICIT_UNARY_OP_VEC2(int2,-)
EXPLICIT_UNARY_OP_VEC2(uint2,-)
EXPLICIT_UNARY_OP_VEC2(long2,-)
EXPLICIT_UNARY_OP_VEC2(ulong2,-)
EXPLICIT_UNARY_OP_VEC2(float2,-)
EXPLICIT_UNARY_OP_VEC2(double2,-)
EXPLICIT_UNARY_OP_VEC4(char4,-)
EXPLICIT_UNARY_OP_VEC4(uchar4,-)
EXPLICIT_UNARY_OP_VEC4(int4,-)
EXPLICIT_UNARY_OP_VEC4(uint4,-)
EXPLICIT_UNARY_OP_VEC4(float4,-)
#else
GENERIC_UNARY_OP(char2,-)
GENERIC_UNARY_OP(uchar2,-)
GENERIC_UNARY_OP(int2,-)
GENERIC_UNARY_OP(uint2,-)
GENERIC_UNARY_OP(long2,-)
GENERIC_UNARY_OP(ulong2,-)
GENERIC_UNARY_OP(float2,-)
GENERIC_UNARY_OP(double2,-)
GENERIC_UNARY_OP(char4,-)
GENERIC_UNARY_OP(uchar4,-)
GENERIC_UNARY_OP(int4,-)
GENERIC_UNARY_OP(uint4,-)
GENERIC_UNARY_OP(float4,-)
#endif

/* unary bitwise not */
EXPLICIT_UNARY_OP_VEC2(char2,~)
EXPLICIT_UNARY_OP_VEC2(uchar2,~)
EXPLICIT_UNARY_OP_VEC2(int2,~)
EXPLICIT_UNARY_OP_VEC2(uint2,~)
EXPLICIT_UNARY_OP_VEC2(long2,~)
EXPLICIT_UNARY_OP_VEC2(ulong2,~)
EXPLICIT_UNARY_OP_VEC4(char4,~)
EXPLICIT_UNARY_OP_VEC4(uchar4,~)
EXPLICIT_UNARY_OP_VEC4(int4,~)
EXPLICIT_UNARY_OP_VEC4(uint4,~)


/* add assign */
#if GCC_VERSION < 40600
EXPLICIT_OP_ASSIGN_VEC2(char2,+)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,+)
EXPLICIT_OP_ASSIGN_VEC2(int2,+)
EXPLICIT_OP_ASSIGN_VEC2(uint2,+)
EXPLICIT_OP_ASSIGN_VEC2(long2,+)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,+)
EXPLICIT_OP_ASSIGN_VEC2(float2,+)
EXPLICIT_OP_ASSIGN_VEC2(double2,+)
EXPLICIT_OP_ASSIGN_VEC4(char4,+)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,+)
EXPLICIT_OP_ASSIGN_VEC4(int4,+)
EXPLICIT_OP_ASSIGN_VEC4(uint4,+)
EXPLICIT_OP_ASSIGN_VEC4(float4,+)
#else
GENERIC_OP_ASSIGN(char2,+)
GENERIC_OP_ASSIGN(uchar2,+)
GENERIC_OP_ASSIGN(int2,+)
GENERIC_OP_ASSIGN(uint2,+)
GENERIC_OP_ASSIGN(long2,+)
GENERIC_OP_ASSIGN(ulong2,+)
GENERIC_OP_ASSIGN(float2,+)
GENERIC_OP_ASSIGN(double2,+)
GENERIC_OP_ASSIGN(char4,+)
GENERIC_OP_ASSIGN(uchar4,+)
GENERIC_OP_ASSIGN(int4,+)
GENERIC_OP_ASSIGN(uint4,+)
GENERIC_OP_ASSIGN(float4,+)
#endif

/* subtract assign */
#if GCC_VERSION < 40600
EXPLICIT_OP_ASSIGN_VEC2(char2,-)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,-)
EXPLICIT_OP_ASSIGN_VEC2(int2,-)
EXPLICIT_OP_ASSIGN_VEC2(uint2,-)
EXPLICIT_OP_ASSIGN_VEC2(long2,-)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,-)
EXPLICIT_OP_ASSIGN_VEC2(float2,-)
EXPLICIT_OP_ASSIGN_VEC2(double2,-)
EXPLICIT_OP_ASSIGN_VEC4(char4,-)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,-)
EXPLICIT_OP_ASSIGN_VEC4(int4,-)
EXPLICIT_OP_ASSIGN_VEC4(uint4,-)
EXPLICIT_OP_ASSIGN_VEC4(float4,-)
#else
GENERIC_OP_ASSIGN(char2,-)
GENERIC_OP_ASSIGN(uchar2,-)
GENERIC_OP_ASSIGN(int2,-)
GENERIC_OP_ASSIGN(uint2,-)
GENERIC_OP_ASSIGN(long2,-)
GENERIC_OP_ASSIGN(ulong2,-)
GENERIC_OP_ASSIGN(float2,-)
GENERIC_OP_ASSIGN(double2,-)
GENERIC_OP_ASSIGN(char4,-)
GENERIC_OP_ASSIGN(uchar4,-)
GENERIC_OP_ASSIGN(int4,-)
GENERIC_OP_ASSIGN(uint4,-)
GENERIC_OP_ASSIGN(float4,-)
#endif

/* multiply assign */
#if GCC_VERSION < 40600
EXPLICIT_OP_ASSIGN_VEC2(char2,*)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,*)
EXPLICIT_OP_ASSIGN_VEC2(int2,*)
EXPLICIT_OP_ASSIGN_VEC2(uint2,*)
EXPLICIT_OP_ASSIGN_VEC2(long2,*)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,*)
EXPLICIT_OP_ASSIGN_VEC2(float2,*)
EXPLICIT_OP_ASSIGN_VEC2(double2,*)
EXPLICIT_OP_ASSIGN_VEC4(char4,*)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,*)
EXPLICIT_OP_ASSIGN_VEC4(int4,*)
EXPLICIT_OP_ASSIGN_VEC4(uint4,*)
EXPLICIT_OP_ASSIGN_VEC4(float4,*)
#else
GENERIC_OP_ASSIGN(char2,*)
GENERIC_OP_ASSIGN(uchar2,*)
GENERIC_OP_ASSIGN(int2,*)
GENERIC_OP_ASSIGN(uint2,*)
GENERIC_OP_ASSIGN(long2,*)
GENERIC_OP_ASSIGN(ulong2,*)
GENERIC_OP_ASSIGN(float2,*)
GENERIC_OP_ASSIGN(double2,*)
GENERIC_OP_ASSIGN(char4,*)
GENERIC_OP_ASSIGN(uchar4,*)
GENERIC_OP_ASSIGN(int4,*)
GENERIC_OP_ASSIGN(uint4,*)
GENERIC_OP_ASSIGN(float4,*)
#endif

/* modulo assign */
EXPLICIT_OP_ASSIGN_VEC2(char2,%)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,%)
EXPLICIT_OP_ASSIGN_VEC2(int2,%)
EXPLICIT_OP_ASSIGN_VEC2(uint2,%)
EXPLICIT_OP_ASSIGN_VEC2(long2,%)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,%)
EXPLICIT_OP_ASSIGN_VEC4(char4,%)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,%)
EXPLICIT_OP_ASSIGN_VEC4(int4,%)
EXPLICIT_OP_ASSIGN_VEC4(uint4,%)

/* divide assign */
#if GCC_VERSION < 40600
EXPLICIT_OP_ASSIGN_VEC2(char2,/)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,/)
EXPLICIT_OP_ASSIGN_VEC2(int2,/)
EXPLICIT_OP_ASSIGN_VEC2(uint2,/)
EXPLICIT_OP_ASSIGN_VEC2(long2,/)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,/)
EXPLICIT_OP_ASSIGN_VEC2(float2,/)
EXPLICIT_OP_ASSIGN_VEC2(double2,/)
EXPLICIT_OP_ASSIGN_VEC4(char4,/)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,/)
EXPLICIT_OP_ASSIGN_VEC4(int4,/)
EXPLICIT_OP_ASSIGN_VEC4(uint4,/)
EXPLICIT_OP_ASSIGN_VEC4(float4,/)
#else
GENERIC_OP_ASSIGN(char2,/)
GENERIC_OP_ASSIGN(uchar2,/)
GENERIC_OP_ASSIGN(int2,/)
GENERIC_OP_ASSIGN(uint2,/)
GENERIC_OP_ASSIGN(long2,/)
GENERIC_OP_ASSIGN(ulong2,/)
GENERIC_OP_ASSIGN(float2,/)
GENERIC_OP_ASSIGN(double2,/)
GENERIC_OP_ASSIGN(char4,/)
GENERIC_OP_ASSIGN(uchar4,/)
GENERIC_OP_ASSIGN(int4,/)
GENERIC_OP_ASSIGN(uint4,/)
GENERIC_OP_ASSIGN(float4,/)
#endif

/* bitwise and assign */
EXPLICIT_OP_ASSIGN_VEC2(char2,&)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,&)
EXPLICIT_OP_ASSIGN_VEC2(int2,&)
EXPLICIT_OP_ASSIGN_VEC2(uint2,&)
EXPLICIT_OP_ASSIGN_VEC2(long2,&)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,&)
EXPLICIT_OP_ASSIGN_VEC4(char4,&)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,&)
EXPLICIT_OP_ASSIGN_VEC4(int4,&)
EXPLICIT_OP_ASSIGN_VEC4(uint4,&)

/* bitwise or assign */
EXPLICIT_OP_ASSIGN_VEC2(char2,|)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,|)
EXPLICIT_OP_ASSIGN_VEC2(int2,|)
EXPLICIT_OP_ASSIGN_VEC2(uint2,|)
EXPLICIT_OP_ASSIGN_VEC2(long2,|)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,|)
EXPLICIT_OP_ASSIGN_VEC4(char4,|)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,|)
EXPLICIT_OP_ASSIGN_VEC4(int4,|)
EXPLICIT_OP_ASSIGN_VEC4(uint4,|)

/* bitwise xor assign */
EXPLICIT_OP_ASSIGN_VEC2(char2,^)
EXPLICIT_OP_ASSIGN_VEC2(uchar2,^)
EXPLICIT_OP_ASSIGN_VEC2(int2,^)
EXPLICIT_OP_ASSIGN_VEC2(uint2,^)
EXPLICIT_OP_ASSIGN_VEC2(long2,^)
EXPLICIT_OP_ASSIGN_VEC2(ulong2,^)
EXPLICIT_OP_ASSIGN_VEC4(char4,^)
EXPLICIT_OP_ASSIGN_VEC4(uchar4,^)
EXPLICIT_OP_ASSIGN_VEC4(int4,^)
EXPLICIT_OP_ASSIGN_VEC4(uint4,^)


/*** other builtin data types [6.1.3] ***/

#define image2d_t __global void*


/*** conversions and type casting [6.2] ***/

#define convert_char(x) static_cast<char>(x) 
#define convert_uchar(x) static_cast<unsigned char>(x) 
#define convert_int(x) static_cast<int>(x) 
#define convert_uint(x) static_cast<unsigned int>(x) 
#define convert_long(x) static_cast<long>(x) 
#define convert_ulong(x) static_cast<unsigned long>(x) 
#define convert_float(x) static_cast<float>(x) 
#define convert_double(x) static_cast<double>(x) 

#define convert_char2(x) static_cast<char2>(x) 
#define convert_uchar2(x) static_cast<uchar2>(x) 
#define convert_int2(x) static_cast<int2>(x) 
#define convert_uint2(x) static_cast<uint2>(x) 
#define convert_long2(x) static_cast<long2>(x) 
#define convert_ulong2(x) static_cast<ulong2>(x) 
#define convert_float2(x) static_cast<float2>(x) 
#define convert_double2(x) static_cast<double2>(x) 

#define convert_char4(x) static_cast<char4>(x)
#define convert_uchar4(x) static_cast<uchar4>(x)       
#define convert_int4(x) static_cast<int4>(x)
#define convert_uint4(x) static_cast<uint4>(x)       
#define convert_float4(x) static_cast<float4>(x)


/*
static __always_inline double as_double( float2 f2 );
static __always_inline double as_double( float2 f2 ) 
	{ return *(double*)(&f2); }

template < typename T >
static __always_inline float2 as_float2( T x ) { return *(float2*)(&x); }

template < typename T >
static __always_inline uint2 as_uint2( T x ) { return *(uint2*)(&x); }

template < typename T >
static __always_inline int2 as_int2( T x ) { return *(int2*)(&x); }
*/

#define AS_TYPE(type) \
template < typename T > \
static __always_inline type as_##type( T x ) { return *(type*)(&x); }

AS_TYPE(int)
AS_TYPE(float)

AS_TYPE(char2)
AS_TYPE(uchar2)
AS_TYPE(int2)
AS_TYPE(uint2)
AS_TYPE(long2)
AS_TYPE(ulong2)
AS_TYPE(float2)
AS_TYPE(double2)
AS_TYPE(char4)
AS_TYPE(uchar4)
AS_TYPE(int4)
AS_TYPE(uint4)
AS_TYPE(float4)


/*** 
 *** operators for vector data types [6.3] 
 ***/

#define GENERIC_BINOP_VEC2(type,vectype,op) \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ) \
	{ return (_##vectype){ a.vec op b.vec }; } \
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ) \
	{ return (_##vectype){ __vector_##vectype(a,a) op b.vec }; } \
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ) \
	{ return (_##vectype){ a.vec op __vector_##vectype(b,b) }; } 

#define GENERIC_BINOP_VEC4(type,vectype,op) \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ) \
	{ return (_##vectype){ a.vec op b.vec }; } \
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ) \
	{ return (_##vectype){ __vector_##vectype(a,a,a,a) op b.vec }; } \
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ) \
	{ return (_##vectype){ a.vec op __vector_##vectype(b,b,b,b) }; } 

#define EXPLICIT_BINOP_VEC2(type,vectype,op) \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ) \
	{ \
		_##vectype tmp = { a.s0 op b.s0, a.s1 op b.s1 }; \
		return tmp; \
	} \
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ) \
	{ \
		_##vectype tmp = { a op b.s0, a op b.s1 }; \
		return tmp; \
	} \
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ) \
	{ \
		_##vectype tmp = { a.s0 op b, a.s1 op b }; \
		return tmp; \
	} \

#define EXPLICIT_BINOP_VEC4(type,vectype,op) \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ); \
static __always_inline _##vectype \
	operator op ( const _##vectype a, const _##vectype b ) \
	{ \
		_##vectype tmp={a.s0 op b.s0,a.s1 op b.s1,a.s2 op b.s2,a.s3 op b.s3}; \
		return tmp; \
	} \
static __always_inline _##vectype operator op ( type a, const _##vectype b );\
static __always_inline _##vectype \
	operator op ( type a, const _##vectype b ) \
	{ \
		_##vectype tmp={a op b.s0,a op b.s1,a op b.s2,a op b.s3}; \
		return tmp; \
	}\
static __always_inline _##vectype operator op ( const _##vectype a, type b );\
static __always_inline _##vectype \
	operator op ( const _##vectype a, type b ) \
	{ \
		_##vectype tmp={a.s0 op b,a.s1 op b,a.s2 op b,a.s3 op b}; \
		return tmp; \
	} 


#if GCC_VERSION < 40600

/* binary add */
EXPLICIT_BINOP_VEC2(char,char2,+)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,+)
EXPLICIT_BINOP_VEC2(int,int2,+)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,+)
EXPLICIT_BINOP_VEC2(long long,long2,+)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,+)
EXPLICIT_BINOP_VEC2(float,float2,+)
EXPLICIT_BINOP_VEC2(double,double2,+)
EXPLICIT_BINOP_VEC4(char,char4,+)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,+)
EXPLICIT_BINOP_VEC4(int,int4,+)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,+)
EXPLICIT_BINOP_VEC4(float,float4,+)

/* binary sub */
EXPLICIT_BINOP_VEC2(char,char2,-)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,-)
EXPLICIT_BINOP_VEC2(int,int2,-)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,-)
EXPLICIT_BINOP_VEC2(long long,long2,-)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,-)
EXPLICIT_BINOP_VEC2(float,float2,-)
EXPLICIT_BINOP_VEC2(double,double2,-)
EXPLICIT_BINOP_VEC4(char,char4,-)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,-)
EXPLICIT_BINOP_VEC4(int,int4,-)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,-)
EXPLICIT_BINOP_VEC4(float,float4,-)

/* binary mult */
EXPLICIT_BINOP_VEC2(char,char2,*)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,*)
EXPLICIT_BINOP_VEC2(int,int2,*)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,*)
EXPLICIT_BINOP_VEC2(long long,long2,*)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,*)
EXPLICIT_BINOP_VEC2(float,float2,*)
EXPLICIT_BINOP_VEC2(double,double2,*)
EXPLICIT_BINOP_VEC4(char,char4,*)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,*)
EXPLICIT_BINOP_VEC4(int,int4,*)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,*)
EXPLICIT_BINOP_VEC4(float,float4,*)

/* binary mod */
EXPLICIT_BINOP_VEC2(char,char2,%)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,%)
EXPLICIT_BINOP_VEC2(int,int2,%)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,%)
EXPLICIT_BINOP_VEC2(long long,long2,%)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,%)
EXPLICIT_BINOP_VEC4(char,char4,%)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,%)
EXPLICIT_BINOP_VEC4(int,int4,%)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,%)

/* binary div */
EXPLICIT_BINOP_VEC2(char,char2,/)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,/)
EXPLICIT_BINOP_VEC2(int,int2,/)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,/)
EXPLICIT_BINOP_VEC2(long long,long2,/)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,/)
EXPLICIT_BINOP_VEC2(float,float2,/)
EXPLICIT_BINOP_VEC2(double,double2,/)
EXPLICIT_BINOP_VEC4(char,char4,/)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,/)
EXPLICIT_BINOP_VEC4(int,int4,/)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,/)
EXPLICIT_BINOP_VEC4(float,float4,/)

/* binary and */
EXPLICIT_BINOP_VEC2(char,char2,&)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,&)
EXPLICIT_BINOP_VEC2(int,int2,&)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,&)
EXPLICIT_BINOP_VEC2(long long,long2,&)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,&)
EXPLICIT_BINOP_VEC4(char,char4,&)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,&)
EXPLICIT_BINOP_VEC4(int,int4,&)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,&)

/* binary or */
EXPLICIT_BINOP_VEC2(char,char2,|)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,|)
EXPLICIT_BINOP_VEC2(int,int2,|)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,|)
EXPLICIT_BINOP_VEC2(long long,long2,|)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,|)
EXPLICIT_BINOP_VEC4(char,char4,|)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,|)
EXPLICIT_BINOP_VEC4(int,int4,|)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,|)

/* binary xor */
EXPLICIT_BINOP_VEC2(char,char2,^)
EXPLICIT_BINOP_VEC2(unsigned char,uchar2,^)
EXPLICIT_BINOP_VEC2(int,int2,^)
EXPLICIT_BINOP_VEC2(unsigned int,uint2,^)
EXPLICIT_BINOP_VEC2(long long,long2,^)
EXPLICIT_BINOP_VEC2(unsigned long long,ulong2,^)
EXPLICIT_BINOP_VEC4(char,char4,^)
EXPLICIT_BINOP_VEC4(unsigned char,uchar4,^)
EXPLICIT_BINOP_VEC4(int,int4,^)
EXPLICIT_BINOP_VEC4(unsigned int,uint4,^)

#else

/* binary add */
GENERIC_BINOP_VEC2(char,char2,+)
GENERIC_BINOP_VEC2(unsigned char,uchar2,+)
GENERIC_BINOP_VEC2(int,int2,+)
GENERIC_BINOP_VEC2(unsigned int,uint2,+)
GENERIC_BINOP_VEC2(long long,long2,+)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,+)
GENERIC_BINOP_VEC2(float,float2,+)
GENERIC_BINOP_VEC2(double,double2,+)
GENERIC_BINOP_VEC4(char,char4,+)
GENERIC_BINOP_VEC4(unsigned char,uchar4,+)
GENERIC_BINOP_VEC4(int,int4,+)
GENERIC_BINOP_VEC4(unsigned int,uint4,+)
GENERIC_BINOP_VEC4(float,float4,+)

/* binary sub */
GENERIC_BINOP_VEC2(char,char2,-)
GENERIC_BINOP_VEC2(unsigned char,uchar2,-)
GENERIC_BINOP_VEC2(int,int2,-)
GENERIC_BINOP_VEC2(unsigned int,uint2,-)
GENERIC_BINOP_VEC2(long long,long2,-)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,-)
GENERIC_BINOP_VEC2(float,float2,-)
GENERIC_BINOP_VEC2(double,double2,-)
GENERIC_BINOP_VEC4(char,char4,-)
GENERIC_BINOP_VEC4(unsigned char,uchar4,-)
GENERIC_BINOP_VEC4(int,int4,-)
GENERIC_BINOP_VEC4(unsigned int,uint4,-)
GENERIC_BINOP_VEC4(float,float4,-)

/* binary mult */
GENERIC_BINOP_VEC2(char,char2,*)
GENERIC_BINOP_VEC2(unsigned char,uchar2,*)
GENERIC_BINOP_VEC2(int,int2,*)
GENERIC_BINOP_VEC2(unsigned int,uint2,*)
GENERIC_BINOP_VEC2(long long,long2,*)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,*)
GENERIC_BINOP_VEC2(float,float2,*)
GENERIC_BINOP_VEC2(double,double2,*)
GENERIC_BINOP_VEC4(char,char4,*)
GENERIC_BINOP_VEC4(unsigned char,uchar4,*)
GENERIC_BINOP_VEC4(int,int4,*)
GENERIC_BINOP_VEC4(unsigned int,uint4,*)
GENERIC_BINOP_VEC4(float,float4,*)

/* binary mod */
GENERIC_BINOP_VEC2(char,char2,%)
GENERIC_BINOP_VEC2(unsigned char,uchar2,%)
GENERIC_BINOP_VEC2(int,int2,%)
GENERIC_BINOP_VEC2(unsigned int,uint2,%)
GENERIC_BINOP_VEC2(long long,long2,%)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,%)
GENERIC_BINOP_VEC4(char,char4,%)
GENERIC_BINOP_VEC4(unsigned char,uchar4,%)
GENERIC_BINOP_VEC4(int,int4,%)
GENERIC_BINOP_VEC4(unsigned int,uint4,%)

/* binary div */
GENERIC_BINOP_VEC2(char,char2,/)
GENERIC_BINOP_VEC2(unsigned char,uchar2,/)
GENERIC_BINOP_VEC2(int,int2,/)
GENERIC_BINOP_VEC2(unsigned int,uint2,/)
GENERIC_BINOP_VEC2(long long,long2,/)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,/)
GENERIC_BINOP_VEC2(float,float2,/)
GENERIC_BINOP_VEC2(double,double2,/)
GENERIC_BINOP_VEC4(char,char4,/)
GENERIC_BINOP_VEC4(unsigned char,uchar4,/)
GENERIC_BINOP_VEC4(int,int4,/)
GENERIC_BINOP_VEC4(unsigned int,uint4,/)
GENERIC_BINOP_VEC4(float,float4,/)

/* binary and */
GENERIC_BINOP_VEC2(char,char2,&)
GENERIC_BINOP_VEC2(unsigned char,uchar2,&)
GENERIC_BINOP_VEC2(int,int2,&)
GENERIC_BINOP_VEC2(unsigned int,uint2,&)
GENERIC_BINOP_VEC2(long long,long2,&)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,&)
GENERIC_BINOP_VEC4(char,char4,&)
GENERIC_BINOP_VEC4(unsigned char,uchar4,&)
GENERIC_BINOP_VEC4(int,int4,&)
GENERIC_BINOP_VEC4(unsigned int,uint4,&)

/* binary or */
GENERIC_BINOP_VEC2(char,char2,|)
GENERIC_BINOP_VEC2(unsigned char,uchar2,|)
GENERIC_BINOP_VEC2(int,int2,|)
GENERIC_BINOP_VEC2(unsigned int,uint2,|)
GENERIC_BINOP_VEC2(long long,long2,|)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,|)
GENERIC_BINOP_VEC4(char,char4,|)
GENERIC_BINOP_VEC4(unsigned char,uchar4,|)
GENERIC_BINOP_VEC4(int,int4,|)
GENERIC_BINOP_VEC4(unsigned int,uint4,|)

/* binary xor */
GENERIC_BINOP_VEC2(char,char2,^)
GENERIC_BINOP_VEC2(unsigned char,uchar2,^)
GENERIC_BINOP_VEC2(int,int2,^)
GENERIC_BINOP_VEC2(unsigned int,uint2,^)
GENERIC_BINOP_VEC2(long long,long2,^)
GENERIC_BINOP_VEC2(unsigned long long,ulong2,^)
GENERIC_BINOP_VEC4(char,char4,^)
GENERIC_BINOP_VEC4(unsigned char,uchar4,^)
GENERIC_BINOP_VEC4(int,int4,^)
GENERIC_BINOP_VEC4(unsigned int,uint4,^)

#endif


/*** these are generic min and max definitions - they should be corrected ***/

#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a<b)?b:a)


/*** math builtin functions [6.11.2] ***/

#define __MATH_BUILTIN_1(func) \
static __always_inline float func##_T( float a ); \
static __always_inline float func##_T( float a ) \
	{ return func##f(a); } \
static __always_inline float2 func##_T( float2 a ); \
static __always_inline float2 func##_T( float2 a ) \
	{ return vector_float2( func##f(a.x),func##f(a.y) ); } \
static __always_inline float4 func##_T( float4 a ); \
static __always_inline float4 func##_T( float4 a ) \
	{ \
		return vector_float4( \
			func##f(a.x),func##f(a.y),func##f(a.z),func##f(a.w)); \
	} \
static __always_inline double func##_T( double a ); \
static __always_inline double func##_T( double a ) { return func(a); } \
static __always_inline double2 func##_T( double2 a ); \
static __always_inline double2 func##_T( double2 a ) \
	{ return vector_double2( func(a.x),func(a.y) ); } 


__MATH_BUILTIN_1(acos)
__MATH_BUILTIN_1(acosh)
__MATH_BUILTIN_1(asin)
__MATH_BUILTIN_1(asinh)
__MATH_BUILTIN_1(atan)
__MATH_BUILTIN_1(atanh)
__MATH_BUILTIN_1(cbrt)
__MATH_BUILTIN_1(ceil)
__MATH_BUILTIN_1(cos)
__MATH_BUILTIN_1(cosh)
__MATH_BUILTIN_1(erfc)
__MATH_BUILTIN_1(erf)
__MATH_BUILTIN_1(exp)
__MATH_BUILTIN_1(exp2)
#ifndef __FreeBSD__
__MATH_BUILTIN_1(exp10)
#else
#warning FreeBSD missing exp10
#endif
__MATH_BUILTIN_1(expm1)
__MATH_BUILTIN_1(fabs)
__MATH_BUILTIN_1(floor)
__MATH_BUILTIN_1(log)
#ifndef __FreeBSD__
__MATH_BUILTIN_1(log2)
#else
#warning FreeBSD missing log2
#endif
__MATH_BUILTIN_1(log10)
__MATH_BUILTIN_1(log1p)
__MATH_BUILTIN_1(logb)
__MATH_BUILTIN_1(rint)
__MATH_BUILTIN_1(round)
__MATH_BUILTIN_1(sin)
__MATH_BUILTIN_1(sinh)
__MATH_BUILTIN_1(sqrt)
__MATH_BUILTIN_1(tan)
__MATH_BUILTIN_1(tanh)
__MATH_BUILTIN_1(tgamma)
__MATH_BUILTIN_1(trunc)

/*
static __inline float rsqrt_T( float a ) { return 1.0f/sqrtf(a); } \
static __inline _float4 rsqrt_T( _float4 a ) \
	{ return _vector_float4( \
		1.0f/sqrtf(a.x),1.0f/sqrtf(a.y),1.0f/sqrtf(a.z),1.0f/sqrtf(a.w) \
	); } 
*/

static __always_inline float rsqrt_T( float a );
static __always_inline float rsqrt_T( float a ) { return 1.0f/sqrtf(a); }

static __always_inline _float2 rsqrt_T( _float2 a );
static __always_inline _float2 rsqrt_T( _float2 a )
   { return _vector_float2(1.0f/sqrtf(a.x),1.0f/sqrtf(a.y)); }

static __always_inline float4 rsqrt_T( _float4 a );
static __always_inline float4 rsqrt_T( _float4 a )
   { 
		return _vector_float4( 1.0f/sqrtf(a.x),1.0f/sqrtf(a.y), 
			1.0f/sqrtf(a.z),1.0f/sqrtf(a.w)); 
	}

static __always_inline double rsqrt_T( double a );
static __always_inline double rsqrt_T( double a ) { return 1.0f/sqrt(a); }

static __always_inline _double2 rsqrt_T( _double2 a );
static __always_inline _double2 rsqrt_T( _double2 a )
   { return _vector_double2(1.0f/sqrt(a.x),1.0f/sqrt(a.y)); }

template < typename T >
	 static __always_inline T clamp_T( T a, T b0, T b1 );
template < typename T >
	static __always_inline T clamp_T( T a, T b0, T b1 )
   { return min(max(a,b0),b1); }

static __always_inline _float2 clamp_T( _float2 a, float b0, float b1);
static __always_inline _float2 clamp_T( _float2 a, float b0, float b1)
   { return _vector_float2( clamp_T(a.x,b0,b1), clamp_T(a.y,b0,b1) ); }

static __always_inline _float4 
	clamp_T( _float4 a, float b0, float b1);
static __always_inline _float4 clamp_T( _float4 a, float b0, float b1)
   { 
		return _vector_float4( clamp_T(a.x,b0,b1), clamp_T(a.y,b0,b1), 
			clamp_T(a.z,b0,b1), clamp_T(a.w,b0,b1) ); 
	}

static __always_inline _double2 
	clamp_T( _double2 a, double b0, double b1);
static __always_inline _double2 clamp_T( _double2 a, double b0, double b1)
   { return _vector_double2( clamp_T(a.x,b0,b1), clamp_T(a.y,b0,b1) ); }

static __always_inline float dot_T( _float2 a, _float2 b);
static __always_inline float dot_T( _float2 a, _float2 b)
   { float2 tmp = a*b; return tmp.x+tmp.y; }

static __always_inline float dot_T( _float4 a, _float4 b);
static __always_inline float dot_T( _float4 a, _float4 b)
   { float4 tmp = a*b; return tmp.x+tmp.y+tmp.z+tmp.w; }

static __always_inline double dot_T( _double2 a, _double2 b);
static __always_inline double dot_T( _double2 a, _double2 b)
   { double2 tmp = a*b; return tmp.x+tmp.y; }

template < typename T >
	static __always_inline T normalize_T( T a );
template < typename T >
	static __always_inline T normalize_T( T a ) { return rsqrt_T(dot_T(a,a))*a; }

static __always_inline _float4 cross_T( _float4 a, _float4 b);
static __always_inline _float4 cross_T( _float4 a, _float4 b)
	{
		return _vector_float4(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z,
			a.x*b.y-a.y*b.x, 0.0f);
	}


#define sqrt(a) sqrt_T(a)
#define acos(a) acos_T(a)
#define acosh(a) acosh_T(a)
#define asin(a) asin_T(a)
#define asinh(a) asinh_T(a)
#define tan(a) tan_T(a)
#define tanh(a) tanh_T(a)
#define cbrt(a) cbrt_T(a)
#define ceil(a) ceil_T(a)
#define cos(a) cos_T(a)
#define cosh(a) cosh_T(a)
#define erfc(a) erfc_T(a)
#define erf(a) erf_T(a)
#define exp(a) exp_T(a)
#define exp2(a) exp2_T(a)
#define exp10(a) exp10_T(a)
#define expm1(a) expm1_T(a)
#define fabs(a) fabs_T(a)
#define floor(a) floor_T(a)
#define log(a) log_T(a)
#define log2(a) log2_T(a)
#define log10(a) log10_T(a)
#define log1p(a) log1p_T(a)
#define logb(a) logb_T(a)
#define rint(a) rint_T(a)
#define round(a) round_T(a)
#define rsqrt(a) rsqrt_T(a)
#define sin(a) sin_T(a)
#define sinh(a) sinh_T(a)
#define sqrt(a) sqrt_T(a)
#define tan(a) tan_T(a)
#define tanh(a) tanh_T(a)
#define tgamma(a) tgamma_T(a)
#define tfunc(a) tfunc_T(a)

#define clamp(a,b0,b1) clamp_T(a,b0,b1)
#define dot(a,b) dot_T(a,b)
#define normalize(a) normalize_T(a)
#define cross(a,b) cross_T(a,b)


/*** sampler declarations [6.11.8.1] ***/

typedef int sampler_t;
#define CLK_NORMALIZED_COORDS_FALSE 0
#define CLK_ADDRESS_NONE 0
#define CLK_FILTER_NEAREST 0


/*** image read write builtins [6.11.18] ***/

#define read_imagei( _img, _smplr, _xy) \
*((int4*)((intptr_t)_img+128)+((__global size_t*)_img)[0]*(_xy).y+(_xy).x)

#define read_imagef( _img, _smplr, _xy) \
*((float4*)((intptr_t)_img+128)+((__global size_t*)_img)[0]*(_xy).y+(_xy).x)


/*** builtin extensions for initializing vector data types [non-standard] ***/

#define __builtin_vector_char2(x,y) 		vector_char2(x,y)
#define __builtin_vector_char4(x,y,z,w) 	vector_char4(x,y,z,w)
#define __builtin_vector_uchar2(x,y) 		vector_uchar2(x,y)
#define __builtin_vector_uchar4(x,y,z,w)	vector_uchar4(x,y,z,w)
#define __builtin_vector_int2(x,y) 			vector_int2(x,y)
#define __builtin_vector_int4(x,y,z,w) 	vector_int4(x,y,z,w)
#define __builtin_vector_long2(x,y) 		vector_long2(x,y)
#define __builtin_vector_uint2(x,y) 		vector_uint2(x,y)
#define __builtin_vector_uint4(x,y,z,w) 	vector_uint4(x,y,z,w)
#define __builtin_vector_ulong2(x,y) 		vector_ulong2(x,y)
#define __builtin_vector_float2(x,y) 		vector_float2(x,y)
#define __builtin_vector_float4(x,y,z,w) 	vector_float4(x,y,z,w)
#define __builtin_vector_double2(x,y) 		vector_double2(x,y)

#else

/***
 *** included if compiling opencl kernel with an opencl compiler
 ***/

/*** builtin extensions for initializing vector data types [non-standard] ***/

#define __builtin_vector_char2(x,y) 		(char2)(x,y)
#define __builtin_vector_char4(x,y,z,w) 	(char4)(x,y,z,w)
#define __builtin_vector_uchar2(x,y) 		(uchar2)(x,y)
#define __builtin_vector_uchar4(x,y,z,w) 	(uchar4)(x,y,z,w)
#define __builtin_vector_int2(x,y) 			(int2)(x,y)
#define __builtin_vector_int4(x,y,z,w) 	(int4)(x,y,z,w)
#define __builtin_vector_uint2(x,y) 		(uint2)(x,y)
#define __builtin_vector_uint4(x,y,z,w)	(uint4)(x,y,z,w)
#define __builtin_vector_float2(x,y) 		(float2)(x,y)
#define __builtin_vector_float4(x,y,z,w)	(float4)(x,y,z,w)
#define __builtin_vector_double2(x,y) 		(double2)(x,y)

#endif

#endif

