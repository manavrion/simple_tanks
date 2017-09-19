#include "block.h"

#include "../gui/game_field.h"

namespace simple_tanks {

    const int Block::kBlockSize = GameField::kBlockSize;
    
    Block::Block() : type(Type::null) {}

    Block::~Block() {}

    Block::Type Block::GetType() {
        return type;
    }

    void Block::SetType(Type type) {
        this->type = type;
    }

    void Block::SetPos(int x, int y) {
        this->x = x;
        this->y = y;
    }

}