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
#include "..\Graphics\IBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;
		class DX11DeviceContext;



		/////////////////////////////////////////////////////////////
		class DX11Buffer final : public IBuffer
		{
		public:
			DX11Buffer(const DX11Buffer& other) = delete;
			DX11Buffer& operator=(const DX11Buffer& other) = delete;
			DX11Buffer(DX11Buffer&& other) = delete;
			DX11Buffer& operator=(DX11Buffer&& other) = delete;

		public:
			DX11Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);
			~DX11Buffer();

			int32 GetByteStride() const;
			
			ID3D11Buffer* GetD3D11Buffer() const;

			void* Map(int32 subresource, RESOURCE_MAP_FLAG flag) override final;
			
			void Unmap() override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const ResourceData* pInitalData, const BufferInfo& info);

		private:
			DX11Device* m_Device;
			DX11DeviceContext* m_Context;
			ID3D11Buffer* m_Resource;
			
			int32 m_ByteStride;

			IObject::CounterType m_References;
		};
	}
}

#endif