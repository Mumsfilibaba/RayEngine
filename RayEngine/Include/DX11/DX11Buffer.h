#pragma once

#include "..\Graphics\IBuffer.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Buffer : public IBuffer
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
			IDevice* GetDevice() const override final;

			void* Map(int32 subresource) override final;
			void Unmap() override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info);

		private:
			IDevice* m_Device;

			ID3D11Buffer* m_Resource;
			int32 m_ByteStride;

			mutable uint32 m_ReferenceCounter;
		};
	}
}

#endif