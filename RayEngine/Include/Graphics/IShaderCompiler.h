#pragma once

#include "IShader.h"
#include <cstring>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct ShaderByteCode
		{
		public:
			/////////////////////////////////////////////////////////////
			inline ShaderByteCode()
				: Bytes(nullptr),
				Size(0)
			{
			}



			/////////////////////////////////////////////////////////////
			inline ShaderByteCode(const ShaderByteCode& other)
				: Bytes(nullptr),
				Size(0)
			{
				Size = other.Size;

				Bytes = new int8[Size];
				memcpy(Bytes, other.Bytes, Size * sizeof(int8));
			}



			/////////////////////////////////////////////////////////////
			inline ShaderByteCode(ShaderByteCode&& other)
				: Bytes(other.Bytes),
				Size(other.Size)
			{
				other.Bytes = nullptr;
				other.Size = 0;
			}



			/////////////////////////////////////////////////////////////
			inline ~ShaderByteCode()
			{
				if (Bytes != nullptr)
				{
					delete[] Bytes;
					Bytes = nullptr;
				}
			}



			/////////////////////////////////////////////////////////////
			inline ShaderByteCode& operator=(const ShaderByteCode& other)
			{
				return *this;
			}



			/////////////////////////////////////////////////////////////
			inline ShaderByteCode& operator=(ShaderByteCode&& other)
			{
				return *this;
			}

		public:
			int8* Bytes;
			int32 Size;
		};



		/////////////////////////////////////////////////////////////
		class IShaderCompiler
		{
		public:
			virtual ~IShaderCompiler() {}

			virtual ShaderByteCode CompileFromFile() const = 0;
			virtual ShaderByteCode CompileFromString() const = 0;
		};
	}
}