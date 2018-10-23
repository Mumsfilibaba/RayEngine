#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLBuffer.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLBuffer::GLBuffer(IDevice* pDevice, const ResourceData* const pInitialData, const BufferDesc* pDesc)
			: m_Device(nullptr),
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

			if (RESOURCE_MAP_FLAG_READ && m_CpuAccess & GL_MAP_READ_BIT)
				pGPUData = glMapBufferRange(m_GLBufferType, 0, m_SizeBytes, GL_MAP_READ_BIT);
			else if (RESOURCE_MAP_FLAG_WRITE && m_CpuAccess & GL_MAP_WRITE_BIT)
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
				m_Device->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, "OpenGL: Failed to unmap buffer.");
			}

			glBindBuffer(m_GLBufferType, 0);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLBuffer::SetName(const std::string& name)
		{
			//Not relevant at the moment
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLBuffer::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLBuffer::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLBuffer::Release()
		{
			IObject::CounterType refs = --m_References;
			if (m_References < 1)
				delete this;

			return refs;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLBuffer::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLBuffer::Create(const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			using namespace System;

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

				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "OpenGL: Failed to allocate enough memory for buffer.");
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