#pragma once
#include "tank.h"

#include <gdi_render\gdi_control.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>

namespace simple_tanks {
    using namespace wnd_accelerator;

    struct Block {
        enum class Type {
            null,
            brick
        };
        Type type;
    };


    class GameField : public GdiControl {
        friend class Tank;
        int mapDim;
        int blockSize;
    public:
        GameField() : 
            mapDim(13*4),
            blockSize(8),
            map(mapDim, std::vector<Block>(mapDim, {Block::Type::null})) {

            SetWidth(blockSize * mapDim); //416
            SetHeight(blockSize * mapDim); //416


            brickTexture.reset(new Image(L"resources/block.png"));
            mapTexture.reset(new Bitmap(L"resources/map.png"));

            
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

            userTank->MoveTo(4*4 * 8, 12*4 * 8);

            
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
        virtual ~GameField() {}

    protected:

        virtual void PaintPre(Gdiplus::Graphics *graphics) final {
            graphics->FillRectangle(&SolidBrush(Color::Black), Rect(0, 0, width, height));

            Gdiplus::TextureBrush brick(brickTexture.get());

            for (size_t i = 0; i < map.size(); i++) {
                for (size_t j = 0; j < map[i].size(); j++) {
                    int x = i * blockSize;
                    int y = j * blockSize;
                    if (map[i][j].type == Block::Type::brick) {
                        graphics->FillRectangle(&brick, Rect(x, y, blockSize, blockSize));
                    }
                }
            }
        }

        virtual void PaintPost(Gdiplus::Graphics *graphics) final {
            
        }

    protected:
        std::vector<std::vector<Block>> map;
        std::unique_ptr<Image> brickTexture;

        std::unique_ptr<Bitmap> mapTexture;
    };

}