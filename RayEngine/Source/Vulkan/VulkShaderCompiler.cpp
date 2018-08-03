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
			: m_Factory(nullptr),
			m_ReferenceCount(0)
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
			EShLanguage shaderType = EShLangCount;
			if (info.Type == SHADER_TYPE_VERTEX)
				shaderType = EShLangVertex;
			else if (info.Type == SHADER_TYPE_HULL)
				shaderType = EShLangTessControl;
			else if (info.Type == SHADER_TYPE_DOMAIN)
				shaderType = EShLangTessEvaluation;
			else if (info.Type == SHADER_TYPE_GEOMETRY)
				shaderType = EShLangGeometry;
			else if (info.Type == SHADER_TYPE_PIXEL)
				shaderType = EShLangFragment;
			else if (info.Type == SHADER_TYPE_COMPUTE)
				shaderType = EShLangCompute;


			const char* srcString = src.c_str();
			glslang::TShader shader(shaderType);
			shader.setStrings(&srcString, 1);


			glslang::EShSource source = static_cast<glslang::EShSource>(0);
			EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

			if (info.SrcLang == SHADER_SOURCE_LANG_HLSL)
			{
				shader.setEnvTargetHlslFunctionality1();
				
				source = glslang::EShSourceHlsl;
				messages = static_cast<EShMessages>(messages | EShMsgReadHlsl);
			}
			else if (info.SrcLang == SHADER_SOURCE_LANG_GLSL)
			{
				source = glslang::EShSourceGlsl;
			}


			int clientInputSemanticsVersion = 100;
			shader.setEnvInput(source, shaderType, glslang::EShClientVulkan, clientInputSemanticsVersion);

			glslang::EShTargetClientVersion vulkanVersion = glslang::EShTargetVulkan_1_0;
			shader.setEnvClient(glslang::EShClientVulkan, vulkanVersion);

			glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_0;
			shader.setEnvTarget(glslang::EShTargetSpv, targetVersion);


			const int defaultVersion = 100;
			const TBuiltInResource defaultTBuiltInResource = {};


			std::string localPath = "";
			DirStackFileIncluder includer;
			includer.pushExternalLocalDirectory(localPath);


			std::string preprocessed;		
			if (!shader.preprocess(&defaultTBuiltInResource, defaultVersion, ENoProfile, false, false, messages, &preprocessed, includer))
			{
				const char* err = shader.getInfoLog();
				err = shader.getInfoDebugLog();
				return ShaderByteCode();
			}


			const char*	preprocessedString = preprocessed.c_str();
			shader.setStrings(&preprocessedString, 1);


			if (!shader.parse(&defaultTBuiltInResource, defaultVersion, false, messages))
			{
				//std::cout << "GLSL Parsing Failed for: " << filename << std::endl;
				const char* err = shader.getInfoLog();
				err = shader.getInfoDebugLog();
				return ShaderByteCode();
			}


			glslang::TProgram program;
			program.addShader(&shader);

			if (!program.link(messages))
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
		IReferenceCounter* VulkShaderCompiler::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VulkShaderCompiler::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VulkShaderCompiler::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VulkShaderCompiler::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}
	}
}