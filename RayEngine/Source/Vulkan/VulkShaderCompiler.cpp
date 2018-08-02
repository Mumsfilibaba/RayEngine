#include <fstream>
#include <sstream>
#include "..\..\Include\Graphics\IFactory.h"
#include "..\..\Include\Vulkan\VulkShaderCompiler.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkShaderCompiler::VulkShaderCompiler(IFactory* pFactory, bool debug)
			: m_Factory(nullptr),
			m_ReferenceCount(0)
		{
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
			return ShaderByteCode();
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