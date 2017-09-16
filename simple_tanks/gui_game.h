#pragma once
#include "game_field.h"

#include <gdi_render\gdi_control.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <string>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GameWindow;

    class GuiGame : public GdiControl {
    public:
        GuiGame() {
            SetBackground(Color::DimGray);

            GameField* gameField = new GameField();
            gameField->SetPosition(800 / 2 - gameField->GetWidth() / 2, 420 / 2 - gameField->GetHeight() / 2);

            Add(gameField);


        }
        virtual ~GuiGame() {}

    protected:

        virtual void PaintPre(Gdiplus::Graphics *graphics) final {
            graphics->FillRectangle(&SolidBrush(Color::DimGray), Rect(0, 0, width, height));
        }

        virtual void PaintPost(Gdiplus::Graphics *graphics) {}

    protected:
    };

}