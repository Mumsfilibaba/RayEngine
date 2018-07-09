#include "..\..\Include\Graphics\IFactory.h"

#if defined(RE_PLATFORM_WINDOWS)

#include "..\..\Include\Vulkan\VkFactory.h"
#include "..\..\Include\DX12\DX12Factory.h"

namespace RayEngine
{
	namespace Graphics
	{
		IFactory* IFactory::Create(GRAPHICS_API api, bool debugLayers)
		{
			if (api == GRAPHICS_API_D3D12)
				return new DX12Factory(debugLayers);
			else if (api == GRAPHICS_API_D3D11)
				return nullptr;
			else if (api == GRAPHICS_API_VULKAN)
				return new VKFactory(debugLayers);

			return nullptr;
		}
	}
}

#endif