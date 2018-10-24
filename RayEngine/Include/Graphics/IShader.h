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
		struct ShaderDesc
		{
			std::string Name;
			std::string EntryPoint;
			std::string Source;
			SHADER_TYPE Type;
			SHADER_SOURCE_LANG SrcLang;
			int32 Flags;
		};
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IShader : public IDeviceObject
		{
			RE_INTERFACE(IShader);

		public:
			IShader() {}
			~IShader() {}

			/*////////////////////////////////////////////////////////////

				Retrives the descriptor that was used to create the
				object.

				pDesc - A valid pointer to a Desc

			////////////////////////////////////////////////////////////*/
			virtual void GetDesc(ShaderDesc* pDesc) const = 0;
		};
	}
}