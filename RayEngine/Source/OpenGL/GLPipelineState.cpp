#include "..\..\Include\OpenGL\GLDevice.h"
#include "..\..\Include\OpenGL\GLShader.h"
#include "..\..\Include\OpenGL\GLPipelineState.h"

namespace RayEngine
{
	namespace Graphics
	{
		GLPipelineState::GLPipelineState(IDevice* pDevice, const PipelineStateInfo& info)
			: m_Device(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(info);
		}


		/////////////////////////////////////////////////////////////
		GLPipelineState::~GLPipelineState()
		{
			if (glIsProgram(m_Program))
			{
				glDeleteProgram(m_Program);
			}
		}


		/////////////////////////////////////////////////////////////
		PIPELINE_TYPE GLPipelineState::GetPipelineType() const
		{
			return m_Type;
		}


		/////////////////////////////////////////////////////////////
		void GLPipelineState::SetName(const std::string& name)
		{
			//Not relevant
		}


		/////////////////////////////////////////////////////////////
		void GLPipelineState::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLPipelineState::GetReferenceCount() const
		{
			return m_References;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLPipelineState::Release()
		{
			IObject::CounterType refs = --m_References;
			if (refs < 1)
				delete this;

			return refs;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLPipelineState::AddRef()
		{
			return ++m_References;
		}


		/////////////////////////////////////////////////////////////
		void GLPipelineState::Create(const PipelineStateInfo& info)
		{
			if (info.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsPipeline(info);
			else if (info.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputePipeline(info);
		}


		/////////////////////////////////////////////////////////////
		void GLPipelineState::CreateGraphicsPipeline(const PipelineStateInfo& info)
		{
			m_Program = glCreateProgram();
			if (info.GraphicsPipeline.pVertexShader != nullptr)
			{
				m_VS = info.GraphicsPipeline.pVertexShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_VS->GetGLShaderID());
			}
			if (info.GraphicsPipeline.pHullShader != nullptr)
			{
				m_HS = info.GraphicsPipeline.pHullShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_HS->GetGLShaderID());
			}
			if (info.GraphicsPipeline.pDomainShader != nullptr)
			{
				m_DS = info.GraphicsPipeline.pDomainShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_DS->GetGLShaderID());
			}
			if (info.GraphicsPipeline.pGeometryShader != nullptr)
			{
				m_GS = info.GraphicsPipeline.pGeometryShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_GS->GetGLShaderID());
			}
			if (info.GraphicsPipeline.pPixelShader != nullptr)
			{
				m_PS = info.GraphicsPipeline.pPixelShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_PS->GetGLShaderID());
			}

			LinkShaders();
		}


		/////////////////////////////////////////////////////////////
		void GLPipelineState::CreateComputePipeline(const PipelineStateInfo& info)
		{
			m_Program = glCreateProgram();
			if (info.GraphicsPipeline.pGeometryShader != nullptr)
			{
				m_GS = info.GraphicsPipeline.pGeometryShader->QueryReference<GLShader>();
				glAttachShader(m_Program, m_GS->GetGLShaderID());
			}

			LinkShaders();
		}


		/////////////////////////////////////////////////////////////
		void GLPipelineState::LinkShaders()
		{
			using namespace System;

			glLinkProgram(m_Program);

			int32 result = GL_TRUE;
			glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
			if (result != GL_TRUE)
			{
				int32 len = 0;
				glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &len);

				std::vector<char> log;
				log.resize(len);
				glGetProgramInfoLog(m_Program, len, &len, log.data());

				std::string message = "OpenGL: Could not link program.\n";
				message += log.data();
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, message);
			}
		}
	}
}