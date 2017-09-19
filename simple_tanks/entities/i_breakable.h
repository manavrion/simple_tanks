#pragma once
#include "afx.h"

namespace simple_tanks {
	using namespace wnd_accelerator;

    struct IBreakable {
        virtual bool IsAlive() = 0;
        virtual void Damage() = 0;
    };

}