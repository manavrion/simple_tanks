#include "game_field.h"

#include "gdi_render\g_button.h"
#include "..\entities\bullet.h"

#include <unordered_map>
#include <string>

namespace simple_tanks {


    const int GameField::kMapDim = 13*4;
    const int GameField::kBlockSize = 8;

    GameField::GameField() : 
        mapTexture(new Bitmap(L"resources/map.png")),
        brickTexture(new Image(L"resources/block.png")),        
        blackBrush(Color::Black), 
        brickBrush(brickTexture.get()),
        map(kMapDim, std::vector<Block>(kMapDim, Block())) {

        SetWidth(kBlockSize * kMapDim); //416
        SetHeight(kBlockSize * kMapDim); //416
        

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                Color color;
                mapTexture->GetPixel(i, j, &color);
                if (color.GetValue() == Color::Red) {
                    map[i][j].SetType(Block::Type::brick);
                }
				map[i][j].SetPos(i*kBlockSize, j*kBlockSize);
            }
        }


        // Tanks

        Tank* userTank = new Tank(this);
        tanks.push_back(userTank);
        userTank->MoveTo(4 * 4 * 8, 12 * 4 * 8);

        auto manip = [](KeyEvent* keyEvent, Tank* tank, bool act) {
            switch (keyEvent->keyCode) {
                case KeyEvent::KeyCodes::alphaW:
                    tank->MoveUp(act);
                    break;
                case KeyEvent::KeyCodes::alphaS:
                    tank->MoveDown(act);
                    break;
                case KeyEvent::KeyCodes::alphaA:
                    tank->MoveLeft(act);
                    break;
                case KeyEvent::KeyCodes::alphaD:
                    tank->MoveRight(act);
                    break;
                case KeyEvent::KeyCodes::space:
                    if (act) {
                        tank->Shoot();
                    }
                    break;
                default:
                    break;
            }
        };

        userTank->AddKeyPressListener([=](Frame* frame, KeyEvent* keyEvent) {
            manip(keyEvent, (Tank*)frame, true);
        });

        userTank->AddKeyReleaseListener([=](Frame* frame, KeyEvent* keyEvent) {
            manip(keyEvent, (Tank*)frame, false);
        });
        
        Add(userTank);
    }


    GameField::~GameField() {}



    void GameField::SpawnBullet(Tank* tank) {
        Bullet* b = new Bullet(this, tank);
        bullets.push_back(b);
        Add(b);
    }


    void GameField::PaintPre(Graphics graphics) {

        // Garbage remove
        Bullet* b = nullptr;
        do {
            b = nullptr;
            for (auto ob : bullets) {
                if (!ob->IsAlive()) {
                    b = ob;
                }
            }
            if (b != nullptr) {
                bullets.erase(std::find(bullets.begin(), bullets.end(), b));
                Erase(b);
                delete b;
            }
        } while (b != nullptr);
        

        Tank* t = nullptr;
        do {
            t = nullptr;
            for (auto ob : tanks) {
                if (!ob->IsAlive()) {
                    t = ob;
                }
            }
            if (t != nullptr) {
                tanks.erase(std::find(tanks.begin(), tanks.end(), t));
                Erase(t);
                delete t;
            }
        } while (t != nullptr);


        graphics.FillRectangle(&blackBrush, Rect(0, 0, width, height));

        

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                int x = i * kBlockSize;
                int y = j * kBlockSize;
                if (map[i][j].GetType() == Block::Type::brick) {
                    graphics.FillRectangle(&brickBrush, Rect(x, y, kBlockSize, kBlockSize));
                }
            }
        }
    }
    void GameField::PaintPost(Graphics graphics) {}

}