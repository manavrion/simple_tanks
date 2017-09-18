#include "bullet.h"

#include "..\gui\game_field.h"
#include "..\gui\gui_game_main_window.h"

#include <thread>
#include <chrono>

namespace simple_tanks {

	const int Bullet::kBulletSize = 32;
	const int Bullet::kStepSize = 1;

	Bullet::Bullet(Direction direction, GameField* gameField) :
		bulletTextureUp(new Image(L"resources/g_bullet_u.png")),
		bulletTextureDown(new Image(L"resources/g_bullet_d.png")),
		bulletTextureLeft(new Image(L"resources/g_bullet_l.png")),
		bulletTextureRight(new Image(L"resources/g_bullet_r.png")),
		bulletThreadTerminate(false),
		gameField(gameField),
		direction(direction) {

		SetWidth(kBulletSize);
		SetHeight(kBulletSize);

		bulletTexture = bulletTextureUp.get();


		bulletThread.reset(new std::thread([&]() {
			while (!bulletThreadTerminate) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				/*if (moveUp && MoveTo(x, y - kStepSize)) {
					bulletTexture = bulletTextureUp.get();
				}
				if (moveDown && MoveTo(x, y + kStepSize)) {
					bulletTexture = bulletTextureDown.get();
				}
				if (moveLeft && MoveTo(x - kStepSize, y)) {
					bulletTexture = bulletTextureLeft.get();
				}
				if (moveRight && MoveTo(x + kStepSize, y)) {
					bulletTexture = bulletTextureRight.get();
				}*/
			}
		}));

		GuiGameMainWindow::AddDynamicObject(this);
	}

	void Bullet::MoveUp(bool moveUp) {
		this->moveUp = moveUp;
	}

	void Bullet::MoveDown(bool moveDown) {
		this->moveDown = moveDown;
	}

	void Bullet::MoveLeft(bool moveLeft) {
		this->moveLeft = moveLeft;
	}

	void Bullet::MoveRight(bool moveRight) {
		this->moveRight = moveRight;
	}

	void Bullet::MoveReset() {
		moveUp = false;
		moveDown = false;
		moveLeft = false;
		moveRight = false;
	}

	Bullet::~Bullet() {
		GuiGameMainWindow::EraseDynamicObject(this);
		bulletThreadTerminate = true;
		bulletThread->join();
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


		x /= gameField->kBlockSize;
		y /= gameField->kBlockSize;

		width = (width - gameField->kBlockSize + 7) / gameField->kBlockSize;
		height = (height - gameField->kBlockSize + 7) / gameField->kBlockSize;

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