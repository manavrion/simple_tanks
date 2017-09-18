#include "gui_game_main_window.h"

#include "gui_battle.h"
#include "gui_menu.h"
#include "gdi_render\g_button.h"

namespace simple_tanks {

    static GuiGameMainWindow* guiGameMainWindow = nullptr;


    GuiGameMainWindow::GuiGameMainWindow() 
        : currentMenu(Menu::main), 
        frameUpdaterThreadTerminate(false) {

        guiGameMainWindow = this;

        Size size(800, 420);

        SetText("Battle City 20!8");
        SetSize(size);
        SetStaticSize();
        SetBackground(Color::Black);

        GuiMenu* guiMainMenu = new GuiMenu();
        GuiBattle* guiGame = new GuiBattle();

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


        frameUpdaterThread.reset(new std::thread([&]() {
            while (true) {
                if (frameUpdaterThreadTerminate) {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                mutex.lock();
                for (auto frame : alwaysRepaint) {
                    frame->MarkToRepaint();
                }
                mutex.unlock();
                Repaint();
            }
        }));
    }

    GuiGameMainWindow::~GuiGameMainWindow() {
        frameUpdaterThreadTerminate = true;
        frameUpdaterThread->join();

        for (auto& key : menuMap) {
            delete key.second;
        }

        guiGameMainWindow = nullptr;
    }
    
    // static
    void GuiGameMainWindow::AddDynamicObject(Frame* frame) {
        if (guiGameMainWindow) {
            guiGameMainWindow->mutex.lock();
            guiGameMainWindow->alwaysRepaint.push_back(frame);
            guiGameMainWindow->mutex.unlock();
        }
    }

    // static
    void GuiGameMainWindow::EraseDynamicObject(Frame* frame) {
        if (guiGameMainWindow) {
            guiGameMainWindow->mutex.lock();
            guiGameMainWindow->alwaysRepaint.erase(
                std::find(guiGameMainWindow->alwaysRepaint.begin(), 
                          guiGameMainWindow->alwaysRepaint.end(), frame));
            guiGameMainWindow->mutex.unlock();
        }
    }

    void GuiGameMainWindow::NotifyKeyListeners(KeyEvent* keyEvent) {
        Window::NotifyKeyListeners(keyEvent);
        menuMap[currentMenu]->NotifyKeyListeners(keyEvent);
    }

    void GuiGameMainWindow::NotifyMouseListeners(MouseEvent* mouseEvent) {
        Window::NotifyMouseListeners(mouseEvent);
        menuMap[currentMenu]->NotifyMouseListeners(mouseEvent);
    }

    void GuiGameMainWindow::PaintChildBuffers(Graphics graphics) {
        if (((GuiGameMainWindow*)menuMap[currentMenu])->paint) {
            menuMap[currentMenu]->Paint(graphics);
        }
    }

}