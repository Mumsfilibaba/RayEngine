#include <string>
#include "..\..\Include\Utilities\EngineUtilities.h"
#include "..\..\Include\OpenGL\GLShader.h"
#include "..\..\Include\OpenGL\GLDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		GLShader::GLShader(IDevice* pDevice, const ShaderInfo& info)
			: m_Device(nullptr),
			m_Type(SHADER_TYPE_UNKNOWN),
			m_Shader(0),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(info);
		}


		/////////////////////////////////////////////////////////////
		GLShader::~GLShader()
		{
			if (glIsShader(m_Shader))
			{
				glDeleteShader(m_Shader);
				m_Shader = 0;
			}
		}


		/////////////////////////////////////////////////////////////
		SHADER_TYPE GLShader::GetType() const
		{
			return m_Type;
		}


		/////////////////////////////////////////////////////////////
		void GLShader::SetName(const std::string& name)
		{
			//Not relevant
		}


		/////////////////////////////////////////////////////////////
		void GLShader::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<GLDevice>();
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLShader::GetReferenceCount() const
		{
			return m_References;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLShader::Release()
		{
			IObject::CounterType refs = --m_References;
			if (m_References < 1)
				delete this;

			return refs;
		}


		/////////////////////////////////////////////////////////////
		IObject::CounterType GLShader::AddRef()
		{
			return ++m_References;
		}


		/////////////////////////////////////////////////////////////
		void GLShader::Create(const ShaderInfo& info)
		{
			m_Type = info.Type;


			std::string source;
			if (info.FilePath.size() < 2)
				source = info.Source;
			else 
				source = ReadFullFile(info.Source, info.FilePath);


			if (info.SrcLang == SHADER_SOURCE_LANG_GLSL)
				CompileGLSL(source);
		}


		/////////////////////////////////////////////////////////////
		void GLShader::CompileGLSL(const std::string& src)
		{
			using namespace System;

			m_Shader = glCreateShader(ShaderTypeToGL(m_Type));

			const char* pSrc = src.c_str();
			int32 len = src.size();

			glShaderSource(m_Shader, 1, &pSrc, &len);
			glCompileShader(m_Shader);

			int32 result = GL_TRUE;
			glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &result);
			if (result != GL_TRUE)
			{
				std::vector<char> log;
				glGetShaderiv(m_Shader, GL_INFO_LOG_LENGTH, &result);
				log.resize(result);

				int32 len = 0;
				glGetShaderInfoLog(m_Shader, result, &len, log.data());

				std::string message = "OpenGL: Could not compile shader.\n";
				message += log.data();
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, message);
			}
		}
	}
}