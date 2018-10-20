/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline int32 GetVertexComponents(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R16G16B16A16_FLOAT:
		case FORMAT_R16G16B16A16_UNORM:
		case FORMAT_R16G16B16A16_UINT:
		case FORMAT_R16G16B16A16_SNORM:
		case FORMAT_R16G16B16A16_SINT:
		case FORMAT_R8G8B8A8_UNORM:
		case FORMAT_R8G8B8A8_UINT:
		case FORMAT_R8G8B8A8_SNORM:
		case FORMAT_R8G8B8A8_SINT:
			return 4;

		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32B32_SINT:
			return 3;

		case FORMAT_R32G32_FLOAT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32G32_SINT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8G8_UINT:
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8G8_SINT:
		case FORMAT_R16_FLOAT:
			return 2;

		case FORMAT_R32_FLOAT:
		case FORMAT_R32_UINT:
		case FORMAT_R32_SINT:
		case FORMAT_R16_UNORM:
		case FORMAT_R16_UINT:
		case FORMAT_R16_SNORM:
		case FORMAT_R16_SINT:
		case FORMAT_R8_UNORM:
		case FORMAT_R8_UINT:
		case FORMAT_R8_SNORM:
		case FORMAT_R8_SINT:
			return 1;

		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline int32 GetVertexFormat(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:
		case FORMAT_R32G32B32_FLOAT:
		case FORMAT_R32G32_FLOAT:
		case FORMAT_R32_FLOAT:
			return GL_FLOAT;

		case FORMAT_R16G16B16A16_FLOAT:
		case FORMAT_R16G16_FLOAT:
		case FORMAT_R16_FLOAT:
			return GL_HALF_FLOAT;

		case FORMAT_R32G32B32A32_UINT:
		case FORMAT_R32G32B32_UINT:
		case FORMAT_R32G32_UINT:
		case FORMAT_R32_UINT:
			return GL_UNSIGNED_INT;

		case FORMAT_R32G32B32A32_SINT:
		case FORMAT_R32G32B32_SINT:
		case FORMAT_R32G32_SINT:
		case FORMAT_R32_SINT:
			return GL_INT;

		case FORMAT_R16G16B16A16_UNORM:
		case FORMAT_R16G16B16A16_UINT:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16G16_UINT:
		case FORMAT_R16_UNORM:
		case FORMAT_R16_UINT:
			return GL_UNSIGNED_SHORT;

		case FORMAT_R16G16B16A16_SNORM:
		case FORMAT_R16G16B16A16_SINT:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16G16_SINT:
		case FORMAT_R16_SNORM:
		case FORMAT_R16_SINT:
			return GL_SHORT;

		case FORMAT_R8G8B8A8_SNORM:
		case FORMAT_R8G8B8A8_SINT:
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8G8_SINT:
		case FORMAT_R8_SNORM:
		case FORMAT_R8_SINT:
			return GL_BYTE;

		case FORMAT_R8G8B8A8_UNORM:
		case FORMAT_R8G8B8A8_UINT:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8G8_UINT:
		case FORMAT_R8_UNORM:
		case FORMAT_R8_UINT:
			return GL_UNSIGNED_BYTE;

		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool NormalizedVertexFormat(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R16G16B16A16_UNORM:
		case FORMAT_R16G16_UNORM:
		case FORMAT_R16_UNORM:
		case FORMAT_R16G16B16A16_SNORM:
		case FORMAT_R16G16_SNORM:
		case FORMAT_R16_SNORM:
		case FORMAT_R8G8B8A8_SNORM:
		case FORMAT_R8G8_SNORM:
		case FORMAT_R8_SNORM:
		case FORMAT_R8G8B8A8_UNORM:
		case FORMAT_R8G8_UNORM:
		case FORMAT_R8_UNORM:
			return true;

		default:
			return false;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline int32 ShaderTypeToGL(SHADER_TYPE type)
	{
		switch (type)
		{
		case SHADER_TYPE_VERTEX:
			return GL_VERTEX_SHADER;
		case SHADER_TYPE_HULL:
			return GL_TESS_CONTROL_SHADER;
		case SHADER_TYPE_DOMAIN:
			return GL_TESS_EVALUATION_SHADER;
		case SHADER_TYPE_GEOMETRY:
			return GL_GEOMETRY_SHADER;
		case SHADER_TYPE_PIXEL:
			return GL_FRAGMENT_SHADER;
		case SHADER_TYPE_COMPUTE:
			return GL_COMPUTE_SHADER;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 ComparisonFuncToGL(COMPARISON_FUNC func)
	{
		switch (func)
		{
		case COMPARISON_FUNC_NEVER:
			return GL_NEVER;
		case COMPARISON_FUNC_LESS:
			return GL_LESS;
		case COMPARISON_FUNC_EQUAL:
			return GL_EQUAL;
		case COMPARISON_FUNC_LESS_EQUAL:
			return GL_LEQUAL;
		case COMPARISON_FUNC_GREATER:
			return GL_GREATER;
		case COMPARISON_FUNC_NOT_EQUAL:
			return GL_NOTEQUAL;
		case COMPARISON_FUNC_GREATER_EQUAL:
			return GL_GEQUAL;
		case COMPARISON_FUNC_ALWAYS:
			return GL_ALWAYS;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 StencilOpToGL(STENCIL_OPERATION operation)
	{
		switch (operation)
		{
		case STENCIL_OPERATION_KEEP:
			return GL_KEEP;
		case STENCIL_OPERATION_ZERO:
			return GL_ZERO;
		case STENCIL_OPERATION_REPLACE:
			return GL_REPLACE;
		case STENCIL_OPERATION_INVERT:
			return GL_INVERT;
		case STENCIL_OPERATION_INCR:
			return GL_INCR;
		case STENCIL_OPERATION_DECR:
			return GL_DECR;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 BlendTypeToGL(BLEND_TYPE type)
	{
		switch (type)
		{
		case BLEND_TYPE_ZERO:
			return GL_ZERO;
		case BLEND_TYPE_ONE:
			return GL_BLEND;
		case BLEND_TYPE_SRC_COLOR:
			return GL_SRC_COLOR;
		case BLEND_TYPE_INV_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case BLEND_TYPE_SRC_ALPHA:
			return GL_SRC_ALPHA;
		case BLEND_TYPE_INV_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case BLEND_TYPE_DEST_ALPHA:
			return GL_DST_ALPHA;
		case BLEND_TYPE_INV_DEST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
		case BLEND_TYPE_DEST_COLOR:
			return GL_DST_COLOR;
		case BLEND_TYPE_INV_DEST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case BLEND_TYPE_SRC_ALPHA_SAT:
			return GL_SRC_ALPHA_SATURATE;
		case BLEND_TYPE_BLEND_FACTOR:
			return GL_CONSTANT_COLOR;
		case BLEND_TYPE_INV_BLEND_FACTOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 BlendOperationToGL(BLEND_OPERATION type)
	{
		switch (type)
		{
		case BLEND_OPERATION_ADD:
			return GL_FUNC_ADD;
		case BLEND_OPERATION_SUBTRACT:
			return GL_FUNC_SUBTRACT;
		case BLEND_OPERATION_REV_SUBTRACT:
			return GL_FUNC_REVERSE_SUBTRACT;
		case BLEND_OPERATION_MIN:
			return GL_MIN;
		case BLEND_OPERATION_MAX:
			return GL_MAX;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 BufferUsageToGL(BUFFER_USAGE usage)
	{
		switch (usage)
		{
		case BUFFER_USAGE_CONSTANT:
			return GL_UNIFORM_BUFFER;
		case BUFFER_USAGE_VERTEX:
			return GL_ARRAY_BUFFER;
		case BUFFER_USAGE_INDEX:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BUFFER_USAGE_SHADER_RESOURCE:
			return GL_TEXTURE_BUFFER;
		case BUFFER_USAGE_UNORDERED_ACCESS:
			return GL_SHADER_STORAGE_BUFFER;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 ResourceUsageToGL(RESOURCE_USAGE usage)
	{
		switch (usage)
		{
		case RESOURCE_USAGE_STATIC:
		case RESOURCE_USAGE_DEFAULT:
			return GL_STATIC_DRAW;
		case RESOURCE_USAGE_DYNAMIC:
			return GL_DYNAMIC_DRAW;
		default:
			return 0;
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 CPUAccessToGL(CPU_ACCESS_FLAG cpuAccess)
	{
		uint32 access = 0;
		if (cpuAccess & CPU_ACCESS_FLAG_READ)
			access |= GL_MAP_READ_BIT;

		if (cpuAccess & CPU_ACCESS_FLAG_WRITE)
			access |= GL_MAP_WRITE_BIT;

		return access;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline uint32 PrimitiveTopologyToGL(PRIMITIVE_TOPOLOGY topology)
	{
		switch (topology)
		{
		case PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			return GL_TRIANGLES;
		case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
			return GL_TRIANGLE_STRIP;
		case PRIMITIVE_TOPOLOGY_POINTS:
			return GL_POINTS;
		case PRIMITIVE_TOPOLOGY_LINELIST:
			return GL_LINES;
		case PRIMITIVE_TOPOLOGY_LINESTRIP:
			return GL_LINE_STRIP;
		default:
			return 0;
		}
	}
}