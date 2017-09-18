#pragma once
#include "gdi_render\gdi_control.h"
#include "tank.h"
#include "i_breakable.h"

#include <thread>
#include <vector>
#include <utility>

namespace simple_tanks {
	using namespace wnd_accelerator;

	class GameField;
    //, public IBreakable
	class Bullet : public GdiControl {
		friend class GameField;
		static const int kBulletSize;
		static const int kStepSize;
		enum class Direction {
			Up,
			Down,
			Left,
			Right
		};
	public:
		Bullet(GameField* gameField, Tank* tank);

		virtual ~Bullet();

	protected:

		bool MoveTo(int x, int y) {
			if (IsValidBulletPos(x, y)) {
				this->x = x;
				this->y = y;
				return true;
			}
			return false;
		}

		bool IsValidBulletPos(int x, int y);


		virtual void PaintPre(Graphics graphics) final override {
			graphics.DrawImage(bulletTexture, 0, 0, width, height);
		}

		virtual void PaintPost(Graphics graphics) final override {}

	protected:
		volatile Direction direction;

		GameField* gameField;

		Image* bulletTexture;
		std::unique_ptr<Image> bulletTextureUp;
		std::unique_ptr<Image> bulletTextureDown;
		std::unique_ptr<Image> bulletTextureLeft;
		std::unique_ptr<Image> bulletTextureRight;

		std::unique_ptr<std::thread> bulletThread;
		volatile bool bulletThreadTerminate;
	};

}