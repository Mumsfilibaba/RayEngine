#pragma once

#include "..\..\Include\Graphics\IRootLayout.h"
#include "GLCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class GLDevice;


		/////////////////////////////////////////////////////////////
		class GLRootLayout final : public IRootLayout
		{
			RE_IMPLEMENT_INTERFACE(GLRootLayout);

		public:
			GLRootLayout(IDevice* pDevice, const RootLayoutInfo& info);
			~GLRootLayout();

		private:
			void Create(const RootLayoutInfo& info);

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			GLDevice* m_Device;

			IObject::CounterType m_References;
		};
	}
}