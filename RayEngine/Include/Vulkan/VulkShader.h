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
#include <Graphics\IShader.h>
#include "VulkCommon.h"
#include "VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IDevice;

		class VulkShader final : public IShader
		{
			RE_IMPLEMENT_INTERFACE(VulkShader);

		public:
			VulkShader(IDevice* pDevice, const ShaderDesc* pDesc);
			~VulkShader();

			inline const char* GetEntryPoint() const
			{
				return m_Desc.pEntryPoint;
			}
			
			inline VkShaderModule GetVkShaderModule() const
			{
				return m_Module;
			}

			inline SHADER_TYPE GetType() const
			{
				return m_Desc.Type;
			}

			void GetDesc(ShaderDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const ShaderDesc* pDesc);

		private:
			VulkDevice* m_Device;
			
			VkShaderModule m_Module;
			
			ShaderDesc m_Desc;
		
			CounterType m_References;
		};
	}
}
