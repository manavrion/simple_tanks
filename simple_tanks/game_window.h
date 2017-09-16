#pragma once
#include "gui_main_menu.h"
#include "gui_game.h"

#include <native_controls\window.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>

namespace simple_tanks {

    using namespace Gdiplus;

    class GameWindow : public wnd_accelerator::Window {
    public:
        GameWindow() : currentMenu(Menu::main) {
            Size size(800, 420);

            SetText("Battle City 20!8");
            SetSize(size);
            SetStaticSize();
            SetBackground(Color::Black);

            GuiMainMenu* guiMainMenu = new GuiMainMenu();
            GuiGame* guiGame = new GuiGame();

            menuMap[Menu::main] = guiMainMenu;
            menuMap[Menu::game] = guiGame;

            guiMainMenu->SetSize(size);
            guiGame->SetSize(size);


            guiMainMenu->Add((new GButton)
                             ->SetWidth(150)
                             ->SetHeight(50)
                             ->SetText("Play")
                             ->SetX((800 / 2) - 150 / 2)
                             ->SetY((420 / 2) - 50 / 2 + 100)
                             ->AddLeftMouseButtonReleaseListener(
                                 [&](Frame* frame, MouseEvent* e) {
                                    currentMenu = Menu::game;
                                    Repaint();
                                 })
            );





            guiMainMenu->Build();
            guiGame->Build();

            guiMainMenu->SetParent(this);
            guiGame->SetParent(this);
        }
        virtual ~GameWindow() {
            for (auto& key : menuMap) {
                delete key.second;
            }
        }



        virtual void NotifyKeyListeners(KeyEvent *keyEvent) final {
            Window::NotifyKeyListeners(keyEvent);
            menuMap[currentMenu]->NotifyKeyListeners(keyEvent);
        }
        virtual void NotifyMouseListeners(MouseEvent *mouseEvent) final {
            Window::NotifyMouseListeners(mouseEvent);
            menuMap[currentMenu]->NotifyMouseListeners(mouseEvent);
        }

    protected:

        void PaintChildBuffers(Graphics* graphics) {
            if (((GameWindow*)menuMap[currentMenu])->paint) {
                menuMap[currentMenu]->Paint();
            }
            graphics->DrawImage(((GameWindow*)menuMap[currentMenu])->buffer, menuMap[currentMenu]->GetRect());
        }

    protected:
        enum class Menu {
            main,
            pause,
            game
        };

    protected:
        Menu currentMenu;
        std::unordered_map<Menu, Frame*> menuMap;
    };

}