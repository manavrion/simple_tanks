#pragma once
#include <utility>
#include <thread>


namespace simple_tanks {

    class Game {
    public:
        Game() {

        }
        ~Game() {

        }
    protected:
        std::unique_ptr<std::thread> frameUpdater;
    };



}