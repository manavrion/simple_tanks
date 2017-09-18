#pragma once
#include <gdi_render\gdi_control.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <string>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GuiMenu : public GdiControl {
    public:
        GuiMenu();
        virtual ~GuiMenu();

    protected:

        virtual void PaintPre(Graphics graphics) override;

        virtual void PaintPost(Graphics graphics) override;

    protected:
        std::unique_ptr<Image> image;
    };

}