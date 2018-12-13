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
#include <Utilities/EngineUtilities.h>
#include <OpenGL/GLShader.h>
#include <OpenGL/GLDevice.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GLShader::GLShader(IDevice* pDevice, const ShaderDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_Shader(0),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<GLDevice*>(pDevice);

			Create(pDesc);
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
		void GLShader::GetDesc(ShaderDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLShader::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType GLShader::Release()
		{
			CounterType refs = --m_References;
			if (m_References < 1)
				delete this;

			return refs;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLShader::Create(const ShaderDesc* pDesc)
		{
			m_Desc = *pDesc;

			if (pDesc->SrcLang == SHADER_SOURCE_LANG_GLSL)
				CompileGLSL(pDesc->pSource);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void GLShader::CompileGLSL(const std::string& src)
		{
			m_Shader = glCreateShader(ShaderTypeToGL(m_Desc.Type));

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

				std::string message = "OpenGL: Could not compile shader.\n";
				if (len > 0)
				{
					std::vector<char> log;
					log.resize(result);
					glGetShaderInfoLog(m_Shader, len, &len, log.data());

					message += log.data();
				}

				LOG_ERROR(message);
			}
		}
	}
}