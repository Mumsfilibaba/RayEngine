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
#include <Graphics/IShader.h>
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLShader final : public IShader
		{
			friend class GLPipelineState;

			RE_IMPLEMENT_INTERFACE(GLShader);

		public:
			GLShader(const ShaderDesc* pDesc);
			~GLShader();

			void GetDesc(ShaderDesc* pDesc) const override final;

			CounterType Release() override final;

			CounterType AddRef() override final;

		private:
			inline int32 GetGLShaderID() const
			{
				return m_Shader;
			}

			inline void ReleaseShader()
			{
				if (glIsShader(m_Shader))
				{
					glDeleteShader(m_Shader);
					m_Shader = 0;
				}
			}

			void Create(const ShaderDesc* pDesc);

			void CompileGLSL(const std::string& src);

		private:
			ShaderDesc m_Desc;

			int32 m_Shader;

			CounterType m_References;
		};
	}
}