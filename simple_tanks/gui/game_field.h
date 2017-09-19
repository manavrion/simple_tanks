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
        friend class Block;
        static const int kMapDim;
        static const int kBlockSize;

    public:
        GameField();
        virtual ~GameField();

        
        void SpawnBullet(Tank* tank);

        void TestBullet(Bullet* bullet) {
            bool collision = false;

            Rect a(bullet->GetRect());

            for (auto& list : map) {
                for (auto& block : list) {
                    if (block.GetType() == Block::Type::brick) {
                        Rect b(block.GetX(), block.GetY(), Block::kBlockSize, Block::kBlockSize);

                        if (a.IntersectsWith(b)) {
                            block.SetType(Block::Type::null);
                            collision = true;
                        }
                    }
                }
            }

            for (auto tank : tanks) {
                Rect b(tank->GetRect());
                if (a.IntersectsWith(b) && !bullet->IsOwner(tank)) {
                    tank->Damage();
                    collision = true;
                }
            }
            if (collision) {
                bullet->Damage();
            }

        }

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


        // Map
        std::list<Tank*> tanks;
        std::list<Bullet*> bullets;
    };

}