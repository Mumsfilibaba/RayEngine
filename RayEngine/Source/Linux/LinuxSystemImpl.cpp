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


#include "../../Include/System/System.h"

#if defined(RE_PLATFORM_LINUX)
#include "../../Include/Utilities/StringUtilities.h"
#include "../../Include/Utilities/EngineUtilities.h"
#include "../../Include/OpenGL/GLDeviceLinux.h"
#include "../../Include/OpenGL/GLSwapchainLinux.h"
#include "../../Include/Linux/LinuxWindowImpl.h"
#include "Linux.h"

namespace RayEngine
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void QuerySystemDesc(SystemDesc* pDesc)
    {
        ::Display* pDisplay = GetDisplay();

        pDesc->Platform = PLATFORM_LINUX;

        Screen* pScreen = ScreenOfDisplay(pDisplay, DefaultScreen(pDisplay)); 
        pDesc->ScreenWidth = pScreen->width;
        pDesc->ScreenHeight = pScreen->height;

        ReleaseDisplay(pDisplay);
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void SendQuitMessage(int32 exitCode)
    {
        
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CreateWindow(IWindow** ppWindow, WindowDesc* pDesc)
    {
        *ppWindow = new LinuxWindowImpl(pDesc);
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CreateDevice(Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDesc, GRAPHICS_API api)
    {
        using namespace Graphics;

        if (api == GRAPHICS_API_OPENGL)
        {
            (*ppDevice) = new GLDeviceLinux(pDesc);
        }
        else 
        {
            (*ppDevice) = nullptr;

            LOG_ERROR("Api not supported on system");
        }
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline bool ExtensionSupported(const std::string& allExtensions, const std::string& extension)
    {
        std::size_t last = 0;
        for (std::size_t i = 0; (i = static_cast<int32>(allExtensions.find(' ', last))) != std::string::npos;)
        {
            if (allExtensions.substr(last, i - last) == extension)
            {
                return true;
            }

            last = i + 1;
        }

        return false;
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CreateOpenGLWindow(IWindow** ppWindow, const WindowDesc* pWindowDesc,
                            Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
                            Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc)
    {
        ::Display* pDisplay = RayEngine::GetDisplay();
        if (pDisplay == nullptr)
        {
            LOG_ERROR("'XOpenDisplay' failed");
            return;
        }

        int32 screen = DefaultScreen(pDisplay);
        std::string wglExtensions = reinterpret_cast<const char*>(glXQueryExtensionsString(pDisplay, screen));
        
        bool supportMsaa = ExtensionSupported(wglExtensions, "GLX_ARB_multisample");
        int32 samples = pSwapchainDesc->Samples;
        int32 sampleBuffers = None;
        if (pSwapchainDesc->Samples > 1)
        {
            if (supportMsaa)
            {
                sampleBuffers = GLX_SAMPLE_BUFFERS_ARB;
            }
            else
            {
                LOG_WARNING("GLX_ARB_multisample is not supported on the system");
            }
        }
        
        int32 renderType = GLX_RGBA_BIT;
        if (FormatIsFloat(pSwapchainDesc->BackBuffer.Format))
        {
            if (ExtensionSupported(wglExtensions, "GLX_ARB_fbconfig_float"))
            {
                renderType = GLX_RGBA_FLOAT_BIT_ARB;
            }
            else
            {
                LOG_WARNING("GLX_ARB_fbconfig_float not supported.");
            }
        }
       
        bool isSRGB = FormatIsSRGB(pSwapchainDesc->BackBuffer.Format);
        bool supportSRGB = ExtensionSupported(wglExtensions, "GLX_EXT_framebuffer_sRGB");
        if (isSRGB && !supportSRGB)
        {
            LOG_WARNING("GLX_EXT_framebuffer_sRGB not supported.");
        }

        int32 depthBits = FormatDepthBits(pSwapchainDesc->DepthStencil.Format);
        if (pSwapchainDesc->DepthStencil.Format != FORMAT_UNKNOWN && depthBits < 1)
        {
            LOG_ERROR(FormatToString(pSwapchainDesc->DepthStencil.Format) + "is not a compatable depthstencil format");
            
            ReleaseDisplay(pDisplay);
            return;
        }

        int32 doubleBuffer = True;
        if (pSwapchainDesc->BackBuffer.Count > 2)
        {
            LOG_WARNING("OpenGL implementation does not support more than 2 buffers");
        }
        else if (pSwapchainDesc->BackBuffer.Count < 2)
        {
            doubleBuffer = False;
        }
        

        int32 stencilBits = FormatStencilBits(pSwapchainDesc->DepthStencil.Format);
        int32 redBits = FormatRedComponentBits(pSwapchainDesc->BackBuffer.Format);
        int32 greenBits = FormatGreenComponentBits(pSwapchainDesc->BackBuffer.Format);
        int32 blueBits = FormatBlueComponentBits(pSwapchainDesc->BackBuffer.Format);
        int32 alphaBits = FormatAlphaComponentBits(pSwapchainDesc->BackBuffer.Format);

        int32 visualAttribs[] = {
            GLX_X_RENDERABLE    , True,
            GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
            GLX_RENDER_TYPE     , renderType,
            GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
            GLX_RED_SIZE        , redBits,
            GLX_GREEN_SIZE      , greenBits,
            GLX_BLUE_SIZE       , blueBits,
            GLX_ALPHA_SIZE      , alphaBits,
            GLX_DEPTH_SIZE      , depthBits,
            GLX_STENCIL_SIZE    , stencilBits,
            GLX_DOUBLEBUFFER    , doubleBuffer,
            sampleBuffers, 1,
            GLX_SAMPLES, samples,
            None
            };

        LOG_INFO("Framebuffer config - r: " + std::to_string(redBits) + ", b: " + std::to_string(blueBits) +
                 ", g: " + std::to_string(greenBits) + ", a: " + std::to_string(alphaBits) + 
                 ", depth: " + std::to_string(depthBits) + ", stencil: " + std::to_string(stencilBits));

        int32 major = 0;
        int32 minor = 0;
        if (glXQueryVersion(pDisplay, &major, &minor))
        {
            if (((major == 1) && (minor < 3)) || major < 1)
            {
                LOG_ERROR("GLX version is to low. Only 1.3 and higher is supported. Current is " + std::to_string(major) + '.' + std::to_string(minor));
                
                ReleaseDisplay(pDisplay);
                return;
            }
            else
            {
                LOG_INFO("GLX version: " + std::to_string(major) + '.' + std::to_string(minor));
            }
        }


        int32 configCount = 0;
        GLXFBConfig frameBufferConfig = {};
        GLXFBConfig* pFrameBufferConfig = glXChooseFBConfig(pDisplay, screen, visualAttribs, &configCount);
        if (pFrameBufferConfig == nullptr || configCount < 1)
        {
            LOG_ERROR("Failed to retrive Framebuffer Configurations. Count: " + std::to_string(configCount));

            ReleaseDisplay(pDisplay);
            return;
        }
        else
        {
            LOG_INFO("Found " + std::to_string(configCount) + " configurations");

            int32 bestIndex = 0;
            int32 highestPoints = 0;
            for (int32 i = 0; i < configCount; i++)
            {
                int32 points = 0;
                
                int32 value = 0;
                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_RED_SIZE, &value);
                if (value == redBits)
                    points++;
                
                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_GREEN_SIZE, &value);
                if (value == greenBits)
                    points++;

                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_BLUE_SIZE, &value);
                if (value == blueBits)
                    points++;

                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_ALPHA_SIZE, &value);
                if (value == alphaBits)
                    points++;

                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_DEPTH_SIZE, &value);
                if (value == depthBits)
                    points++;

                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_STENCIL_SIZE, &value);
                if (value == stencilBits)
                    points++;

                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_DOUBLEBUFFER, &value);
                if (value == doubleBuffer)
                    points++;

                glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_RENDER_TYPE, &value);
                if (value == renderType)
                    points++;

                if (supportMsaa)
                {
                    glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_SAMPLES, &value);
                    if (value == samples || (value == 0 && samples < 2))
                        points++;
                }

                if (supportSRGB)
                {
                    glXGetFBConfigAttrib(pDisplay, pFrameBufferConfig[i], GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT, &value);
                    if ((value == GL_TRUE) == isSRGB)
                        points++;
                }

                if (points > highestPoints)
                {
                    highestPoints = points;
                    bestIndex = i;
                }

                LOG_INFO("Points: " + std::to_string(points));
            }

            frameBufferConfig = pFrameBufferConfig[bestIndex];
        }

        XVisualInfo* pVisualInfo = glXGetVisualFromFBConfig(pDisplay, frameBufferConfig);
        XFree(pFrameBufferConfig);

        (*ppWindow) = new LinuxWindowImpl(pWindowDesc, pVisualInfo);

        ::Window xWindow = 0;
        (*ppWindow)->GetNativeWindowHandle(&xWindow);

        Graphics::GLDeviceLinux* pGLDevice = new Graphics::GLDeviceLinux(pDeviceDesc, xWindow, &frameBufferConfig);
        (*ppDevice) = pGLDevice;

        (*ppSwapchain) = new Graphics::GLSwapchainLinux(pSwapchainDesc, pGLDevice);

        ReleaseDisplay(pDisplay);
    }


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void CreateWindowForRendering(	IWindow** ppWindow, const WindowDesc* pWindowDesc,
                                    Graphics::IDevice** ppDevice, Graphics::DeviceDesc* pDeviceDesc,
                                    Graphics::ISwapchain** ppSwapchain, Graphics::SwapchainDesc* pSwapchainDesc,
                                    GRAPHICS_API api)
    {
        if (api == GRAPHICS_API_OPENGL)
        {
            CreateOpenGLWindow(ppWindow, pWindowDesc, ppDevice, pDeviceDesc, ppSwapchain, pSwapchainDesc);
        }
        else 
        {
            (*ppDevice) = nullptr;
            
            LOG_ERROR("Api not supported on system");
        }
    }
}

#endif