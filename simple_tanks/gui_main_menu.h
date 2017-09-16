#pragma once
#include <gdi_render\gdi_control.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <string>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GameWindow;

    class GuiMainMenu : public GdiControl {
    public:
        GuiMainMenu() {
            SetBackground(Color::Black);

            image.reset(new Image(L"resources/battle_city.png"));
        }
        virtual ~GuiMainMenu() {}

    protected:

        virtual void PaintPre(Gdiplus::Graphics *graphics) final {
            graphics->DrawImage(image.get(), (width / 2) - (image->GetWidth() / 2), 64, image->GetWidth(), image->GetHeight());
        }

        virtual void PaintPost(Gdiplus::Graphics *graphics) {}

    protected:
        std::unique_ptr<Image> image;
    };

}