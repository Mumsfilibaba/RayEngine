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
#include <vector>
#include "..\Graphics\IShader.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"
#include "..\DXBase\DXShaderBase.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Shader final : public IShader, public DXShaderBase
		{
			RE_IMPLEMENT_INTERFACE(DX12Shader);

		public:
			DX12Shader(IDevice* pDevice, const ShaderDesc* pDesc);
			~DX12Shader();

			D3D12_SHADER_BYTECODE GetD3D12ByteCode() const;

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(ShaderDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(const ShaderDesc* pDesc);

		private:
			DX12Device* m_Device;

			ShaderDesc m_Desc;

			IObject::CounterType m_References;
		};
	}
}

#endif