#pragma once
#include "gdi_render\gdi_control.h"
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class TankLayout {
    public:
        ~TankLayout();

        enum class Direction {
            Up,
            Down,
            Left,
            Right
        };

        static TankLayout* GetGreenTankLayout(Direction direction);
        static TankLayout* GetWhiteTankLayout(Direction direction);

        void MoveUp();
        void MoveDown();
        void MoveLeft();
        void MoveRight();

        Image* GetTankSprite();        

    protected:
        TankLayout(Image* tankTextureUp,
                   Image* tankTextureDown,
                   Image* tankTextureLeft,
                   Image* tankTextureRight,
                   Image* tankTextureTUp,
                   Image* tankTextureTDown,
                   Image* tankTextureTLeft,
                   Image* tankTextureTRight,
                   Direction direction);

        void Move();

    protected:
        Image* tankTexture;
        std::unique_ptr<Image> tankTextureUp;
        std::unique_ptr<Image> tankTextureDown;
        std::unique_ptr<Image> tankTextureLeft;
        std::unique_ptr<Image> tankTextureRight;
        std::unique_ptr<Image> tankTextureTUp;
        std::unique_ptr<Image> tankTextureTDown;
        std::unique_ptr<Image> tankTextureTLeft;
        std::unique_ptr<Image> tankTextureTRight;

    };

}