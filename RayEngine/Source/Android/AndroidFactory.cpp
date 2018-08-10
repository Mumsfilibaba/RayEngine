#include "..\..\Include\Graphics\IFactory.h"

#if defined(RE_PLATFORM_ANDROID)

#include "..\..\Include\Vulkan\VulkFactory.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		IFactory* IFactory::Create(GRAPHICS_API api, bool debugLayers)
		{
			return Create("<Unnamed-Factory>", api, debugLayers);
		}



		/////////////////////////////////////////////////////////////
		IFactory* IFactory::Create(const std::string& name, GRAPHICS_API api, bool debugLayers)
		{
			if (api != GRAPHICS_API_VULKAN)
				return nullptr;

			return new VulkFactory(debugLayers);
		}
	}
}

#endif