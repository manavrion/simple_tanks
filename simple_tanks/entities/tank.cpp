#include "tank.h"

#include "..\gui\game_field.h"
#include "..\gui\gui_game_main_window.h"

#include <thread>
#include <chrono>

namespace simple_tanks {

    const int Tank::kTankSize = 32;
    const int Tank::kStepSize = 1;
    
    Tank::Tank(GameField* gameField, TankLayout* tankLayout) :
        tankLayout(tankLayout),
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


        tankThread.reset(new std::thread([&]() {
            while (!tankThreadTerminate) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                
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
                        this->tankLayout->MoveUp();
                        direction = Direction::Up;
                    }
                    if (moveDown && MoveTo(x, y + kStepSize)) {
                        this->tankLayout->MoveDown();
                        direction = Direction::Down;
                    }
                    if (moveLeft && MoveTo(x - kStepSize, y)) {
                        this->tankLayout->MoveLeft();
                        direction = Direction::Left;
                    }
                    if (moveRight && MoveTo(x + kStepSize, y)) {
                        this->tankLayout->MoveRight();
                        direction = Direction::Right;
                    }
                } else {
                    if (moveUp) {
                        MoveTo(x, y - kStepSize);
                        this->tankLayout->MoveUp();
                        direction = Direction::Up;
                    }
                    if (moveDown) {
                        MoveTo(x, y + kStepSize);
                        this->tankLayout->MoveDown();
                        direction = Direction::Down;
                    }
                    if (moveLeft) {
                        MoveTo(x - kStepSize, y);
                        this->tankLayout->MoveLeft();
                        direction = Direction::Left;
                    }
                    if (moveRight) {
                        MoveTo(x + kStepSize, y);
                        this->tankLayout->MoveRight();
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