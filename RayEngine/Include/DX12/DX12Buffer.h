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
#include "..\..\Include\Graphics\IBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Resource.h"
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Buffer final : public IBuffer, public DX12Resource
		{
			RE_IMPLEMENT_INTERFACE(DX12Buffer);

		public:
			DX12Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferDesc* pDesc);
			~DX12Buffer();

			inline DX12DescriptorHandle GetDX12DescriptorHandleSRVCBVUAV() const
			{
				return m_Views.SRVCBVUAV;
			}

			inline D3D12_VERTEX_BUFFER_VIEW GetD3D12VertexBufferView() const
			{
				return m_Views.Vertex;
			}

			inline D3D12_INDEX_BUFFER_VIEW GetD3D12IndexBufferView() const
			{
				return m_Views.Index;
			}

			void* Map(int32 subresource, RESOURCE_MAP_FLAG flag) override final;
			
			void Unmap() override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const ResourceData* pInitalData, const BufferDesc* pDesc);
			
			void CreateView(const BufferDesc* pDesc);


		private:
			DX12Device* m_Device;
			
			BUFFER_USAGE m_BufferType;
			
			union 
			{
				DX12DescriptorHandle SRVCBVUAV;
				D3D12_VERTEX_BUFFER_VIEW Vertex;
				D3D12_INDEX_BUFFER_VIEW Index;
			} m_Views;

			int32 m_MappedSubresource;

			IObject::CounterType m_References;
		};
	}
}

#endif