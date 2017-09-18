#pragma once

namespace wnd_accelerator {

    struct Block {
        enum class Type {
            null,
            brick,
            rock
        };

        Type type;
    };

}