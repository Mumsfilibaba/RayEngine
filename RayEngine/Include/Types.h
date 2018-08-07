#pragma once

#include <string>

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
#if defined(UNICODE)
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
		RESOURCE_STATE_RENDER_TARGET = 3,
		RESOURCE_STATE_PRESENT = 4,
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
	enum CPU_ACCESS_FLAG : int32
	{
		CPU_ACCESS_FLAG_NONE = 0,
		CPU_ACCESS_FLAG_READ = (1 << 0),
		CPU_ACCESS_FLAG_WRITE = (1 << 1)
	};



	/////////////////////////////////////////////////////////////
	enum PRIMITIVE_TOPOLOGY : int32
	{
		PRIMITIVE_TOPOLOGY_UNKNOWN = 0,
		PRIMITIVE_TOPOLOGY_TRIANGLELIST = 1,
		PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 2,
		PRIMITIVE_TOPOLOGY_POINTS = 3,
		PRIMITIVE_TOPOLOGY_LINELIST = 4,
		PRIMITIVE_TOPOLOGY_LINESTRIP = 5,
	};



	/////////////////////////////////////////////////////////////
	enum BLEND_TYPE : int32
	{
		BLEND_TYPE_UNKNOWN = 0,
		//Blend factor = (0, 0, 0, 0)
		BLEND_TYPE_ZERO = 1,
		//Blend factor = (1, 1, 1, 1)
		BLEND_TYPE_ONE = 2,
		//Blend factor = (Rsrc, Gsrc, Bsrc, Asrc)
		BLEND_TYPE_SRC_COLOR = 3,
		//Blend factor = (1 - Rsrc, 1 - Gsrc, 1 - Bsrc, 1 - Asrc)
		BLEND_TYPE_INV_SRC_COLOR = 4,
		//Blend factor = (Asrc, Asrc, Asrc, Asrc)
		BLEND_TYPE_SRC_ALPHA = 5,
		//Blend factor = (1 - Asrc, 1 - Asrc, 1 - Asrc, 1 - Asrc)
		BLEND_TYPE_INV_SRC_ALPHA = 6,
		//Blend factor = (Adst, Adst, Adst, Adst)
		BLEND_TYPE_DEST_ALPHA = 7,
		//Blend factor = (1 - Adst, 1 - Adst, 1 - Adst, 1 - Adst)
		BLEND_TYPE_INV_DEST_ALPHA = 8,
		//Blend factor = (Rdst, Gdst, Bdst, Adst)
		BLEND_TYPE_DEST_COLOR = 9,
		//Blend factor = (1 - Rdst, 1 - Gdst, 1 - Bdst, 1 - Adst)
		BLEND_TYPE_INV_DEST_COLOR = 10,
		//Blend factor = (f, f, f, f), f = min(Asrc, 1 - Asrc)
		BLEND_TYPE_SRC_ALPHA_SAT = 11,
		//Blend factor = (BlendFactor[0], BlendFactor[1], BlendFactor[2], BlendFactor[3])
		BLEND_TYPE_BLEND_FACTOR = 12,
		//Blend factor = (1 - BlendFactor[0], 1 - BlendFactor[1], 1 - BlendFactor[2], 1 - BlendFactor[3])
		BLEND_TYPE_INV_BLEND_FACTOR = 13,
	};



	/////////////////////////////////////////////////////////////
	enum BLEND_OPERATION : int32
	{
		BLEND_OPERATION_UNKNOWN = 0,
		BLEND_OPERATION_ADD = 1,
		BLEND_OPERATION_SUBTRACT = 2,
		BLEND_OPERATION_REV_SUBTRACT = 3,
		BLEND_OPERATION_MIN = 4,
		BLEND_OPERATION_MAX = 5,
	};



	/////////////////////////////////////////////////////////////
	enum DEPTH_WRITE_MASK : int32
	{
		DEPTH_WRITE_MASK_UNKNOWN = 0,
		DEPTH_WRITE_MASK_ZERO = 1,
		DEPTH_WRITE_MASK_ALL = 2,
	};



	/////////////////////////////////////////////////////////////
	enum COMPARISON_FUNC : int32
	{
		COMPARISON_FUNC_UNKNOWN = 0,
		COMPARISON_FUNC_NEVER = 1,
		COMPARISON_FUNC_LESS = 2,
		COMPARISON_FUNC_EQUAL = 3,
		COMPARISON_FUNC_LESS_EQUAL = 4,
		COMPARISON_FUNC_GREATER = 5,
		COMPARISON_FUNC_NOT_EQUAL = 6,
		COMPARISON_FUNC_GREATER_EQUAL = 7,
		COMPARISON_FUNC_ALWAYS = 8
	};



	/////////////////////////////////////////////////////////////
	enum STENCIL_OPERATION : int32
	{
		STENCIL_OPERATION_UNKNOWN = 0,
		STENCIL_OPERATION_KEEP = 1,
		STENCIL_OPERATION_ZERO = 2,
		STENCIL_OPERATION_REPLACE = 3,
		STENCIL_OPERATION_INVERT = 4,
		STENCIL_OPERATION_INCR = 5,
		STENCIL_OPERATION_DECR = 6,
	};



	/////////////////////////////////////////////////////////////
	enum FILL_MODE : int32
	{
		FILL_MODE_UNKNOWN = 0,
		FILL_MODE_SOLID = 1,
		FILL_MODE_WIREFRAME = 2,
	};



	/////////////////////////////////////////////////////////////
	enum CULL_MODE : int32
	{
		CULL_MODE_UNKNOWN = 0,
		CULL_MODE_BACK = 1,
		CULL_MODE_FRONT = 2,
		CULL_MODE_NONE = 3,
	};



	/////////////////////////////////////////////////////////////
	enum COLOR_WRITE_ENABLE : int32
	{
		COLOR_WRITE_ENABLE_UNKNOWN = 0,
		COLOR_WRITE_ENABLE_RED = (1 << 0),
		COLOR_WRITE_ENABLE_GREEN = (1 << 1),
		COLOR_WRITE_ENABLE_BLUE = (1 << 2),
		COLOR_WRITE_ENABLE_ALPHA = (1 << 3),
		COLOR_WRITE_ENABLE_ALL = COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN |
		COLOR_WRITE_ENABLE_BLUE | COLOR_WRITE_ENABLE_ALPHA
	};



	/////////////////////////////////////////////////////////////
	enum MSAA_SAMPLE_COUNT : int32
	{
		MSAA_SAMPLE_COUNT_UNKNOWN = 0,
		MSAA_SAMPLE_COUNT_1 = 1,
		MSAA_SAMPLE_COUNT_2 = 2,
		MSAA_SAMPLE_COUNT_4 = 4,
		MSAA_SAMPLE_COUNT_8 = 8,
		MSAA_SAMPLE_COUNT_16 = 16
	};




	/////////////////////////////////////////////////////////////
	enum SHADER_VISIBILITY : int32
	{
		SHADER_VISIBILITY_UNKNOWN = 0,
		SHADER_VISIBILITY_INPUT_LAYOUT = (1 << 0),
		SHADER_VISIBILITY_VERTEX_SHADER = (1 << 1),
		SHADER_VISIBILITY_HULL_SHADER = (1 << 2),
		SHADER_VISIBILITY_DOMAIN_SHADER = (1 << 3),
		SHADER_VISIBILITY_GEOMETRY_SHADER = (1 << 4),
		SHADER_VISIBILITY_PIXEL_SHADER = (1 << 5),
	};



	/////////////////////////////////////////////////////////////
	enum SHADER_TYPE : int32
	{
		SHADER_TYPE_UNKNOWN = 0,
		SHADER_TYPE_VERTEX = 1,
		SHADER_TYPE_HULL = 2,
		SHADER_TYPE_DOMAIN = 3,
		SHADER_TYPE_GEOMETRY = 4,
		SHADER_TYPE_PIXEL = 5,
		SHADER_TYPE_COMPUTE = 6,
	};



	/////////////////////////////////////////////////////////////
	enum PIPELINE_TYPE : int32
	{
		PIPELINE_TYPE_UNKNOWN = 0,
		PIPELINE_TYPE_GRAPHICS = 1,
		PIPELINE_TYPE_COMPUTE = 2,
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



	/////////////////////////////////////////////////////////////
	struct StencilOperationInfo
	{
		STENCIL_OPERATION StencilFailOperation = STENCIL_OPERATION_KEEP;
		STENCIL_OPERATION StencilDepthFailOperation = STENCIL_OPERATION_KEEP;
		STENCIL_OPERATION StencilPassoperation = STENCIL_OPERATION_KEEP;
		COMPARISON_FUNC StencilFunc = COMPARISON_FUNC_ALWAYS;
	};
}