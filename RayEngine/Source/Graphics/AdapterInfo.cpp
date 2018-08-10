#include "..\..\Include\Graphics\AdapterInfo.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		std::string AdapterInfo::GetVendorString(int32 vendorID)
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



		/////////////////////////////////////////////////////////////
		AdapterList::AdapterList()
			: pAdapters(nullptr),
			Count(0)
		{
		}



		/////////////////////////////////////////////////////////////
		AdapterList::AdapterList(int32 count)
			: pAdapters(nullptr),
			Count(count)
		{
			pAdapters = new AdapterInfo[count];
		}



		/////////////////////////////////////////////////////////////
		AdapterList::AdapterList(AdapterList&& other)
			: pAdapters(other.pAdapters),
			Count(other.Count)
		{
			other.pAdapters = nullptr;
			other.Count = 0;
		}



		/////////////////////////////////////////////////////////////
		AdapterList::~AdapterList()
		{
			if (pAdapters != nullptr)
			{
				delete[] pAdapters;
				pAdapters = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		AdapterInfo& AdapterList::operator[](int32 index)
		{
			assert(index < Count);
			return pAdapters[index];
		}



		/////////////////////////////////////////////////////////////
		AdapterList& AdapterList::operator=(AdapterList&& other)
		{
			if (this != &other)
			{
				pAdapters = other.pAdapters;
				Count = other.Count;

				other.pAdapters = nullptr;
				other.Count = 0;
			}

			return *this;
		}
	}
}