#pragma once
#include <gdi_render\gdi_control.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>
#include <thread>
//#include "game_window.h"

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GameField;

    class Tank : public GdiControl {
        friend class GameField;
        int tankSize;
        int stepSize;

        enum class Move {
            Null,
            Up,
            Down,
            Left,
            Right
        };
        Move move;
        GameField* gameField;
    public:
        Tank(GameField* gameField);


        void MoveUp() {
            move = Move::Up;
        }

        void MoveDown() {
            move = Move::Down;
        }

        void MoveLeft() {
            move = Move::Left;
        }

        void MoveRight() {
            move = Move::Right;
        }

        void MoveReset() {
            move = Move::Null;
        }


        Point GetMovePos() {
            return Point(x/ stepSize, y/ stepSize);
        }


        virtual ~Tank() {
            tankThread->~thread();
            tankThread.reset();
        }

    protected:

        void MoveTo(int x, int y) {
            if (IsValidTankPos(x, y)) {
                this->x = x;
                this->y = y;
            }
        }

        bool IsValidTankPos(int x, int y);


        virtual void PaintPre(Graphics graphics) final {
            graphics.DrawImage(tankTexture, 0, 0, width, height);
        }

        virtual void PaintPost(Graphics graphics) final {

        }

    protected:
        Image* tankTexture;
        std::unique_ptr<Image> tankTextureUp;
        std::unique_ptr<Image> tankTextureDown;
        std::unique_ptr<Image> tankTextureLeft;
        std::unique_ptr<Image> tankTextureRight;

        std::unique_ptr<std::thread> tankThread;
    };

}