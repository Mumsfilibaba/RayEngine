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
#include "../../Include/OpenGL/GLDeviceLinux.h"

#if defined(RE_PLATFORM_LINUX)
#include "../Linux/Linux.h"

namespace RayEngine
{
    namespace Graphics
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        GLDeviceLinux::GLDeviceLinux(const DeviceDesc* pDesc)
            : GLDevice(pDesc),
            m_pDisplay(nullptr),
            m_XWindow(0),
            m_IsWindowOwner(false)
        {
            Create();
        }


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        GLDeviceLinux::GLDeviceLinux(const DeviceDesc* pDesc, ::Window xWindow, GLXFBConfig* pFBConfig)
            : GLDevice(pDesc),
            m_pDisplay(nullptr),
            m_XWindow(0),
            m_IsWindowOwner(false)
        {
            Create(xWindow, pFBConfig);
        }
        

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        GLDeviceLinux::~GLDeviceLinux()
        {
            if (m_IsWindowOwner)
            {
                XDestroyWindow(m_pDisplay, m_XWindow);
                m_XWindow = 0;
            }

            ReleaseDisplay(m_pDisplay);
            m_pDisplay = nullptr;

            LOG_INFO("Destroyed Linux Context");
        }
       

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void GLDeviceLinux::Create()
        {
            m_pDisplay = RayEngine::GetDisplay();
            if (m_pDisplay == nullptr)
            {
                LOG_ERROR("'XOpenDisplay' failed");
                return;
            }

            int32 visualAttribs[] = {
                GLX_X_RENDERABLE    , True,
                GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
                GLX_RENDER_TYPE     , GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
                GLX_RED_SIZE        , 8,
                GLX_GREEN_SIZE      , 8,
                GLX_BLUE_SIZE       , 8,
                GLX_ALPHA_SIZE      , 8,
                GLX_DEPTH_SIZE      , 24,
                GLX_STENCIL_SIZE    , 8,
                GLX_DOUBLEBUFFER    , True,
                None
                };

           
            int32 major = 0;
            int32 minor = 0;
            if (glXQueryVersion(m_pDisplay, &major, &minor))
            {
                if (((major == 1) && (minor < 3)) || major < 1)
                {
                    LOG_ERROR("GLX version is to low. Only 1.3 and higher is supported. Current is " + std::to_string(major) + '.' + std::to_string(minor));
                    return;
                }
                else
                {
                    LOG_INFO("GLX version: " + std::to_string(major) + '.' + std::to_string(minor));
                }
            }


            int32 screen = DefaultScreen(m_pDisplay);
            int32 configCount = 0;
            GLXFBConfig* pFrameBufferConfig = glXChooseFBConfig(m_pDisplay, screen, visualAttribs, &configCount);
            if (pFrameBufferConfig == nullptr)
            {
                LOG_ERROR("Failed to retrive Framebuffer Configurations");
                return;
            }

            GLXFBConfig frameBufferConfig = pFrameBufferConfig[0];
            XVisualInfo* pVisualInfo = glXGetVisualFromFBConfig(m_pDisplay, frameBufferConfig);
            XFree(pFrameBufferConfig);

            ::Window rootWin = RootWindow(m_pDisplay, screen);
            XSetWindowAttributes winAttribs = {};
            Colormap cmap = XCreateColormap(m_pDisplay, rootWin, pVisualInfo->visual, AllocNone);

            winAttribs.colormap = cmap;
            winAttribs.event_mask = StructureNotifyMask;

            ::Window xWindow = XCreateWindow(m_pDisplay, rootWin, 0, 0, 128, 128, 0, pVisualInfo->depth, InputOutput, pVisualInfo->visual, CWColormap | CWEventMask, &winAttribs);
            XFree(pVisualInfo);

            if (xWindow == 0)
            {
                LOG_ERROR("Failed to create a dummy window for a windowless context");
                return;
            }

            m_IsWindowOwner = true;
            Create(xWindow, &frameBufferConfig);
        }


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void GLDeviceLinux::Create(::Window xWindow, GLXFBConfig* pFBConfig)
        {
            if (m_pDisplay == nullptr)
            {
                m_pDisplay = RayEngine::GetDisplay();
            }

            int32 screen = DefaultScreen(m_pDisplay);            

            std::string glxExtenions = reinterpret_cast<const char*>(glXQueryExtensionsString(m_pDisplay, screen));
            QueryExtensionsFromString(m_Extensions, glxExtenions);

            LoadOpenGL();
            if ((!ExtensionSupported("GLX_ARB_create_context")) || glXCreateContextAttribsARB == nullptr)
            {
                LOG_ERROR("Does not support creation of context");
                return;
            }

            int32 contextAttribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                GLX_CONTEXT_MINOR_VERSION_ARB, 3,
                None
            };

            m_XWindow = xWindow;
            m_NativeContext = glXCreateContextAttribsARB(m_pDisplay, *pFBConfig, 0, True, contextAttribs);
            if (m_NativeContext == RE_GL_NULL_NATIVE_CONTEXT)
            {
                LOG_ERROR("Failed to create context");
                return;
            }
            else
            {
                LOG_INFO("Created context");

                XMapWindow(m_pDisplay, m_XWindow);

                XSync(m_pDisplay, False);
                glXMakeCurrent(m_pDisplay, m_XWindow, m_NativeContext);
            }

            int32 extensionCount = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
            for (int32 i = 0; i < extensionCount; i++)
            {
                std::string extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
                m_Extensions.push_back(extension);
            }
        }
    }
}

#endif