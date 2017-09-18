#include "gui_menu.h"
#include "gui_game_main_window.h"

namespace simple_tanks {

    GuiMenu::GuiMenu() {
        SetBackground(Color::Black);

        image.reset(new Image(L"resources/battle_city.png"));
    }

    GuiMenu::~GuiMenu() {}

    void GuiMenu::PaintPre(Graphics graphics) {
        graphics.DrawImage(image.get(), (width / 2) - (image->GetWidth() / 2), 64, image->GetWidth(), image->GetHeight());
    }

    void GuiMenu::PaintPost(Graphics graphics) {}

}