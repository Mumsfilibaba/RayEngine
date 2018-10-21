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

#include "..\..\Include\Graphics\AdapterDesc.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string AdapterDesc::GetVendorString(int32 vendorID)
		{
			switch (vendorID)
			{
			case AdapterDesc::AMDVendorID:			return "AMD";
			case AdapterDesc::ImgTecVendorID:		return "ImgTec";
			case AdapterDesc::NVIDIAVendorID:		return "NVIDIA";
			case AdapterDesc::ARMVendorID:			return "ARM";
			case AdapterDesc::QualcommVendorID:		return "Qualcomm";
			case AdapterDesc::INTELVendorID:		return "INTEL";
			case AdapterDesc::MicrosoftVendorID:	return "Microsoft";
			default: return "Unknown Vendor";
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AdapterList::AdapterList()
			: pAdapters(nullptr),
			Count(0)
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AdapterList::AdapterList(int32 count)
			: pAdapters(nullptr),
			Count(count)
		{
			pAdapters = new AdapterDesc[count];
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AdapterList::AdapterList(AdapterList&& other)
			: pAdapters(other.pAdapters),
			Count(other.Count)
		{
			other.pAdapters = nullptr;
			other.Count = 0;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AdapterList::~AdapterList()
		{
			if (pAdapters != nullptr)
			{
				delete[] pAdapters;
				pAdapters = nullptr;
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		AdapterDesc& AdapterList::operator[](int32 index)
		{
			assert(index < Count);
			return pAdapters[index];
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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