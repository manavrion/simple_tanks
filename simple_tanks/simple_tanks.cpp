// simpletanks.cpp : Defines the entry point for the application.
//

#include "resources\targetver.h"
#include "simple_tanks.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <utility>

#include "gui_game_main_window.h"

using namespace simple_tanks;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow) {

    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    std::unique_ptr<GuiGameMainWindow> window(new GuiGameMainWindow());
    window->Run();

    return 0;
}

