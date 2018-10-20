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
#include "ISampler.h"

#if defined(max)
#undef max
#endif

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IBuffer;
		class ITexture;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum SHADER_SOURCE_LANG : int32
		{
			SHADER_SOURCE_LANG_UNKNOWN = 0,
			SHADER_SOURCE_LANG_GLSL = 1,
			SHADER_SOURCE_LANG_HLSL = 2,
		};



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum SHADER_FLAGS : int32
		{
			SHADER_FLAGS_NONE = 0,
			SHADER_FLAGS_DEBUG = (1 << 0),
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct ShaderInfo
		{
			std::string Name = "";
			std::string Source = "";
			std::string EntryPoint = "main";
			SHADER_TYPE Type = SHADER_TYPE_UNKNOWN;
			SHADER_SOURCE_LANG SrcLang = SHADER_SOURCE_LANG_UNKNOWN;
			int32 Flags =
#if defined(RE_DEBUG)
				SHADER_FLAGS_DEBUG;
#else
				SHADER_FLAGS_NONE;
#endif
		};

		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IShader : public IDeviceObject
		{
			RE_INTERFACE(IShader);

		public:
			IShader() {}
			~IShader() {}

			//Retrives the shadertype
			virtual SHADER_TYPE GetType() const = 0;
		};
	}
}