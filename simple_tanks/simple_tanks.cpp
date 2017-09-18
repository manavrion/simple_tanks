#include "simple_tanks.h"

#include "gui\gui_game_main_window.h"
#include "resources\targetver.h"

#include <windows.h>
#include <utility>

using namespace simple_tanks;

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow) {

    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    std::unique_ptr<Window> window(new GuiGameMainWindow());
    window->Run();

    return 0;
}

