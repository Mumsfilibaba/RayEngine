#pragma once

#include "..\..\Include\Vulkan\VkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		template<typename T>
		struct VkObjData
		{
			T Data;
			int64 RefCount;
		};



		/////////////////////////////////////////////////////////////
		template<typename T>
		class VkObjBase
		{
		public:
			/////////////////////////////////////////////////////////////
			inline VkObjBase()
				: m_Data(nullptr)
			{
				m_Data = new VkObjData<T>();
			}



			/////////////////////////////////////////////////////////////
			inline ~VkObjBase()
			{
				if (m_Data != nullptr)
				{
					if (m_Data->RefCount > 1)
					{
						delete m_Data;
						m_Data = nullptr;
					}
				}
			}



			/////////////////////////////////////////////////////////////
			inline operator T()
			{
				return m_Data->Data;
			}



			/////////////////////////////////////////////////////////////
			inline void AddRef()
			{
				m_Data->RefCount++;
			}

		private:
			VkObjData<T>* m_Data;
		};
	}
}