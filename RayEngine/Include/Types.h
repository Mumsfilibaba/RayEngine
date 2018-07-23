#pragma once

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;



	/////////////////////////////////////////////////////////////
#ifdef UNICODE
	typedef wchar_t Tchar;
#else
	typedef char Tchar;
#endif



	/////////////////////////////////////////////////////////////
	enum FORMAT : int32
	{
		FORMAT_UNKNOWN = 0,
		FORMAT_R32G32B32A32_FLOAT = 1,
		FORMAT_R32G32B32A32_UINT = 2,
		FORMAT_R32G32B32A32_SINT = 3,
		FORMAT_R32G32B32_FLOAT = 4,
		FORMAT_R32G32B32_UINT = 5,
		FORMAT_R32G32B32_SINT = 6,
		FORMAT_R16G16B16A16_FLOAT = 7,
		FORMAT_R16G16B16A16_UNORM = 8,
		FORMAT_R16G16B16A16_UINT = 9,
		FORMAT_R16G16B16A16_SNORM = 10,
		FORMAT_R16G16B16A16_SINT = 11,
		FORMAT_R32G32_FLOAT = 12,
		FORMAT_R32G32_UINT = 13,
		FORMAT_R32G32_SINT = 14,
		FORMAT_D32_FLOAT_S8X24_UINT = 15,
		FORMAT_R10G10B10A2_UNORM = 16,
		FORMAT_R10G10B10A2_UINT = 17,
		FORMAT_R11G11B10_FLOAT = 18,
		FORMAT_R8G8B8A8_TYPELESS = 19,
		FORMAT_R8G8B8A8_UNORM = 20,
		FORMAT_R8G8B8A8_UNORM_SRGB = 21,
		FORMAT_R8G8B8A8_UINT = 22,
		FORMAT_R8G8B8A8_SNORM = 23,
		FORMAT_R8G8B8A8_SINT = 24,
		FORMAT_R16G16_FLOAT = 25,
		FORMAT_R16G16_UNORM = 26,
		FORMAT_R16G16_UINT = 27,
		FORMAT_R16G16_SNORM = 28,
		FORMAT_R16G16_SINT = 29,
		FORMAT_D32_FLOAT = 30,
		FORMAT_R32_FLOAT = 31,
		FORMAT_R32_UINT = 32,
		FORMAT_R32_SINT = 33,
		FORMAT_D24_UNORM_S8_UINT = 34,
		FORMAT_R8G8_UNORM = 35,
		FORMAT_R8G8_UINT = 36,
		FORMAT_R8G8_SNORM = 37,
		FORMAT_R8G8_SINT = 38,
		FORMAT_R16_FLOAT = 39,
		FORMAT_D16_UNORM = 40,
		FORMAT_R16_UNORM = 41,
		FORMAT_R16_UINT = 42,
		FORMAT_R16_SNORM = 43,
		FORMAT_R16_SINT = 44,
		FORMAT_R8_UNORM = 45,
		FORMAT_R8_UINT = 46,
		FORMAT_R8_SNORM = 47,
		FORMAT_R8_SINT = 48,
		FORMAT_B8G8R8A8_UNORM = 49,
		FORMAT_B8G8R8A8_UNORM_SRGB = 50,
		FORMAT_B4G4R4A4_UNORM = 51,
	};



	/////////////////////////////////////////////////////////////
	enum GRAPHICS_API : int32
	{
		GRAPHICS_API_UNKNOWN = 0,
		GRAPHICS_API_D3D12 = 1,
		GRAPHICS_API_D3D11 = 2,
		GRAPHICS_API_VULKAN = 3,
	};



	/////////////////////////////////////////////////////////////
	enum RESOURCE_STATE : int32
	{
		RESOURCE_STATE_UNKNOWN = 0,
		RESOURCE_STATE_COMMON = 1,
		RESOURCE_STATE_DEPTH_WRITE = 2,
	};



	/////////////////////////////////////////////////////////////
	enum RESOURCE_USAGE : int32
	{
		RESOURCE_USAGE_UNKNOWN = 0,
		RESOURCE_USAGE_STATIC = 1,
		RESOURCE_USAGE_DEFAULT = 2,
		RESOURCE_USAGE_DYNAMIC = 3,
	};



	/////////////////////////////////////////////////////////////
	struct ResourceData
	{
		//pData - Data that is meant to be submitted to the graphics card
		const void* pData = nullptr;
		//ByteStride - Size in bytes of one element
		int32 ByteStride = 0;
		//WidthOrCount - Number of elements in pData - One row of pixels in a texture or number of elements in a buffer
		int32 WidthOrCount = 0;
		//Height - Number of rows in pData - Does not matter for buffers
		int32 Height = 0;
	};
}