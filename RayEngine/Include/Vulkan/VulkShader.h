#pragma once

#include "..\Graphics\IShader.h"
#include "VulkCommon.h"

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
			VulkShader(IDevice* pDevice, const ShaderByteCode& byteCode);
			~VulkShader();

			const std::string& GetEntryPoint() const;
			VkShaderModule GetVkShaderModule() const;
			SHADER_TYPE GetType() const override final;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const ShaderByteCode& byteCode);

		private:
			IDevice* m_Device;
			SHADER_TYPE m_Type;
			VkShaderModule m_Module;
			std::string m_EntryPoint;
		};
	}
}
