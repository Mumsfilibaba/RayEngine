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
#include "IDeviceObject.h"

#if defined(max)
#undef max
#endif

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IBuffer;
		class ITexture;
		class ISampler;



		/////////////////////////////////////////////////////////////
		enum STATIC_SAMPLER_BORDER_COLOR : int32
		{
			STATIC_SAMPLER_BORDER_COLOR_UNKNOWN = 0,
			STATIC_SAMPLER_BORDER_COLOR_TRANSPARENT_BLACK = 1,
			STATIC_SAMPLER_BORDER_COLOR_OPAQUE_BLACK = 2,
			STATIC_SAMPLER_BORDER_COLOR_OPAQUE_WHITE = 3,
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_USAGE : int32
		{
			SHADER_USAGE_UNKNOWN = 0,
			SHADER_USAGE_DYNAMIC = 1,
			SHADER_USAGE_STATIC = 2,
		};



		/////////////////////////////////////////////////////////////
		enum VARIABLE_TYPE : int32
		{
			VARIABLE_TYPE_UNKNOWN = 0,
			VARIABLE_TYPE_UNIFORMBUFFER = 1,
			VARIABLE_TYPE_UNORDERED_ACCESS = 2,
			VARIABLE_TYPE_TEXTURE = 3,
			VARIABLE_TYPE_SAMPLER = 4,
			VARIABLE_TYPE_SHADER_CONSTANTS = 5,
		};



		/////////////////////////////////////////////////////////////
		enum ROOT_LAYOUT_FLAGS : int32
		{
			ROOT_LAYOUT_FLAG_NONE = 0,
			ROOT_LAYOUT_FLAG_DISABLE_INPUT_LAYOUT = (1 << 0)
		};



		/*////////////////////////////////////////////////////////////
		
		ShaderVariables defines a variable in the shader. This
		can be a texture, buffer or sampler.

		Type - Type of variable - Is this a sampler, texture, 
		unorderedaccess- or constant-buffer

		ShaderUsage - The usage of the variable. Variables that won't 
		be updated between drawcalls should be static. This puts them 
		into a descriptortable in the rootlayout (In apis that uses
		descriptor tables i.e D3D12 and vulkan). Dynamic variables gets
		put directly into the rootlayout. Dynamic variables are those 
		that may be updated between drawcalls. Since this requries the
		driver to allocate space for a descriptor heap, it is better 
		for performance if these descriptors are stored directly in the
		rootlayout to avoid the allocation.

		ShaderRegister - Register in the shader that the resource is 
		mapped to.

		ShaderSpace - ShaderSpace that the resource will be
		mapped to. This makes it posible to have different
		resources with the same registervalue, but different
		ShaderSpaces.

		////////////////////////////////////////////////////////////*/
		struct ShaderVariable
		{
			SHADER_TYPE ShaderStage = SHADER_TYPE_UNKNOWN;
			VARIABLE_TYPE Type = VARIABLE_TYPE_UNKNOWN;
			SHADER_USAGE ShaderUsage = SHADER_USAGE_UNKNOWN;
			int32 ShaderRegister = 0;
			int32 ShaderSpace = 0;
			int32 ConstantCount = 0;
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct StaticSampler
		{
			SHADER_TYPE ShaderStage = SHADER_TYPE_UNKNOWN;
			SAMPLER_FILTER_MODE FilterMode = SAMPLER_FILTER_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressU = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressV = SAMPLER_ADRESS_MODE_UNKNOWN;
			SAMPLER_ADRESS_MODE AdressW = SAMPLER_ADRESS_MODE_UNKNOWN;
			float MipLODBias = 0.0f;
			float MinLOD = 0.0f;
			float MaxLOD = std::numeric_limits<float>::max();
			uint32 MaxAnistropy = 16;
			COMPARISON_FUNC ComparisonFunc = COMPARISON_FUNC_UNKNOWN;
			STATIC_SAMPLER_BORDER_COLOR BorderColor = STATIC_SAMPLER_BORDER_COLOR_UNKNOWN;
			int32 ShaderRegister = 0;
			int32 ShaderSpace = 0;
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct RootLayoutInfo
		{
			std::string Name;
			PIPELINE_TYPE PipelineType;
			int32 Flags;
			ShaderVariable* pVariables;
			int32 VariableCount;
			StaticSampler* pStaticSamplers;
			int32 SamplerCount;
		};



		/////////////////////////////////////////////////////////////
		class IRootLayout : public IDeviceObject
		{
			RE_INTERFACE(IRootLayout);

		public:
			IRootLayout() {}
			~IRootLayout() {}
		};
	}
}