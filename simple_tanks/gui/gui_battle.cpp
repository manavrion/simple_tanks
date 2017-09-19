#include "gui_battle.h"

#include "game_field.h"

namespace simple_tanks {

    GuiBattle::GuiBattle() {
        SetBackground(Color::DimGray);

        GameField* gameField = new GameField();
        gameField->SetPosition(800 / 2 - gameField->GetWidth() / 2, 0);

        Add(gameField);
    }

    GuiBattle::~GuiBattle() {}

    void GuiBattle::PaintPre(Graphics graphics) {
        graphics.FillRectangle(&SolidBrush(Color::DimGray), Rect(0, 0, width, height));
    }

    void GuiBattle::PaintPost(Graphics graphics) {}

}