#include <iostream>

#include <System/System.h>
#include <System/Window.h>
#include <System/Event.h>
#include <System/Keyboard.h>

#include <Utilities/StringUtilities.h>

int main()
{
    using namespace RayEngine;

    if (!InitializeSystem(0))
    {
        std::cout << "Error\n";
        return 0;
    }
    
    std::cout << "Hello World\n";

    SystemDesc systemDesc = {};
    QuerySystemDesc(&systemDesc);

    WindowDesc windowDesc = {};
    windowDesc.Width = 800;
    windowDesc.Height = 600;
    windowDesc.x = (systemDesc.ScreenWidth / 2.0f) - (windowDesc.Width / 2.0f);
    windowDesc.y = (systemDesc.ScreenHeight / 2.0f) - (windowDesc.Height / 2.0f);
    windowDesc.BackgroundColor.r = 100;
    windowDesc.BackgroundColor.g = 149;
    windowDesc.BackgroundColor.b = 237;
    IWindow* pWindow = CreateWindow(&windowDesc);
    pWindow->Show();

    Event event;
    while (event.Type != EVENT_TYPE_QUIT)
    {
        pWindow->GetEvent(&event);
        if (event.Type == EVENT_TYPE_CLOSE)
        {
            std::cout << "Recived window closed event\n";
            SendQuitMessage(0);
            break;
        }
        else if (event.Type == EVENT_TYPE_RESIZE)
        {
            std::cout << "Recived window resize. W: " << event.Resize.Width << ", H: " << event.Resize.Height << "\n";
        }
        else if (event.Type == EVENT_TYPE_KEYPRESSED)
        {
            std::cout << "Pressed key: " << KeyCodeToString(event.Key.KeyCode) << "\n";
        }
        else if (event.Type == EVENT_TYPE_KEYRELEASED)
        {
            std::cout << "Released key: " << KeyCodeToString(event.Key.KeyCode) << "\n";
        }
        else
        {
            std::cout << "Recived window event\n";
        }

        if (Keyboard::IsPressed(KEY_A))
        {
            std::cout << "A is pressed\n";
        }
    }

    pWindow->Destroy();

    ReleaseSystem();
    return event.Quit.ExitCode;
}