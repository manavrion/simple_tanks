#pragma once
#include "afx.h"
#include "bullet.h"

namespace simple_tanks {
	using namespace wnd_accelerator;

	struct IBreakable {

		virtual Rect GetRect() = 0;
		virtual int GetHealth() = 0;
		virtual void SetHealth(int health) = 0;

		virtual bool IsAlive() = 0;

		virtual void TestBullet(Bullet* bullet) = 0;
	};

}