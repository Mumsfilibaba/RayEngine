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

#include "RayEngine.h"
#include "../../Include/System/Image.h"
#include "../../Include/Utilities/EngineUtilities.h"

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Image::Image(const void* pPixels, int32 width, int32 height, FORMAT format)
        : m_pPixels(nullptr),
        m_Height(0),
        m_Width(0),
        m_Format(FORMAT_UNKNOWN)
    {
        m_Height = height;
        m_Width = width;
        m_Format = format;

        int32 stride = FormatStride(format);
        int32 size = width * height * stride;
        m_pPixels = malloc(size);
        memcpy(m_pPixels, pPixels, size);
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Image::Image(Image&& other)
        : m_pPixels(other.m_pPixels),
        m_Height(other.m_Height),
        m_Width(other.m_Width),
        m_Format(other.m_Format)
    {
        other.m_pPixels = nullptr;
        other.m_Height = 0;
        other.m_Width = 0;
        other.m_Format = FORMAT_UNKNOWN;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Image::Image(const Image& other)
        : m_pPixels(nullptr),
        m_Height(other.m_Height),
        m_Width(other.m_Width),
        m_Format(other.m_Format)
    {
        int32 stride = FormatStride(m_Format);
        int32 size = m_Width * m_Height * stride;
        m_pPixels = malloc(size);
        memcpy(m_pPixels, other.m_pPixels, size);
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Image::~Image()
    {
        if (m_pPixels != nullptr)
        {
            free(m_pPixels);
            m_pPixels = nullptr;
        }
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Image::GetPixels(void** const ppPixels) const
    {
        if (m_pPixels == nullptr)
        {
            *ppPixels = nullptr;
            return;
        }

        int32 stride = FormatStride(m_Format);
        int32 size = m_Width * m_Height * stride;
        *ppPixels = malloc(size);
        memcpy(*ppPixels, m_pPixels, size);
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Image& Image::operator=(Image&& other)
    {
        if (this != &other)
        {   
            Release();

            m_pPixels = other.m_pPixels;
            m_Height = other.m_Height;
            m_Width = other.m_Width;
            m_Format = other.m_Format;
            
            other.m_pPixels = nullptr;
            other.m_Height = 0;
            other.m_Width = 0;
            other.m_Format = FORMAT_UNKNOWN;
        }

        return *this;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Image& Image::operator=(const Image& other)
    {
        if (this != &other)
        {
            Release();

            m_Height = other.m_Height;
            m_Width = other.m_Width;
            m_Format = other.m_Format;

            int32 stride = FormatStride(m_Format);
            int32 size = m_Width * m_Height * stride;
            m_pPixels = malloc(size);
            memcpy(m_pPixels, other.m_pPixels, size);
        }

        return *this;
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Image::Release()
    {
        if (m_pPixels != nullptr)
        {
            free(m_pPixels);
            m_pPixels = nullptr;
        }
    }
}