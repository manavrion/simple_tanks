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
        gameField(gameField),
        direction(Direction::Up),
        moveUp(false),
        moveDown(false),
        moveLeft(false),
        moveRight(false),
        health(1) {

        SetWidth(kTankSize);
        SetHeight(kTankSize);

        tankTexture = tankTextureUp.get();


        tankThread.reset(new std::thread([&]() {
            while (!tankThreadTerminate) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                
                if (moveUp && moveDown) {
                    moveUp = false;
                    moveDown = false;
                }
                if (moveLeft && moveRight) {
                    moveLeft = false;
                    moveRight = false;
                }

                int count = 0;
                if (moveUp) count++;
                if (moveDown) count++;
                if (moveLeft) count++;
                if (moveRight) count++;               

                if (count > 1) {
                    if (moveUp && MoveTo(x, y - kStepSize)) {
                        tankTexture = tankTextureUp.get();
                        direction = Direction::Up;
                    }
                    if (moveDown && MoveTo(x, y + kStepSize)) {
                        tankTexture = tankTextureDown.get();
                        direction = Direction::Down;
                    }
                    if (moveLeft && MoveTo(x - kStepSize, y)) {
                        tankTexture = tankTextureLeft.get();
                        direction = Direction::Left;
                    }
                    if (moveRight && MoveTo(x + kStepSize, y)) {
                        tankTexture = tankTextureRight.get();
                        direction = Direction::Right;
                    }
                } else {
                    if (moveUp) {
                        MoveTo(x, y - kStepSize);
                        tankTexture = tankTextureUp.get();
                        direction = Direction::Up;
                    }
                    if (moveDown) {
                        MoveTo(x, y + kStepSize);
                        tankTexture = tankTextureDown.get();
                        direction = Direction::Down;
                    }
                    if (moveLeft) {
                        MoveTo(x - kStepSize, y);
                        tankTexture = tankTextureLeft.get();
                        direction = Direction::Left;
                    }
                    if (moveRight) {
                        MoveTo(x + kStepSize, y);
                        tankTexture = tankTextureRight.get();
                        direction = Direction::Right;
                    }
                }
                
            }
        }));

        GuiGameMainWindow::AddDynamicObject(this);
    }

    void Tank::MoveUp(bool moveUp) {
        this->moveUp = moveUp;
    }

    void Tank::MoveDown(bool moveDown) {
        this->moveDown = moveDown;
    }

    void Tank::MoveLeft(bool moveLeft) {
        this->moveLeft = moveLeft;
    }

    void Tank::MoveRight(bool moveRight) {
        this->moveRight = moveRight;
    }

    void Tank::MoveReset() {
        moveUp = false;
        moveDown = false;
        moveLeft = false;
        moveRight = false;
    }

    void Tank::Shoot() {
        gameField->SpawnBullet(this);
    }

    Tank::~Tank() {
        GuiGameMainWindow::EraseDynamicObject(this);
        tankThreadTerminate = true;
        tankThread->join();
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

        width = (width - gameField->kBlockSize + 7) / gameField->kBlockSize;
        height = (height - gameField->kBlockSize + 7) / gameField->kBlockSize;

        if (width >= gameField->map.size() || height >= gameField->map.size()) {
            return false;
        }

        for (int i = x; i <= width; i++) {
            for (int j = y; j <= height; j++) {
                if (gameField->map[i][j].GetType() != Block::Type::null) {
                    return false;
                }
            }
        }
        return true;
    }

}