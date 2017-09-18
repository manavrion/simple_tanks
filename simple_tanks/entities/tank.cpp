#include "tank.h"

#include "..\gui\game_field.h"
#include "..\gui\gui_game_main_window.h"

#include <thread>
#include <chrono>

namespace simple_tanks {

    const int Tank::kTankSize = 32;
    const int Tank::kStepSize = 1;
    
    Tank::Tank(GameField* gameField) :
        tankTextureUp(new Image(L"resources/g_tank_l1_u.png")),
        tankTextureDown(new Image(L"resources/g_tank_l1_d.png")),
        tankTextureLeft(new Image(L"resources/g_tank_l1_l.png")),
        tankTextureRight(new Image(L"resources/g_tank_l1_r.png")),
        tankThreadTerminate(false),
        gameField(gameField) {

        SetWidth(kTankSize);
        SetHeight(kTankSize);

        tankTexture = tankTextureUp.get();


        tankThread.reset(new std::thread([&]() {
            while (!tankThreadTerminate) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                switch (move) {
                    case simple_tanks::Tank::Move::Null:
                        break;
                    case simple_tanks::Tank::Move::Up:
                        MoveTo(x, y - kStepSize);
                        tankTexture = tankTextureUp.get();
                        break;
                    case simple_tanks::Tank::Move::Down:
                        MoveTo(x, y + kStepSize);
                        tankTexture = tankTextureDown.get();
                        break;
                    case simple_tanks::Tank::Move::Left:
                        MoveTo(x - kStepSize, y);
                        tankTexture = tankTextureLeft.get();
                        break;
                    case simple_tanks::Tank::Move::Right:
                        MoveTo(x + kStepSize, y);
                        tankTexture = tankTextureRight.get();
                        break;
                }
            }
        }));

        GuiGameMainWindow::AddDynamicObject(this);
    }

    bool Tank::IsValidTankPos(int x, int y) {
        int width = x + kTankSize;
        int height = y + kTankSize;

        if (x < 0 || y < 0) {
            return false;
        }

        if (width > gameField->GetWidth() || height > gameField->GetHeight()) {
            return false;
        }


        x /= gameField->kBlockSize;
        y /= gameField->kBlockSize;

        width = (width - gameField->kBlockSize + 6) / gameField->kBlockSize;
        height = (height - gameField->kBlockSize + 6) / gameField->kBlockSize;

        if (width >= gameField->map.size() || height >= gameField->map.size()) {
            return false;
        }

        for (int i = x; i <= width; i++) {
            for (int j = y; j <= height; j++) {
                if (gameField->map[i][j].type != Block::Type::null) {
                    return false;
                }
            }
        }
        return true;
    }

}