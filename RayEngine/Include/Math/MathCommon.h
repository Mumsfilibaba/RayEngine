#pragma once

#include "MathUtilities.h"

#if defined(_WIN32)
#define forceinline _forceinline
#else
#define forceinline 
#endif

#if defined(_WIN32)
#define VECTORALIGN __declspec(align(16))
#else
#define VECTORALIGN
#endif

#if defined(_WIN32) && !defined(NO_SIMD)
#define SSE_INTRINSICS
#endif

namespace Math
{
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
}