#pragma once
#include "game_field.h"

#include <gdi_render\gdi_control.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <string>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GuiGameMainWindow;

    class GuiBattle : public GdiControl {
    public:
        GuiBattle();
        virtual ~GuiBattle();

    protected:

        virtual void PaintPre(Graphics graphics) final override;

        virtual void PaintPost(Graphics graphics) override;

    protected:
    };

}