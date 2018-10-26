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