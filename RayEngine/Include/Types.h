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
	typedef wchar_t Tchar;
#else
	typedef char Tchar;
#endif

	enum FORMAT : int32
	{
		FORMAT_UNKNOWN = 0,
		FORMAT_R8G8B8A8_UINT = 1,
		FORMAT_R32G32B32A32_FLOAT = 2,
	};

	enum GRAPHICS_API : int32
	{
		GRAPHICS_API_UNKNOWN = 0,
		GRAPHICS_API_VULKAN = 1,
	};
}