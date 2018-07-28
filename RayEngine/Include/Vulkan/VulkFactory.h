#pragma once

#include "..\Graphics\IFactory.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkFactory : public IFactory
		{
		public:
			VulkFactory(const VulkFactory& other) = delete;
			VulkFactory& operator=(const VulkFactory& other) = delete;

		public:
			VulkFactory(bool debugLayers);
			VulkFactory(VulkFactory&& other);
			~VulkFactory();

			VkInstance GetVkInstance() const;

			VulkFactory& operator=(VulkFactory&& other);

			void EnumerateAdapters(AdapterList& list) const override final;

			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) override final;
			bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo,
				ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateShaderCompiler(IShaderCompiler** ppCompiler) override final;

			GRAPHICS_API GetGraphicsApi() const override final;
			
			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(bool debugLayers);

		private:
			VkInstance m_Instance;
			VkDebugReportCallbackEXT m_DbgCallback;

			mutable uint32 m_ReferenceCount;

		public:
			static bool InstanceLayersSupported(const char* const * ppNeededLayers, int32 count);
			static bool InstanceExtensionsSupported(const char* const * ppNeededExtensions, int32 count);
			static bool DeviceExtensionsSupported(VkPhysicalDevice& adapter, const char* const * ppNeededExtensions, int32 count);
			static void FillAdapterInfo(AdapterInfo& info, VkPhysicalDeviceFeatures& features,
				VkPhysicalDeviceProperties& properties, int32 id, int32 supportFlags);
			static void CheckQueueFamilySupport(VkPhysicalDevice& adapter, VkQueueFamilyProperties& queuefamily, int32& supportFlags);

		private:
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
				VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location,
				int32_t code, const char* pLayerPrefix, const char* pMsg, void* pUserData);
		};
	}
}