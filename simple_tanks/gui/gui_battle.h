#pragma once
#include "gdi_render\gdi_control.h"

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GuiBattle : public GdiControl {
    public:
        GuiBattle();
        virtual ~GuiBattle();

    protected:
        virtual void PaintPre(Graphics graphics) final override;
        virtual void PaintPost(Graphics graphics) final override;
    };

}