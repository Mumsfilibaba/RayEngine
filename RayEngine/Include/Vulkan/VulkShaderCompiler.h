#pragma once

#include "..\Graphics\IShaderCompiler.h"
#include <glslang/Public/ShaderLang.h>

namespace RayEngine
{
	namespace Graphics
	{
		class VulkShaderCompiler : public IShaderCompiler
		{
		public:
			VulkShaderCompiler(const VulkShaderCompiler& other) = delete;
			VulkShaderCompiler& operator=(const VulkShaderCompiler& other) = delete;
			VulkShaderCompiler(VulkShaderCompiler&& other) = delete;
			VulkShaderCompiler& operator=(VulkShaderCompiler&& other) = delete;

		public:
			VulkShaderCompiler(IFactory* pFactory, bool debug);
			~VulkShaderCompiler();

			ShaderByteCode CompileFromFile(const std::string& fName, const std::string& fPath, const ShaderCompileInfo& info) const override final;
			ShaderByteCode CompileFromString(const std::string& src, const ShaderCompileInfo& info) const override final;

			IFactory* GetFactory() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			static EShLanguage GetShaderType(SHADER_TYPE type);

		private:
			IFactory* m_Factory;

			int32 m_Flags;

			mutable uint32 m_ReferenceCount;
		};
	}
}