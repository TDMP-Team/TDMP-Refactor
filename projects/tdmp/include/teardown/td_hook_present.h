#ifndef TDMP_HOOK_PRESENT_H
#define TDMP_HOOK_PRESENT_H

namespace td::renderer {
    enum class type : uint8_t {
        opengl = 0,
        d3d12,
    };

    void hookPresent(renderer::type type);
}

#endif // TDMP_HOOK_PRESENT_H