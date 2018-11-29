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
#include "../Defines.h"
#include "../Types.h"

namespace RayEngine
{
    class Image final
    {
    public:
        Image(const void* pPixels, int32 width, int32 height, FORMAT format);
        Image(Image&& other);
        Image(const Image& other);
        ~Image();

        /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the height of the image
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        inline int32 GetHeight() const
        {
            return m_Height;   
        }

        /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the width of the image
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        inline int32 GetWidth() const
        {
            return m_Width;   
        }

        /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the format of the image
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        inline FORMAT GetFormat() const
        {
            return m_Format;   
        }

        /*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Returns the pixels of the image. The caller is responsible for releasing the memory when done with the array.

            ppPixels - A valid pointer to a void*.

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
        void GetPixels(void** const ppPixels) const;

        Image& operator=(Image&& other);
        Image& operator=(const Image& other);

    private:
        void Release();

    private:
        void* m_pPixels;
        int32 m_Height;
        int32 m_Width;
        FORMAT m_Format;
    };
}