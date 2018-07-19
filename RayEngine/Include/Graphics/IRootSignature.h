#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum PARAMETER_TYPE : int32
		{
			PARAMETER_TYPE_UNKNOWN = 0,
			PARAMETER_TYPE_DESCRIPTOR_TABLE = 1,
			PARAMETER_TYPE_VIEW = 2,
			PARAMETER_TYPE_CONSTANT = 3
		};



		/////////////////////////////////////////////////////////////
		enum VIEW_TYPE : int32
		{
			VIEW_TYPE_UNKNOWN = 0,
			VIEW_TYPE_UNIFORMBUFFER = 1,
			VIEW_TYPE_TEXTURE = 2,
			VIEW_TYPE_SAMPLER = 3,
		};



		/////////////////////////////////////////////////////////////
		enum SHADER_VISIBILITY : int32
		{
			SHADER_VISIBILITY_UNKNOWN = 0,
			SHADER_VISIBILITY_VERTEX = 1,
			SHADER_VISIBILITY_HULL = 2,
			SHADER_VISIBILITY_DOMAIN = 3,
			SHADER_VISIBILITY_GEOMETRY = 4,
			SHADER_VISIBILITY_PIXEL = 5,
			SHADER_VISIBILITY_ALL = 6,
		};



		/////////////////////////////////////////////////////////////
		enum ROOT_SIGNATURE_VISIBILITY : int32
		{
			ROOT_SIGNATURE_VISIBILITY_UNKNOWN = 0,
			ROOT_SIGNATURE_VISIBILITY_INPUT_LAYOUT = (1 << 0),
			ROOT_SIGNATURE_VISIBILITY_VERTEX_SHADER = (1 << 1),
			ROOT_SIGNATURE_VISIBILITY_HULL_SHADER = (1 << 2),
			ROOT_SIGNATURE_VISIBILITY_DOMAIN_SHADER = (1 << 3),
			ROOT_SIGNATURE_VISIBILITY_GEOMETRY_SHADER = (1 << 4),
			ROOT_SIGNATURE_VISIBILITY_PIXEL_SHADER = (1 << 5),
		};



		/////////////////////////////////////////////////////////////
		struct ShaderParameter
		{
			PARAMETER_TYPE VariableType = PARAMETER_TYPE_UNKNOWN;
			SHADER_VISIBILITY ShaderVisibility = SHADER_VISIBILITY_UNKNOWN;
			int32 ShaderRegister = 0;
			
			union
			{
				struct
				{
					VIEW_TYPE ViewType = VIEW_TYPE_UNKNOWN;
					int32 Count = 0;
				} DescriptorTable;

				struct
				{
					VIEW_TYPE ViewType = VIEW_TYPE_UNKNOWN;
				} View;

				struct
				{
					int32 Count = 0;
				} Constant;
			};
		};



		/////////////////////////////////////////////////////////////
		struct RootSignatureInfo
		{
			ShaderParameter* Parameters = nullptr;
			int32 ParameterCount = 0;
			ROOT_SIGNATURE_VISIBILITY RootSignatureVisibility = ROOT_SIGNATURE_VISIBILITY_UNKNOWN;
		};



		/////////////////////////////////////////////////////////////
		class IRootSignature
		{
		public:
			IRootSignature(IRootSignature&& other) = delete;
			IRootSignature(const IRootSignature& other) = delete;
			IRootSignature& operator=(IRootSignature&& other) = delete;
			IRootSignature& operator=(const IRootSignature& other) = delete;

		public:
			IRootSignature() {}
			virtual ~IRootSignature() {}
		};
	}
}