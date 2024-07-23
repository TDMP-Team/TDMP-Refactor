#ifndef TDMP_TEARDOWN_H
#define TDMP_TEARDOWN_H

#include "pch.h"
#include "memory/memory.h"
#include "teardown/types.h"

namespace mp::teardown {
    void initialize();

    static types::game_t* game = nullptr;
}

#endif // TDMP_TEARDOWN_H