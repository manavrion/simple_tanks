#pragma once
#include "..\entities\tank.h"
#include "..\entities\base.h"
#include "..\entities\block.h"
#include "..\entities\bullet.h"
#include "..\entities\i_breakable.h"
#include "..\entities\direction.h"
#include "gdi_render\gdi_control.h"

#include <vector>
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    struct Node;

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

            Rect aForMap(a.X - 11, a.Y - 11, a.Width + 22, a.Height + 22);

            for (auto& list : map) {
                for (auto& block : list) {
                    if (block.GetType() != Block::Type::null) {
                        Rect b(block.GetX(), block.GetY(), Block::kBlockSize, Block::kBlockSize);
                        if (a.IntersectsWith(b)) {                       
                            collision = true;
                        }
                    }
                }
            }
            if (collision) {
                for (auto& list : map) {
                    for (auto& block : list) {
                        if (block.GetType() != Block::Type::null) {
                            Rect b(block.GetX(), block.GetY(), Block::kBlockSize, Block::kBlockSize);

                            if (aForMap.IntersectsWith(b)) {
                                if (block.GetType() == Block::Type::brick) {
                                    block.SetType(Block::Type::null);
                                }
                                collision = true;

                            }
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

            for (auto ob : bullets) {
                Rect b(ob->GetRect());
                if (a.IntersectsWith(b) && ob != bullet) {
                    ob->Damage();
                    collision = true;
                }
            }

            if (base) {
                Rect b(base->GetRect());
                if (a.IntersectsWith(b)) {
                    base->Damage();
                    collision = true;
                }
            }            

            if (collision) {
                bullet->Damage();
            }

        }

    protected:
        virtual void PaintPre(Graphics graphics) final override;
        virtual void PaintChildBuffers(Graphics graphics) final override;
        virtual void PaintPost(Graphics graphics) final override;

    protected:
        std::vector<std::vector<Block>> map;
        std::unique_ptr<Image> brickTexture;
        std::unique_ptr<Image> rockTexture;

        std::unique_ptr<Bitmap> mapTexture;

        // Caching
        SolidBrush blackBrush;
        Gdiplus::TextureBrush brickBrush;
        Gdiplus::TextureBrush rockBrush;


        // Map
        std::list<Tank*> tanks;
        std::mutex tanksMutex;
        std::unique_ptr<std::thread> tanksSpawner;
        bool tanksSpawnerTerminate;


        std::list<Bullet*> bullets;
        Tank* userTank;
        Base* base;


        bool gameover;



        // Graph for bot
        std::unique_ptr<std::thread> worldStateRegenerator;
        bool worldStateRegeneratorTerminate;

        std::vector<std::vector<Node>> nodemap;


    };

}