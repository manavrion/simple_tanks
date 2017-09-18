#pragma once
#include "..\entities\tank.h"
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

		struct Block : public IBreakable {
			enum class Type {
				null,
				brick,
				rock
			};

			virtual Rect GetRect() override {
				return Rect(x, y, kBlockSize, kBlockSize);
			}
			virtual int GetHealth() override {}
			virtual void SetHealth(int health) override {}

			virtual bool IsAlive() override {}

			virtual void TestBullet(Bullet* bullet) override {
				Rect rect = bullet->GetRect();

			}

			void SetPos(int x, int y) {
				this->x = x;
				this->y = y;
			}

			Type type;
		protected:			
			bool health;

			int x;
			int y;
		};
    public:
        GameField();
        virtual ~GameField();

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