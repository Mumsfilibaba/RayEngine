#include "..\..\Include\DX12\DXShaderCompiler.h"
#include <fstream>
#include <sstream>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DXShaderCompiler::DXShaderCompiler(bool debug)
			: m_Flags(0)
		{
			if (debug)
				m_Flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		}



		/////////////////////////////////////////////////////////////
		DXShaderCompiler::DXShaderCompiler(DXShaderCompiler&& other)
			: m_Flags(other.m_Flags)
		{
			other.m_Flags = 0;
		}



		/////////////////////////////////////////////////////////////
		DXShaderCompiler& DXShaderCompiler::operator=(DXShaderCompiler&& other)
		{
			if (this != &other)
			{
				m_Flags = other.m_Flags;

				other.m_Flags = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		DXShaderCompiler::~DXShaderCompiler()
		{
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
		IReferenceCounter* DXShaderCompiler::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DXShaderCompiler::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DXShaderCompiler::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DXShaderCompiler::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		std::string DXShaderCompiler::GetShaderModel(SHADERTYPE type) const
		{
			switch (type)
			{
			case SHADERTYPE_VERTEX: return "vs_5_0";
			case SHADERTYPE_HULL: return "hs_5_0";
			case SHADERTYPE_DOMAIN: return "ds_5_0";
			case SHADERTYPE_GEOMETRY: return "gs_5_0";
			case SHADERTYPE_PIXEL: return "ps_5_0";
			case SHADERTYPE_COMPUTE: return "cs_5_0";
			}

			return std::string();
		}
	}
}