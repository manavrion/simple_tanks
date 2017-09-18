#pragma once
#include "gui_menu.h"
#include "gui_battle.h"

#include <native_controls\window.h>
#include <gdi_render\g_button.h>
#include <unordered_map>
#include <utility>
#include <thread>
#include <mutex>

namespace simple_tanks {

    class GuiGameMainWindow : public wnd_accelerator::Window {
    public:
        GuiGameMainWindow();
        virtual ~GuiGameMainWindow();

        static void AddDynamicObject(Frame* frame);
        static void EraseDynamicObject(Frame* frame);

        
        virtual void NotifyKeyListeners(KeyEvent *keyEvent) final override;
        virtual void NotifyMouseListeners(MouseEvent *mouseEvent) final override;

    protected:

        void PaintChildBuffers(Graphics graphics) final override;

    protected:
        enum class Menu {
            main,
            pause,
            game
        };

    protected:
        Menu currentMenu;
        std::unordered_map<Menu, Frame*> menuMap;

        std::vector<Frame*> alwaysRepaint;
        std::unique_ptr<std::thread> frameUpdaterThread;
        bool frameUpdaterThreadTerminate;
        std::mutex mutex;
    };

}