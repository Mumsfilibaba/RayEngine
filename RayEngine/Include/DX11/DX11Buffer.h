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
#include <Graphics\IBuffer.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class DX11Device;
		class DX11DeviceContext;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Buffer final : public IBuffer
		{
			RE_IMPLEMENT_INTERFACE(DX11Buffer);

		public:
			DX11Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferDesc* pDesc);
			~DX11Buffer();

			inline int32 GetByteStride() const
			{
				return m_Desc.ByteStride;
			}
			
			inline ID3D11Buffer* GetD3D11Buffer() const
			{
				return m_Resource;
			}

			void* Map(int32 subresource, RESOURCE_MAP_FLAG flag) override final;
			
			void Unmap() override final;

			void GetDesc(BufferDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const ResourceData* pInitalData, const BufferDesc* pDesc);

		private:
			DX11Device* m_Device;
			DX11DeviceContext* m_Context;
			ID3D11Buffer* m_Resource;
			
			BufferDesc m_Desc;

			CounterType m_References;
		};
	}
}

#endif