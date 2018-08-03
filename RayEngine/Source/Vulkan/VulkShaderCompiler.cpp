#include <fstream>
#include <sstream>
#include "..\..\Include\Graphics\IFactory.h"
#include "..\..\Include\Vulkan\VulkShaderCompiler.h"
#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkShaderCompiler::VulkShaderCompiler(IFactory* pFactory, bool debug)
			: m_Factory(nullptr)
		{
			static bool glslangInitialized = false;
			if (!glslangInitialized)
			{
				glslang::InitializeProcess();
				glslangInitialized = true;
			}
		}



		/////////////////////////////////////////////////////////////
		VulkShaderCompiler::~VulkShaderCompiler()
		{
			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode VulkShaderCompiler::CompileFromFile(const std::string& fName, const std::string& fPath, const ShaderCompileInfo& info) const
		{
			std::ifstream file(fPath + fName, std::ios::in);
			if (file.is_open())
			{
				//Load file
				std::stringstream buf;
				buf << file.rdbuf();
				std::string src = buf.str();

				//Close
				file.close();

				return CompileFromString(src, info);
			}

			return ShaderByteCode();
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode VulkShaderCompiler::CompileFromString(const std::string& src, const ShaderCompileInfo& info) const
		{
			EShLanguage shaderType = GetShaderType(info.Type);
			glslang::TShader shader(shaderType);
			 
			const char* srcString = src.c_str();
			shader.setStrings(&srcString, 1);

			int32 messages = EShMsgSpvRules | EShMsgVulkanRules;
			int32 clientInputSemanticsVersion = 100;
			glslang::EShClient client = glslang::EShClientVulkan;
			glslang::EShSource source = glslang::EShSourceNone;
			glslang::EShTargetClientVersion vulkanVersion = glslang::EShTargetVulkan_1_0;
			glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_0;

			if (info.SrcLang == SHADER_SOURCE_LANG_HLSL)
			{
				shader.setEnvTargetHlslFunctionality1();
				
				source = glslang::EShSourceHlsl;
				messages |= EShMsgReadHlsl;
			}
			else if (info.SrcLang == SHADER_SOURCE_LANG_GLSL)
			{
				source = glslang::EShSourceGlsl;
			}

			shader.setEnvInput(source, shaderType, client, clientInputSemanticsVersion);
			shader.setEnvClient(client, vulkanVersion);
			shader.setEnvTarget(glslang::EShTargetSpv, targetVersion);


			const int defaultVersion = 100;
			const TBuiltInResource defaultTBuiltInResource = {};

			std::string localPath = "";
			std::string preprocessed;

			DirStackFileIncluder includer;
			includer.pushExternalLocalDirectory(localPath);

			if (!shader.preprocess(&defaultTBuiltInResource, defaultVersion, ENoProfile, 
				false, false, static_cast<EShMessages>(messages), &preprocessed, includer))
			{
				const char* err = shader.getInfoLog();
				err = shader.getInfoDebugLog();
				return ShaderByteCode();
			}


			const char*	preprocessedString = preprocessed.c_str();
			shader.setStrings(&preprocessedString, 1);

			if (!shader.parse(&defaultTBuiltInResource, defaultVersion, false, static_cast<EShMessages>(messages)))
			{
				const char* err = shader.getInfoLog();
				err = shader.getInfoDebugLog();
				return ShaderByteCode();
			}


			glslang::TProgram program;
			program.addShader(&shader);

			if (!program.link(static_cast<EShMessages>(messages)))
			{
				const char* err = shader.getInfoLog();
				err = shader.getInfoDebugLog();
				return ShaderByteCode();
			}


			std::vector<uint32> spirV;
			spv::SpvBuildLogger logger;
			glslang::SpvOptions spvOptions;
			glslang::GlslangToSpv(*program.getIntermediate(shaderType), spirV, &logger, &spvOptions);

			return ShaderByteCode(info.Type, info.SrcLang,
				reinterpret_cast<int8*>(spirV.data()), static_cast<int32>(spirV.size()) * sizeof(uint32));
		}



		/////////////////////////////////////////////////////////////
		IFactory* VulkShaderCompiler::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		EShLanguage VulkShaderCompiler::GetShaderType(SHADER_TYPE type)
		{
			if (type == SHADER_TYPE_VERTEX)
				return EShLangVertex;
			else if (type == SHADER_TYPE_HULL)
				return EShLangTessControl;
			else if (type == SHADER_TYPE_DOMAIN)
				return EShLangTessEvaluation;
			else if (type == SHADER_TYPE_GEOMETRY)
				return EShLangGeometry;
			else if (type == SHADER_TYPE_PIXEL)
				return EShLangFragment;
			else if (type == SHADER_TYPE_COMPUTE)
				return EShLangCompute;

			return static_cast<EShLanguage>(0);
		}
	}
}