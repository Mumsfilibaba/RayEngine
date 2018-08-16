#pragma once

#include "ITexture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		struct RenderTargetViewInfo
		{
			std::string Name = "";
			FORMAT Format = FORMAT_UNKNOWN;
			VIEWDIMENSION ViewDimension = VIEWDIMENSION_UNKNOWN;
			const IResource* pResource = nullptr;

			union
			{
				struct
				{
				} Buffer;

				struct
				{
				} Texture1D;

				struct
				{
				} Texture1DArray;

				struct
				{
				} Texture2D;

				struct
				{
				} Texture2DMS;

				struct
				{
				} Texture2DArray;

				struct
				{
				} Texture2DMSArray;

				struct
				{
				} Texture3D;
			};
		};



		/////////////////////////////////////////////////////////////
		class IRenderTargetView : public IDeviceObject
		{
		public:
			IRenderTargetView(IRenderTargetView&& other) = delete;
			IRenderTargetView(const IRenderTargetView& other) = delete;
			IRenderTargetView& operator=(IRenderTargetView&& other) = delete;
			IRenderTargetView& operator=(const IRenderTargetView& other) = delete;

		public:
			IRenderTargetView() {}
			virtual ~IRenderTargetView() {}
		};
	}
}