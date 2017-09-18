#include "game_field.h"

#include "gdi_render\g_button.h"

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
        map(kMapDim, std::vector<Block>(kMapDim, { Block::Type::null })) {

        SetWidth(kBlockSize * kMapDim); //416
        SetHeight(kBlockSize * kMapDim); //416
        

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                Color color;
                mapTexture->GetPixel(i, j, &color);
                if (color.GetValue() == Color::Red) {
                    map[i][j].type = Block::Type::brick;
                }

            }
        }


        // Tanks

        Tank* userTank = new Tank(this);
        userTank->MoveTo(4 * 4 * 8, 12 * 4 * 8);

        userTank->AddKeyPressListener([=](Frame* frame, KeyEvent* keyEvent) {
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaW) {
                userTank->MoveUp();
            }
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaS) {
                userTank->MoveDown();
            }
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaA) {
                userTank->MoveLeft();
            }
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaD) {
                userTank->MoveRight();
            }
            frame->Repaint();
        });

        userTank->AddKeyReleaseListener([=](Frame* frame, KeyEvent* keyEvent) {
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaW) {
                userTank->MoveReset();
            }
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaS) {
                userTank->MoveReset();
            }
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaA) {
                userTank->MoveReset();
            }
            if (keyEvent->keyCode == KeyEvent::KeyCodes::alphaD) {
                userTank->MoveReset();
            }
            frame->Repaint();
        });
        
        Add(userTank);
    }


    GameField::~GameField() {}
    void GameField::PaintPre(Graphics graphics) {
        graphics.FillRectangle(&blackBrush, Rect(0, 0, width, height));

        

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                int x = i * kBlockSize;
                int y = j * kBlockSize;
                if (map[i][j].type == Block::Type::brick) {
                    graphics.FillRectangle(&brickBrush, Rect(x, y, kBlockSize, kBlockSize));
                }
            }
        }
    }
    void GameField::PaintPost(Graphics graphics) {}

}