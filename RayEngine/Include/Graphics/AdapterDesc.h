/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#pragma once
#include <string>
#include <cassert>
#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum ADAPTER_FLAGS : int32
		{
			ADAPTER_FLAGS_NONE = (1 << 0),
			ADAPTER_FLAGS_SOFTWARE = (1 << 1),
			ADAPTER_FLAGS_GEOMETRYSHADER = (1 << 2),
			ADAPTER_FLAGS_TESSELATIONSHADERS = (1 << 3),
			ADAPTER_FLAGS_SWAPCHAIN = (1 << 4),
			ADAPTER_FLAGS_COMPUTE = (1 << 5),
			ADAPTER_FLAGS_GRAPHICS = (1 << 6),
		};


		/*////////////////////////////////////////////////////////////

			Contains information about a adapter (graphics-card).

			VendorName - Name of the adapter's vendor
			
			ModelName - Name of the adapter

			ApiID - Identifer of the adapter based on the api.
			This can be an index of the adapters that the api is
			able to find on the system.

			DeviceID - This ID is the same for all apis because it
			is tied to the physical device (Exluding software 
			adapters)

			VendorID - This ID is the same for all apis because it
			is tied to the physical device (Exluding software
			adapters)

			Flags - Flags that is set based on what features that
			are available on the adapter.

			Limits - An anonymous struct that contains the maximum
			values that the adapter support.

		////////////////////////////////////////////////////////////*/
		struct AdapterDesc
		{
		public:
			std::string VendorName;
			std::string ModelName;
			int32 ApiID;
			int32 DeviceID;
			int32 VendorID;
			int64 Flags;

			struct
			{
				struct
				{
					int32 Width;
				} Texture1D;

				struct
				{
					int32 Width;
					int32 Height;
				} Texture2D;

				struct
				{
					int32 Width;
					int32 Height;
					int32 Depth;
				} Texture3D;

				struct
				{
					int32 Width;
					int32 Height;
				} TextureCube;

				int32 RenderTargetCount;
			} Limits;

		public:
			static constexpr int32 AMDVendorID = 0x1002;
			static constexpr int32 ImgTecVendorID = 0x1010;
			static constexpr int32 NVIDIAVendorID = 0x10DE;
			static constexpr int32 ARMVendorID = 0x13B5;
			static constexpr int32 QualcommVendorID = 0x5143;
			static constexpr int32 INTELVendorID = 0x8086;
			static constexpr int32 MicrosoftVendorID = 0x1414;

		public:
			static std::string GetVendorString(int32 vendorID);
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct AdapterList
		{
		public:
			AdapterList(const AdapterList& other) = delete;
			AdapterList& operator=(const AdapterList& other) = delete;

		public:
			AdapterList();
			AdapterList(int32 count);
			AdapterList(AdapterList&& other);
			~AdapterList();

			AdapterDesc& operator[](int32 index);
			AdapterList& operator=(AdapterList&& other);

		public:
			AdapterDesc* pAdapters = nullptr;
			int32 Count = 0;
		};
	}
}