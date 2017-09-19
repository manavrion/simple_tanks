#pragma once
#include "native_controls\window.h"

#include <mutex>
#include <thread>
#include <unordered_map>
#include <utility>

namespace simple_tanks {
    using namespace wnd_accelerator;

    class GuiGameMainWindow : public Window {
    public:
        GuiGameMainWindow();
        virtual ~GuiGameMainWindow();

        static void AddDynamicObject(Frame* frame);
        static void EraseDynamicObject(Frame* frame);
        static GuiGameMainWindow* GetInstance();
        static bool IsRestart();
        static bool Reclose();
                
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
        volatile bool frameUpdaterThreadTerminate;
        std::mutex mutex;
    };

}