#pragma once
#include "gdi_render\gdi_control.h"
#include "gdi_render\g_button.h"

#include <string>
#include <unordered_map>
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GuiMenu : public GdiControl {
    public:
        GuiMenu();
        virtual ~GuiMenu();

    protected:
        virtual void PaintPre(Graphics graphics) final override;
        virtual void PaintPost(Graphics graphics) final override;

    protected:
        std::unique_ptr<Image> image;
    };

}