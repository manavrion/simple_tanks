#pragma once
#include "direction.h"

namespace simple_tanks {

    class Block {
    public:
        static const int kBlockSize;
        enum class Type {
            null,
            brick,
            rock
        };
    public:
        Block();
        ~Block();


        Type GetType();

        void SetType(Type type);

        void SetPos(int x, int y);

        int GetX() {
            return x;
        }

        int GetY() {
            return y;
        }

    protected:
        Type type;
        int x;
        int y;
    };

}