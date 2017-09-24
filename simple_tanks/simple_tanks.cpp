#include "simple_tanks.h"

#include "gui\gui_game_main_window.h"
#include "resources\targetver.h"

#include <windows.h>
#include <utility>

using namespace simple_tanks;

void(*_stdcall pUpdateNodes1)(int, int);
void(*_stdcall pUpdateNodes2)(int, int);
void(*_stdcall pUpdateNodes3)(int, int);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow) {

    HMODULE hLib1 = LoadLibrary("nw1.dll");
    HMODULE hLib2 = LoadLibrary("nw2.dll");
    HMODULE hLib3 = LoadLibrary("nw3.dll");

    (FARPROC & _stdcall)pUpdateNodes1 = GetProcAddress(hLib1, "updateNodes");
    (FARPROC & _stdcall)pUpdateNodes2 = GetProcAddress(hLib2, "updateNodes");
    (FARPROC & _stdcall)pUpdateNodes3 = GetProcAddress(hLib3, "updateNodes");

    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    while (GuiGameMainWindow::IsRestart()) {
        std::unique_ptr<Window> window(new GuiGameMainWindow());
        window->Run();
        break;
    }


    FreeLibrary(hLib1);
    FreeLibrary(hLib2);
    FreeLibrary(hLib3);

    return 0;
}

