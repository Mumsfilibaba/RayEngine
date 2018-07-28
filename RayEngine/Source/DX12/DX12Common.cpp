#include <sstream>
#include "..\..\Include\DX12\DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	RESOURCE_STATE DXToREResourceState(D3D12_RESOURCE_STATES state)
	{
		switch (state)
		{
		case D3D12_RESOURCE_STATE_DEPTH_WRITE: return RESOURCE_STATE_DEPTH_WRITE;
		case D3D12_RESOURCE_STATE_RENDER_TARGET: return RESOURCE_STATE_RENDER_TARGET;
		case D3D12_RESOURCE_STATE_COMMON:
		default: return RESOURCE_STATE_COMMON;
		}
	}



	/////////////////////////////////////////////////////////////
	D3D12_RESOURCE_STATES ReToDXResourceState(RESOURCE_STATE state)
	{
		switch (state)
		{
		case RESOURCE_STATE_DEPTH_WRITE: return D3D12_RESOURCE_STATE_DEPTH_WRITE;
		case RESOURCE_STATE_RENDER_TARGET: return D3D12_RESOURCE_STATE_RENDER_TARGET;
		case RESOURCE_STATE_PRESENT:
		case RESOURCE_STATE_COMMON:
		default: return D3D12_RESOURCE_STATE_COMMON;
		}
	}



	/////////////////////////////////////////////////////////////
	void D3D12SetName(ID3D12Object* pObject, const std::string& name)
	{
		std::wstringstream wName;
		wName << name.c_str();

		if FAILED(pObject->SetName(wName.str().c_str()))
			return;
	}
}

#endif