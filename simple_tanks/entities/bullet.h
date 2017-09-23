#pragma once
#include "gdi_render\gdi_control.h"
#include "i_breakable.h"
#include "direction.h"

#include <thread>
#include <vector>
#include <utility>

namespace simple_tanks {
	using namespace wnd_accelerator;

	class GameField;
    class Tank;
    //, public IBreakable
	class Bullet : public GdiControl, public IBreakable {
		friend class GameField;
		static const int kBulletSize;
		static const int kStepSize;
	public:
		Bullet(GameField* gameField, Tank* tank);

		virtual ~Bullet();

        bool IsAlive() {
            return alive;
        }

        virtual void Damage() {
            alive = false;
        }

        virtual bool IsOwner(Tank* tank) {
            return tank == owner;
        }

	protected:

        bool MoveTo(int x, int y);

		bool IsValidBulletPos(int x, int y);


		virtual void PaintPre(Graphics graphics) final override {
			graphics.DrawImage(bulletTexture, 0, 0, width, height);
		}

		virtual void PaintPost(Graphics graphics) final override {}

	protected:
		Direction direction;

		GameField* gameField;

		Image* bulletTexture;
		std::unique_ptr<Image> bulletTextureUp;
		std::unique_ptr<Image> bulletTextureDown;
		std::unique_ptr<Image> bulletTextureLeft;
		std::unique_ptr<Image> bulletTextureRight;

		std::unique_ptr<std::thread> bulletThread;
		volatile bool bulletThreadTerminate;

        bool alive;
        Tank* owner;
	};

}