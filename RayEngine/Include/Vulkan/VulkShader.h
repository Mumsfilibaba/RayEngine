#pragma once

#include "..\Graphics\IShader.h"
#include "VulkCommon.h"
#include "VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkShader : public IShader
		{
		public:
			VulkShader(const VulkShader& other) = delete;
			VulkShader& operator=(const VulkShader& other) = delete;
			VulkShader(VulkShader&& other) = delete;
			VulkShader& operator=(VulkShader&& other) = delete;

		public:
			VulkShader(IDevice* pDevice, const ShaderInfo& info);
			~VulkShader();

			const std::string& GetEntryPoint() const;
			VkShaderModule GetVkShaderModule() const;
			
			SHADER_TYPE GetType() const override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ShaderInfo& info);

		private:
			VulkDevice* m_Device;
			VkShaderModule m_Module;
			SHADER_TYPE m_Type;
			std::string m_EntryPoint;
		};
	}
}
