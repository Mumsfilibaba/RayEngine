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

			void SetName(const std::string& name) override final;
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
