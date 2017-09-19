#pragma once
#include "gdi_render\gdi_control.h"
#include "i_breakable.h"

#include <thread>
#include <vector>
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    //, public IBreakable
    class Base : public GdiControl, public IBreakable {
        static const int kBaseSize;
    public:
        Base();

        virtual ~Base();

        bool IsAlive() {
            return health > 0;
        }

        virtual void Damage() {
            health--;
        }

    protected:

        virtual void PaintPre(Graphics graphics) final override;

        virtual void PaintPost(Graphics graphics) final override;

    protected:
        std::unique_ptr<Image> baseTexture;

        int health;
    };

}