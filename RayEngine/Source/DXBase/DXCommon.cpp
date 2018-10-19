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

#include "..\..\Include\Utilities\EngineUtilities.h"
#include "..\..\Include\DXBase\DXCommon.h"

#if defined(RE_PLATFORM_WINDOWS)
#include <comdef.h>
#include <d3dcompiler.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string DXErrorString(HRESULT hr)
	{
		_com_error err(hr);
		return err.ErrorMessage();
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DXGI_FORMAT ReToDXFormat(FORMAT format)
	{
		switch (format)
		{
		case FORMAT_R32G32B32A32_FLOAT:		return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case FORMAT_R32G32B32A32_UINT:		return DXGI_FORMAT_R32G32B32A32_UINT;
		case FORMAT_R32G32B32A32_SINT:		return DXGI_FORMAT_R32G32B32A32_SINT;
		case FORMAT_R32G32B32_FLOAT:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case FORMAT_R32G32B32_UINT:			return DXGI_FORMAT_R32G32B32_UINT;
		case FORMAT_R32G32B32_SINT:			return DXGI_FORMAT_R32G32B32_SINT;
		case FORMAT_R16G16B16A16_FLOAT:		return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case FORMAT_R16G16B16A16_UNORM:		return DXGI_FORMAT_R16G16B16A16_UNORM;
		case FORMAT_R16G16B16A16_UINT:		return DXGI_FORMAT_R16G16B16A16_UINT;
		case FORMAT_R16G16B16A16_SNORM:		return DXGI_FORMAT_R16G16B16A16_SNORM;
		case FORMAT_R16G16B16A16_SINT:		return DXGI_FORMAT_R16G16B16A16_SINT;
		case FORMAT_R32G32_FLOAT:			return DXGI_FORMAT_R32G32_FLOAT;
		case FORMAT_R32G32_UINT:			return DXGI_FORMAT_R32G32_UINT;
		case FORMAT_R32G32_SINT:			return DXGI_FORMAT_R32G32_SINT;
		case FORMAT_D32_FLOAT_S8X24_UINT:	return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		case FORMAT_R8G8B8A8_UNORM:			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case FORMAT_R8G8B8A8_UNORM_SRGB:	return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case FORMAT_R8G8B8A8_UINT:			return DXGI_FORMAT_R8G8B8A8_UINT;
		case FORMAT_R8G8B8A8_SNORM:			return DXGI_FORMAT_R8G8B8A8_SNORM;
		case FORMAT_R8G8B8A8_SINT:			return DXGI_FORMAT_R8G8B8A8_SINT;
		case FORMAT_R16G16_FLOAT:			return DXGI_FORMAT_R16G16_FLOAT;
		case FORMAT_R16G16_UNORM:			return DXGI_FORMAT_R16G16_UNORM;
		case FORMAT_R16G16_UINT:			return DXGI_FORMAT_R16G16_UINT;
		case FORMAT_R16G16_SNORM:			return DXGI_FORMAT_R16G16_SNORM;
		case FORMAT_R16G16_SINT:			return DXGI_FORMAT_R16G16_SINT;
		case FORMAT_D32_FLOAT:				return DXGI_FORMAT_D32_FLOAT;
		case FORMAT_R32_FLOAT:				return DXGI_FORMAT_R32_FLOAT;
		case FORMAT_R32_UINT:				return DXGI_FORMAT_R32_UINT;
		case FORMAT_R32_SINT:				return DXGI_FORMAT_R32_SINT;
		case FORMAT_D24_UNORM_S8_UINT:		return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case FORMAT_R8G8_UNORM:				return DXGI_FORMAT_R8G8_UNORM;
		case FORMAT_R8G8_UINT:				return DXGI_FORMAT_R8G8_UINT;
		case FORMAT_R8G8_SNORM:				return DXGI_FORMAT_R8G8_SNORM;
		case FORMAT_R8G8_SINT:				return DXGI_FORMAT_R8G8_SINT;
		case FORMAT_R16_FLOAT:				return DXGI_FORMAT_R16_FLOAT;
		case FORMAT_D16_UNORM:				return DXGI_FORMAT_D16_UNORM;
		case FORMAT_R16_UNORM:				return DXGI_FORMAT_R16_UNORM;
		case FORMAT_R16_UINT:				return DXGI_FORMAT_R16_UINT;
		case FORMAT_R16_SNORM:				return DXGI_FORMAT_R16_SNORM;
		case FORMAT_R16_SINT:				return DXGI_FORMAT_R16_SINT;
		case FORMAT_R8_UNORM:				return DXGI_FORMAT_R8_UNORM;
		case FORMAT_R8_UINT:				return DXGI_FORMAT_R8_UINT;
		case FORMAT_R8_SNORM:				return DXGI_FORMAT_R8_SNORM;
		case FORMAT_R8_SINT:				return DXGI_FORMAT_R8_SINT;
		case FORMAT_B8G8R8A8_UNORM:			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case FORMAT_B8G8R8A8_UNORM_SRGB:	return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		}

		return DXGI_FORMAT_UNKNOWN;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D_PRIMITIVE_TOPOLOGY ReToDXTopology(PRIMITIVE_TOPOLOGY topology)
	{
		switch (topology)
		{
		case PRIMITIVE_TOPOLOGY_TRIANGLELIST: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case PRIMITIVE_TOPOLOGY_LINELIST: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case PRIMITIVE_TOPOLOGY_LINESTRIP: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case PRIMITIVE_TOPOLOGY_POINTS: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		default: return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}
	}
}

#endif