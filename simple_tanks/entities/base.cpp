#include "base.h"
#include "../gui/gui_game_main_window.h"

namespace simple_tanks {

    const int Base::kBaseSize = 32;

    Base::Base() : health(5), baseTexture(new Image(L"resources/base.png")) {
        width = kBaseSize;
        height = kBaseSize;
        GuiGameMainWindow::AddDynamicObject(this);
    }

    Base::~Base() {
        GuiGameMainWindow::EraseDynamicObject(this);
    }

    void Base::PaintPre(Graphics graphics) {
        graphics.DrawImage(baseTexture.get(), 0, 0, kBaseSize, kBaseSize);
    }

    void Base::PaintPost(Graphics graphics) {}

}