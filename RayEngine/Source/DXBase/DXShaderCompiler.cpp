#include <fstream>
#include <sstream>
#include "..\..\Include\DXBase\DXShaderCompiler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Factory.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DXShaderCompiler::DXShaderCompiler(IFactory* pFactory, bool debug)
			: m_Factory(nullptr),
			m_Flags(0)
		{
			AddRef();
			m_Factory = reinterpret_cast<IFactory*>(pFactory->QueryReference());

			if (debug)
				m_Flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		}



		/////////////////////////////////////////////////////////////
		DXShaderCompiler::~DXShaderCompiler()
		{
			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		ShaderByteCode DXShaderCompiler::CompileFromFile(const std::string& fName, const std::string& fPath,
			const ShaderCompileInfo& info) const
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
		ShaderByteCode DXShaderCompiler::CompileFromString(const std::string& src, const ShaderCompileInfo& info) const
		{
			if (info.SrcLang == SHADER_SOURCE_LANG_GLSL)
				return ShaderByteCode();


			using namespace Microsoft::WRL;

			ComPtr<ID3DBlob> shader;
			ComPtr<ID3DBlob> error;

			if (FAILED(D3DCompile(src.c_str(), src.size(), 0, nullptr, nullptr, info.EntryPoint.c_str(), 
				GetShaderModel(info.Type).c_str(), m_Flags, 0, &shader, &error)))
			{
				std::string err = reinterpret_cast<const char*>(error->GetBufferPointer());
				return ShaderByteCode();
			}

			return ShaderByteCode(info.Type, info.SrcLang, reinterpret_cast<int8*>(shader->GetBufferPointer()),
				static_cast<int32>(shader->GetBufferSize()));
		}



		/////////////////////////////////////////////////////////////
		IFactory* DXShaderCompiler::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		std::string DXShaderCompiler::GetShaderModel(SHADER_TYPE type) const
		{
			switch (type)
			{
			case SHADER_TYPE_VERTEX: return "vs_5_0";
			case SHADER_TYPE_HULL: return "hs_5_0";
			case SHADER_TYPE_DOMAIN: return "ds_5_0";
			case SHADER_TYPE_GEOMETRY: return "gs_5_0";
			case SHADER_TYPE_PIXEL: return "ps_5_0";
			case SHADER_TYPE_COMPUTE: return "cs_5_0";
			}

			return std::string();
		}
	}
}

#endif