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
			if (info.SrcLang == SHADER_SOURCE_LANG_GLSL)
				CompileGLSL(info.Source);
		}


		/////////////////////////////////////////////////////////////
		void GLShader::CompileGLSL(const std::string& src)
		{
		}
	}
}