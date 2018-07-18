#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct RootSignatureInfo
		{
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