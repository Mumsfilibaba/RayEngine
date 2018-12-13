#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12Renderer.h>

namespace RayEngine
{
	namespace Graphics
	{
		DX12Renderer::DX12Renderer(DX12Device* pDevice)
			: m_References(0)
		{
		}
	}
}

#endif