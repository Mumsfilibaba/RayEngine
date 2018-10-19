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
#include "..\Graphics\IShader.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLDevice;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class GLShader final : public IShader
		{
			RE_IMPLEMENT_INTERFACE(GLShader);

		public:
			GLShader(IDevice* pDevice, const ShaderInfo& info);
			~GLShader();

			inline int32 GetGLShaderID() const
			{
				return m_Shader;
			}

			SHADER_TYPE GetType() const override final;

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(const ShaderInfo& info);

			void CompileGLSL(const std::string& src);

		private:
			GLDevice* m_Device;
			
			SHADER_TYPE m_Type;

			int32 m_Shader;

			IObject::CounterType mReferences;
		};
	}
}