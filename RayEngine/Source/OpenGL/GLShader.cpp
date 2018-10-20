#include <string>
#include "..\..\Include\Utilities\EngineUtilities.h"
#include "..\..\Include\OpenGL\GLShader.h"
#include "..\..\Include\OpenGL\GLDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLShader::GLShader(IDevice* pDevice, const ShaderInfo& info)
			: m_Device(nullptr),
			m_Type(SHADER_TYPE_UNKNOWN),
			m_Shader(0),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(info);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLShader::~GLShader()
		{
			if (glIsShader(m_Shader))
			{
				glDeleteShader(m_Shader);
				m_Shader = 0;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SHADER_TYPE GLShader::GetType() const
		{
			return m_Type;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLShader::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLShader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLShader::GetReferenceCount() const
		{
			return mReferences;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLShader::Release()
		{
			IObject::CounterType refs = --mReferences;
			if (mReferences < 1)
				delete this;

			return refs;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType GLShader::AddRef()
		{
			return ++mReferences;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLShader::Create(const ShaderInfo& info)
		{
			m_Type = info.Type;

			if (info.SrcLang == SHADER_SOURCE_LANG_GLSL)
				CompileGLSL(info.Source);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLShader::CompileGLSL(const std::string& src)
		{
			using namespace System;

			m_Shader = glCreateShader(ShaderTypeToGL(m_Type));

			const char* pSrc = src.c_str();
			int32 len = static_cast<int32>(src.size());

			glShaderSource(m_Shader, 1, &pSrc, &len);
			glCompileShader(m_Shader);

			int32 result = GL_TRUE;
			glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &result);
			if (result != GL_TRUE)
			{
				int32 len = 0;
				glGetShaderiv(m_Shader, GL_INFO_LOG_LENGTH, &len);

				std::vector<char> log;
				log.resize(result);
				glGetShaderInfoLog(m_Shader, len, &len, log.data());

				std::string message = "OpenGL: Could not compile shader.\n";
				message += log.data();
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, message);
			}
		}
	}
}