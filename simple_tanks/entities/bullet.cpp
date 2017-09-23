#include "bullet.h"

#include "tank.h"
#include "..\gui\game_field.h"
#include "..\gui\gui_game_main_window.h"

#include <thread>
#include <chrono>

namespace simple_tanks {

	const int Bullet::kBulletSize = 10;
	const int Bullet::kStepSize = 1;

	Bullet::Bullet(GameField* gameField, Tank* tank) :
		bulletTextureUp(new Image(L"resources/g_bullet_u.png")),
		bulletTextureDown(new Image(L"resources/g_bullet_d.png")),
		bulletTextureLeft(new Image(L"resources/g_bullet_l.png")),
		bulletTextureRight(new Image(L"resources/g_bullet_r.png")),
		bulletThreadTerminate(false),
		gameField(gameField),
		direction(tank->GetDirection()),
        alive(true),
        owner(tank) {

        SetX(tank->GetX());
        SetY(tank->GetY());
        SetWidth(kBulletSize);
        SetHeight(kBulletSize);

        switch (tank->GetDirection()) {
            case Direction::up:
                y -= kBulletSize;
                x += 11;
                direction = Direction::up;
                bulletTexture = bulletTextureUp.get();
                break;
            case Direction::down:
                y += tank->GetHeight();
                x += 11;
                direction = Direction::down;
                bulletTexture = bulletTextureDown.get();
                break;
            case Direction::left:
                x -= kBulletSize;
                y += 11;
                direction = Direction::left;
                bulletTexture = bulletTextureLeft.get();
                break;
            case Direction::right:
                x += tank->GetWidth();
                y += 11;
                direction = Direction::right;
                bulletTexture = bulletTextureRight.get();
                break;
        }


		bulletThread.reset(new std::thread([&]() {
			while (!bulletThreadTerminate) {
				std::this_thread::sleep_for(std::chrono::milliseconds(4));
                bool live = false;
                switch (direction) {
                    case Direction::up:
                        live = MoveTo(x, y - kStepSize);
                        break;
                    case Direction::down:
                        live = MoveTo(x, y + kStepSize);
                        break;
                    case Direction::left:
                        live = MoveTo(x - kStepSize, y);
                        break;
                    case Direction::right:
                        live = MoveTo(x + kStepSize, y);
                        break;
                    default:
                        break;
                }
                if (!live) {
                    alive = false;
                }
			}
		}));

		GuiGameMainWindow::AddDynamicObject(this);
	}


	Bullet::~Bullet() {
		GuiGameMainWindow::EraseDynamicObject(this);
		bulletThreadTerminate = true;
		bulletThread->join();
	}

    bool Bullet::MoveTo(int x, int y) {
        if (IsValidBulletPos(x, y)) {
            
            this->x = x;
            this->y = y;
            return true;
        }
        return false;
    }

    bool Bullet::IsValidBulletPos(int x, int y) {
		int width = x + kBulletSize;
		int height = y + kBulletSize;

		if (x < 0 || y < 0) {
			return false;
		}

		if (width > gameField->GetWidth() || height > gameField->GetHeight()) {
			return false;
		}

		return true;
	}

}