#pragma once

#include "..\Graphics\IBuffer.h"
#include "..\OpenGL\GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLDevice;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLBuffer final : public IBuffer
		{
			RE_IMPLEMENT_INTERFACE(GLBuffer);

		public:
			GLBuffer(IDevice* pDevice, const ResourceData* const pInitialData, const BufferDesc* pDesc);
			~GLBuffer();

			inline uint32 GetGLBuffer() const
			{
				return m_Buffer;
			}

			void* Map(int32 subresource, RESOURCE_MAP_FLAG flag) override final;
			
			void Unmap() override final;

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(BufferDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(const ResourceData* const pInitialData, const BufferDesc* pDesc);

		private:
			GLDevice* m_Device;

			BufferDesc m_Desc;

			uint32 m_Buffer;
			uint32 m_SizeBytes;
			uint32 m_CpuAccess;
			uint32 m_GLBufferType;
			uint32 m_GLBufferUsage;

			IObject::CounterType m_References;
		};
	}
}