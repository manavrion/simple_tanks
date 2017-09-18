#pragma once
#include "..\entities\tank.h"
#include "..\entities\block.h"
#include "..\entities\bullet.h"
#include "..\entities\i_breakable.h"
#include "gdi_render\gdi_control.h"

#include <vector>
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GameField : public GdiControl {
        friend class Tank;
		friend class Bullet;
        static const int kMapDim;
        static const int kBlockSize;

    public:
        GameField();
        virtual ~GameField();

        
        void SpawnBullet(Tank* tank);



    protected:
        virtual void PaintPre(Graphics graphics) final override;
        virtual void PaintPost(Graphics graphics) final override;

    protected:
        std::vector<std::vector<Block>> map;
        std::unique_ptr<Image> brickTexture;

        std::unique_ptr<Bitmap> mapTexture;

        // Caching
        SolidBrush blackBrush;
        Gdiplus::TextureBrush brickBrush;
    };

}