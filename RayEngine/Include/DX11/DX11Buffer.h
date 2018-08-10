#pragma once

#include "..\Graphics\IBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
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

			void* Map(int32 subresource) override final;
			void Unmap() override final;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const ResourceData* pInitalData, const BufferInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11Buffer* m_Resource;
			int32 m_ByteStride;
		};
	}
}

#endif