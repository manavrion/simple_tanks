#pragma once
#include "afx.h"
#include "bullet.h"

namespace simple_tanks {
	using namespace wnd_accelerator;

	struct IBreakable {

		virtual Rect GetRect(){}
		virtual int GetHealth(){}
		virtual void SetHealth(int health){}

		virtual bool IsAlive(){}

		//virtual void TestBullet(Bullet* bullet){}
	};

}