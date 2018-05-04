#pragma once

namespace RayEngine
{
	//Typedefs
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;

	//define if we are using ascii or unicode
#ifdef UNICODE
	typedef wchar_t TChar;
#else
	typedef char TChar;
#endif
}