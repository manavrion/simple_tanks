#include "tank.h"
#include "game_field.h"
#include <thread>
#include <chrono>

namespace simple_tanks {



    Tank::Tank(GameField* gameField) 
        : tankSize(32), stepSize(1), gameField(gameField) {

        SetWidth(32);
        SetHeight(32);

        tankTextureUp.reset(new Image(L"resources/g_tank_l1_u.png"));
        tankTextureDown.reset(new Image(L"resources/g_tank_l1_d.png"));
        tankTextureLeft.reset(new Image(L"resources/g_tank_l1_l.png"));
        tankTextureRight.reset(new Image(L"resources/g_tank_l1_r.png"));

        tankTexture = tankTextureUp.get();


        tankThread.reset(new std::thread([&]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                switch (move) {
                    case simple_tanks::Tank::Move::Null:
                        break;
                    case simple_tanks::Tank::Move::Up:
                        MoveTo(x, y - stepSize);
                        tankTexture = tankTextureUp.get();
                        break;
                    case simple_tanks::Tank::Move::Down:
                        MoveTo(x, y + stepSize);
                        tankTexture = tankTextureDown.get();
                        break;
                    case simple_tanks::Tank::Move::Left:
                        MoveTo(x - stepSize, y);
                        tankTexture = tankTextureLeft.get();
                        break;
                    case simple_tanks::Tank::Move::Right:
                        MoveTo(x + stepSize, y);
                        tankTexture = tankTextureRight.get();
                        break;
                }
                Repaint();
            }
        }));
    }

    bool Tank::IsValidTankPos(int x, int y) {
        int width = x + tankSize;
        int height = y + tankSize;

        if (x < 0 || y < 0) {
            return false;
        }

        if (width > gameField->GetWidth() || height > gameField->GetHeight()) {
            return false;
        }


        x /= gameField->blockSize;
        y /= gameField->blockSize;

        width = (width - gameField->blockSize + 6) / gameField->blockSize;
        height = (height - gameField->blockSize + 6) / gameField->blockSize;

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