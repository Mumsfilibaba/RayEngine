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

#include <RayEngine.h>
#include <OpenGL/GLDevice.h>
#include <OpenGL/GLBuffer.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLBuffer::GLBuffer(IDevice* pDevice, const ResourceData* const pInitialData, const BufferDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_Buffer(0),
			m_SizeBytes(0),
			m_CpuAccess(0),
			m_GLBufferType(0),
			m_GLBufferUsage(0),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(pInitialData, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLBuffer::~GLBuffer()
		{
			if (glIsBuffer(m_Buffer))
			{
				glDeleteBuffers(1, &m_Buffer);
				m_Buffer = 0;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void* GLBuffer::Map(int32 subresource, RESOURCE_MAP_FLAG flag)
		{
			void* pGPUData = nullptr;

			glBindBuffer(m_GLBufferType, m_Buffer);

			if (flag == RESOURCE_MAP_FLAG_READ && (m_CpuAccess & GL_MAP_READ_BIT))
				pGPUData = glMapBufferRange(m_GLBufferType, 0, m_SizeBytes, GL_MAP_READ_BIT);
			else if (flag == RESOURCE_MAP_FLAG_WRITE && (m_CpuAccess & GL_MAP_WRITE_BIT))
				pGPUData = glMapBufferRange(m_GLBufferType, 0, m_SizeBytes, GL_MAP_WRITE_BIT);

			glBindBuffer(m_GLBufferType, 0);

			return pGPUData;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLBuffer::Unmap()
		{
			glBindBuffer(m_GLBufferType, m_Buffer);
			
			if (!glUnmapBuffer(m_GLBufferType))
			{
				LOG_ERROR("OpenGL: Failed to unmap buffer.");
			}

			glBindBuffer(m_GLBufferType, 0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLBuffer::GetDesc(BufferDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLBuffer::Release()
		{
			CounterType refs = --m_References;
			if (m_References < 1)
				delete this;

			return refs;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLBuffer::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLBuffer::Create(const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			uint32 type = BufferUsageToGL(pDesc->Type);
			uint32 usage = ResourceUsageToGL(pDesc->Usage);
			uint32 buffer = 0;
			uint32 cpuAccess = CPUAccessToGL(pDesc->CpuAccess);
			uint32 sizeBytes = pDesc->Count * pDesc->ByteStride;

			glGenBuffers(1, &buffer);
			glBindBuffer(type, buffer);

			glBufferData(type, sizeBytes, nullptr, usage);
			if (pInitialData != nullptr)
			{
				glBufferSubData(type, 0, pInitialData->WidthOrCount * pInitialData->ByteStride, pInitialData->pData);
			}

			int32 size = 0;
			glGetBufferParameteriv(type, GL_BUFFER_SIZE, &size);
			if (size < pDesc->ByteStride * pDesc->Count)
			{
				glBindBuffer(type, 0);
				
				glDeleteBuffers(1, &buffer);
				buffer = 0;

				LOG_ERROR("OpenGL: Failed to allocate enough memory for buffer.");
			}
			else
			{
				m_Desc = *pDesc;
			}

			glBindBuffer(type, 0);

			m_Buffer = buffer;
			m_CpuAccess = cpuAccess;
			m_SizeBytes = sizeBytes;
			m_GLBufferType = type;
			m_GLBufferUsage = usage;
		}
	}
}