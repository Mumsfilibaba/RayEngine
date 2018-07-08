#include "..\..\Include\Graphics\IFactory.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Vulkan\VkFactory.h"

namespace RayEngine
{
	namespace Graphics
	{
		IFactory* IFactory::Create(GRAPHICS_API api, bool debugLayers)
		{
			if (api != GRAPHICS_API_VULKAN)
				return nullptr;

			return new VKFactory(debugLayers);
		}
	}
}

#endif