#include <System/System.h>
#include <System/Window.h>
#include <System/Event.h>
#include <System/Keyboard.h>
#include <System/Mouse.h>
#include <System/Image.h>
#include <Debug/Debug.h>
#include <Utilities/StringUtilities.h>
#include <Utilities/TextureUtilities.h>
#include <Graphics/IDevice.h>
#include <Graphics/IDeviceContext.h>
#include <Graphics/ISwapchain.h>
#include <Math/ColorF.h>

int main()
{
    using namespace RayEngine;
    using namespace Graphics;

    SystemDesc systemDesc = {};
    QuerySystemDesc(&systemDesc);

	LOG_INFO("Screen: w: " + std::to_string(systemDesc.ScreenWidth) + ", h: " + std::to_string(systemDesc.ScreenHeight));

    const uint8* pPixels = nullptr;
    int32 width = 0;
    int32 height = 0;
    if(!LoadImageFromFile("Walter.png", "", reinterpret_cast<const void**>(&pPixels), width, height, FORMAT_R8G8B8A8_UNORM))
    {
        LOG_ERROR("Failed to load file");
    }
    else
    {
        LOG_INFO("Loaded image - w: " + std::to_string(width) + ", h: " + std::to_string(height));
    }

    Image icon(pPixels, width, height, FORMAT_R8G8B8A8_UNORM);
    LOG_INFO("Icon image Created");

    delete pPixels;
    pPixels = nullptr;

    if(!LoadImageFromFile("Cursor.png", "", reinterpret_cast<const void**>(&pPixels), width, height, FORMAT_R8G8B8A8_UNORM))
    {
        LOG_ERROR("Failed to load file");
    }
    else
    {
        LOG_INFO("Loaded image - w: " + std::to_string(width) + ", h: " + std::to_string(height));
    }

    Image cursor(pPixels, width, height, FORMAT_R8G8B8A8_UNORM);
    LOG_INFO("Cursor image Created");

    delete pPixels;
    pPixels = nullptr;

    WindowDesc windowDesc = {};
    windowDesc.Title = "RayEngine";
    windowDesc.Width = 800;
    windowDesc.Height = 600;
    windowDesc.x = static_cast<int32>((systemDesc.ScreenWidth - windowDesc.Width) / 2.0f);
    windowDesc.y = static_cast<int32>((systemDesc.ScreenHeight - windowDesc.Height) / 2.0f);
    windowDesc.BackgroundColor.r = 127;
    windowDesc.BackgroundColor.g = 127;
    windowDesc.BackgroundColor.b = 127;
    windowDesc.pIcon = &icon;
    windowDesc.Cursor.pImage = &cursor;
    windowDesc.Cursor.Hotspot = Math::Point();
    windowDesc.Style = WINDOWSTYLE_MAXIMIZABLE | WINDOWSTYLE_TITLEBAR;
    
    DeviceDesc deviceDesc = {};
    deviceDesc.Name = "Main Device";
    deviceDesc.DeviceFlags = DEVICE_FLAG_DEBUG;
    deviceDesc.SamplerDescriptorCount = 8;
    deviceDesc.ResourceDescriptorCount = 8;
    deviceDesc.RendertargetDescriptorCount = 2;
    deviceDesc.DepthStencilDescriptorCount = 2;

    SwapchainDesc swapchainDesc = {};
    swapchainDesc.Name = "Swapchain";
    swapchainDesc.Width = windowDesc.Width;
    swapchainDesc.Height = windowDesc.Height;
    swapchainDesc.Samples = 1;
    swapchainDesc.BackBuffer.Count = 2;
    swapchainDesc.BackBuffer.Format = FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.DepthStencil.Format = FORMAT_D24_UNORM_S8_UINT;

    IWindow* pWindow = nullptr;
    IDevice* pDevice = nullptr;
    ISwapchain* pSwapchain = nullptr;
    CreateWindowForRendering(&pWindow, &windowDesc, &pDevice, &deviceDesc, &pSwapchain, &swapchainDesc, GRAPHICS_API_D3D11);
    
    pWindow->SetTitle("Hello World");
    pWindow->Show();

    LOG_INFO("Window Created");
    LOG_INFO("Device Created");
    LOG_INFO("Swapchain Created");

	AdapterDesc adpaterDesc = {};
	pDevice->GetAdapterDesc(&adpaterDesc);

	LOG_INFO("Device Vendor: " + adpaterDesc.VendorName);
	LOG_INFO("Device Model: " + adpaterDesc.ModelName);

    IDeviceContext* pContext = nullptr;
    pDevice->GetImmediateContext(&pContext);
    pContext->SetSwapChain(pSwapchain);

    Event event = {};
    while (event.Type != EVENT_TYPE_QUIT)
    {
		if (pWindow->PeekEvent(&event))
		{
			if (event.Type == EVENT_TYPE_CLOSE)
			{
				LOG_INFO("Window closed");
				SendQuitMessage(0);
			}
		}

        float color[] = { 0.392f, 0.584f, 0.929f, 1.0f };
        pContext->ClearRendertargetView(nullptr, color);
        pContext->ClearDepthStencilView(nullptr, 1.0f, 0);
        pSwapchain->Present();
    }

    pWindow->Destroy();
    ReRelease_S(pSwapchain);
    ReRelease_S(pContext);
    ReRelease_S(pDevice);

    return event.Quit.ExitCode;
}