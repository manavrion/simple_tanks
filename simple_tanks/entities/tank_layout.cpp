#include "tank_layout.h"


namespace simple_tanks {


    TankLayout::TankLayout(Image* tankTextureUp,
                           Image* tankTextureDown,
                           Image* tankTextureLeft,
                           Image* tankTextureRight,
                           Image* tankTextureTUp,
                           Image* tankTextureTDown,
                           Image* tankTextureTLeft,
                           Image* tankTextureTRight,
                           Direction direction) :
    tankTextureUp(tankTextureUp),
    tankTextureDown(tankTextureDown),
    tankTextureLeft(tankTextureLeft),
    tankTextureRight(tankTextureRight),
    tankTextureTUp(tankTextureTUp),
    tankTextureTDown(tankTextureTDown),
    tankTextureTLeft(tankTextureTLeft),
    tankTextureTRight(tankTextureTRight)
    {
        switch (direction) {
            case simple_tanks::TankLayout::Direction::Up:
                tankTexture = tankTextureUp;
                break;
            case simple_tanks::TankLayout::Direction::Down:
                tankTexture = tankTextureDown;
                break;
            case simple_tanks::TankLayout::Direction::Left:
                tankTexture = tankTextureLeft;
                break;
            case simple_tanks::TankLayout::Direction::Right:
                tankTexture = tankTextureRight;
                break;
            default:
                break;
        }
    }

    void TankLayout::Move() {
        swap(tankTextureTUp, tankTextureUp);
        swap(tankTextureTDown, tankTextureDown);
        swap(tankTextureTLeft, tankTextureLeft);
        swap(tankTextureTRight, tankTextureRight);
    }

    TankLayout::~TankLayout() {}

    // static
    TankLayout* TankLayout::GetGreenTankLayout(Direction direction) {
        return new TankLayout(new Image(L"resources/g_tank_l1_u.png"),
                              new Image(L"resources/g_tank_l1_d.png"),
                              new Image(L"resources/g_tank_l1_l.png"),
                              new Image(L"resources/g_tank_l1_r.png"),
                              new Image(L"resources/t_g_tank_l1_u.png"),
                              new Image(L"resources/t_g_tank_l1_d.png"),
                              new Image(L"resources/t_g_tank_l1_l.png"),
                              new Image(L"resources/t_g_tank_l1_r.png"),
                              direction);
    }

    // static
    TankLayout* TankLayout::GetWhiteTankLayout(Direction direction) {
        return new TankLayout(new Image(L"resources/w_tank_l1_u.png"),
                              new Image(L"resources/w_tank_l1_d.png"),
                              new Image(L"resources/w_tank_l1_l.png"),
                              new Image(L"resources/w_tank_l1_r.png"),
                              new Image(L"resources/t_w_tank_l1_u.png"),
                              new Image(L"resources/t_w_tank_l1_d.png"),
                              new Image(L"resources/t_w_tank_l1_l.png"),
                              new Image(L"resources/t_w_tank_l1_r.png"),
                              direction);
    }

    void TankLayout::MoveUp() {
        tankTexture = tankTextureUp.get();
        Move();
    }

    void TankLayout::MoveDown() {
        tankTexture = tankTextureDown.get();
        Move();
    }

    void TankLayout::MoveLeft() {
        tankTexture = tankTextureLeft.get();
        Move();
    }

    void TankLayout::MoveRight() {
        tankTexture = tankTextureRight.get();
        Move();
    }

    Image* TankLayout::GetTankSprite() {
        return tankTexture;
    }

}