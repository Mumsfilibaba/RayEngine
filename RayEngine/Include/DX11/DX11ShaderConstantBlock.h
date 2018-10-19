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
#include "..\Graphics\IDeviceObject.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11ShaderConstantBlock final : public IDeviceObject
		{
			RE_IMPLEMENT_INTERFACE(DX11ShaderConstantBlock);

		public:
			DX11ShaderConstantBlock(DX11Device* pDevice, int32 numConstants);
			~DX11ShaderConstantBlock();

			void SetConstant(uint32 constant, uint32 index);

			ID3D11Buffer* GetD3D11Buffer() const;
			
			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(int32 numConstants);

		private:
			DX11Device* m_Device;
			ID3D11Buffer* m_ConstantBuffer;
			
			std::vector<uint32> m_Constants;
			IObject::CounterType mReferences;
		};
	}
}

#endif