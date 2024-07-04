#ifndef TDMP_TEARDOWN_H
#define TDMP_TEARDOWN_H

#include "pch.h"
#include "memory/memory.h"

namespace mp::teardown {

    void initialize();
    void earlyEntryThread();

}

#endif // TDMP_TEARDOWN_H