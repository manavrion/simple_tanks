#pragma once
#include "gdi_render\gdi_control.h"
#include "i_breakable.h"
#include "tank_layout.h"

#include <thread>
#include <vector>
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GameField;

    class Tank : public GdiControl, public IBreakable {
        friend class GameField;
        static const int kTankSize;
        static const int kStepSize;
    public:
        enum class Direction {
            Up,
            Down,
            Left,
            Right
        };
    public:
        Tank(GameField* gameField, TankLayout* tankLayout);

        void MoveUp(bool moveUp);
        void MoveDown(bool moveDown);
        void MoveLeft(bool moveLeft);
        void MoveRight(bool moveRight);
        void MoveReset();

        void Shoot();



        virtual bool IsAlive() {
            if (health > 0) {
                return true;
            }
            return false;
        }
        virtual void Damage() {
            health--;
        }


        virtual ~Tank();


        Direction GetDirection() {
            return direction;
        }

    protected:

        bool MoveTo(int x, int y) {
            if (IsValidTankPos(x, y)) {
                this->x = x;
                this->y = y;
                return true;
            }
            return false;
        }

        bool IsValidTankPos(int x, int y);


        virtual void PaintPre(Graphics graphics) final override {
            graphics.DrawImage(tankLayout->GetTankSprite(), 0, 0, width, height);
        }

        virtual void PaintPost(Graphics graphics) final override {}

    protected:
        volatile Direction direction;
        volatile bool moveUp;
        volatile bool moveDown;
        volatile bool moveLeft;
        volatile bool moveRight;

        GameField* gameField;

        std::unique_ptr<TankLayout> tankLayout;

        std::unique_ptr<std::thread> tankThread;
        volatile bool tankThreadTerminate;

        int health;
    };

}