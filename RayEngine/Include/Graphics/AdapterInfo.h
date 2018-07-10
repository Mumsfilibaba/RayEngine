#pragma once

#include "..\Defines.h"
#include "..\Types.h"
#include <string>
#include <cassert>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
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



		/////////////////////////////////////////////////////////////
		struct AdapterInfo
		{
		public:
			std::string VendorName;
			std::string ModelName;
			int32 ApiID = 0;
			int32 DeviceID = 0;
			int32 VendorID = 0;
			int64 Flags = ADAPTER_FLAGS_NONE;

			struct
			{
				struct
				{
					int32 Width = 0;
				} Texture1D;

				struct
				{
					int32 Width = 0;
					int32 Height = 0;
				} Texture2D;

				struct
				{
					int32 Width = 0;
					int32 Height = 0;
					int32 Depth = 0;
				} Texture3D;

				struct
				{
					int32 Width = 0;
					int32 Height = 0;
				} TextureCube;

				int32 RenderTargetCount = 0;
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
			inline static std::string GetVendorString(int32 vendorID)
			{
				switch (vendorID)
				{
				case AdapterInfo::AMDVendorID:			return "AMD";
				case AdapterInfo::ImgTecVendorID:		return "ImgTec";
				case AdapterInfo::NVIDIAVendorID:		return "NVIDIA";
				case AdapterInfo::ARMVendorID:			return "ARM";
				case AdapterInfo::QualcommVendorID:		return "Qualcomm";
				case AdapterInfo::INTELVendorID:		return "INTEL";
				case AdapterInfo::MicrosoftVendorID:	return "Microsoft";
				default: return "Unknown Vendor";
				}
			}
		};



		/////////////////////////////////////////////////////////////
		struct AdapterList
		{
		public:
			AdapterList(const AdapterList& other) = delete;
			AdapterList& operator=(const AdapterList& other) = delete;


		public:
			/////////////////////////////////////////////////////////////
			inline AdapterList()
				: Adapters(nullptr),
				Count(0)
			{
			}



			/////////////////////////////////////////////////////////////
			inline AdapterList(int32 count)
				: Adapters(nullptr),
				Count(count)
			{
				Adapters = new AdapterInfo[count];
			}




			/////////////////////////////////////////////////////////////
			inline AdapterList(AdapterList&& other)
				: Adapters(other.Adapters),
				Count(other.Count)
			{
				other.Adapters = nullptr;
				other.Count = 0;
			}



			/////////////////////////////////////////////////////////////
			~AdapterList()
			{
				if (Adapters != nullptr)
				{
					delete[] Adapters;
					Adapters = nullptr;
				}
			}



			/////////////////////////////////////////////////////////////
			inline AdapterInfo& operator[](int32 index)
			{
				assert(index < Count);
				return Adapters[index];
			}



			/////////////////////////////////////////////////////////////
			inline AdapterList& operator=(AdapterList&& other)
			{
				if (this != &other)
				{
					Adapters = other.Adapters;
					Count = other.Count;

					other.Adapters = nullptr;
					other.Count = 0;
				}

				return *this;
			}


		public:
			AdapterInfo* Adapters = nullptr;
			int32 Count = 0;
		};
	}
}