#ifndef TDMP_FUNCTION_HINTS_H
#define TDMP_FUNCTION_HINTS_H

#include "pch.h"

#include "memory/memory.h"

/*
 * These hint functions are to help generate the game structures
 * If the game updates, it's likely that the structure will change. So with this, we rely on the function
 * layout instead and read the offsets instead. I tested it with 1.5.4 to 1.6.0, it worked pretty well.

 * I only had to change 1 or 2 `readInt32`'s to `readInt8`. It's also easy to do this since you can very easily
 * find the function within IDA.
*/

namespace mp::function_hints {

    static uintptr_t getTeardownStructure(uintptr_t base) {
        int32_t teardownStructureOffset = mem::readInt32(base + 3);
        return mem::readUintptr(base + 7 + teardownStructureOffset);
    }

    namespace player {
        inline void getPos(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base + 0x1B);
            int32_t playerOffset = mem::readInt32(base + 0x22 + 3);

            structureOrder = { structure_order_element("player"), structure_order_element("pos", "teardown::types::vector3_t") };
            offsetOrder = { playerOffset, 0x0 };
        }

        // Some of these have been modified to work with 1.6.0 experimental, so might need changing a tiny bit for 1.5.4

        inline void getVelocity(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t playerOffset = mem::readInt32(base + 19);
            int32_t velocityOffset = mem::readInt32(base + 23 + 3); // 1.5.4 should use readInt8

            structureOrder = { structure_order_element("player"), structure_order_element("velocity", "teardown::types::vector3_t")};
            offsetOrder = { playerOffset, velocityOffset };
        }

        inline void getVehicle(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t sceneOffset = mem::readInt8(base + 0x7 + 3);
            int32_t vehicleOffset = mem::readInt32(base + 0xB + 3);
            int32_t vehicleHandleOffset = mem::readInt8(base + 0x17 + 2);

            structureOrder = { structure_order_element("scene", "scene*"), structure_order_element("vehicle", "vehicle*", true), structure_order_element("handle", "int32_t")};
            offsetOrder = { sceneOffset, vehicleOffset, vehicleHandleOffset };
        }

        inline void isGrounded(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t playerOffset = mem::readInt32(base + 0xA + 3);
            int32_t goundedOffset = mem::readInt32(base + 0x11 + 3);

            structureOrder = { structure_order_element("player"), structure_order_element("isGrounded", "bool") };
            offsetOrder = { playerOffset, goundedOffset };
        }

        inline void getGrabShape(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t playerOffset = mem::readInt32(base + 10);
            int32_t grabbedBodyOffset = mem::readInt32(base + 17);
            int32_t grabbedBodyHandleOffset = mem::readInt8(base + 0x1A + 2);

            structureOrder = { structure_order_element("player"), structure_order_element("shape", "shape*", true), structure_order_element("handle", "int32_t") };
            offsetOrder = { playerOffset, grabbedBodyOffset, grabbedBodyHandleOffset };
        }

        inline void getGrabBody(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t playerOffset = mem::readInt32(base + 10);
            int32_t grabbedBodyOffset = mem::readInt32(base + 0xE + 3);
            int32_t grabbedBodyHandleOffset = mem::readInt8(base + 0x1A + 2);

            structureOrder = { structure_order_element("player"), structure_order_element("body", "body*", true), structure_order_element("handle", "int32_t")};
            offsetOrder = { playerOffset, grabbedBodyOffset, grabbedBodyHandleOffset };
        }

        inline void getHealth(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t playerOffset = mem::readInt32(base + 10);
            int32_t healthOffset = mem::readInt32(base + 18);

            structureOrder = { structure_order_element("player"), structure_order_element("health", "float") };
            offsetOrder = { playerOffset, healthOffset };
        }

        inline void respawn(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t respawnOffset = mem::readInt32(base + 9);

            structureOrder = { structure_order_element("something"), structure_order_element("respawnFlag", "bool") };
            offsetOrder = { 0x0, respawnOffset };
        }

        inline void getWalkingSpeed(HINT_CALLBACK_ARGS) {
            uintptr_t teardownStructure = getTeardownStructure(base);

            int32_t playerOffset = mem::readInt32(base + 10);
            int32_t walkingSpeedOffset = mem::readInt32(base + 18);

            structureOrder = { structure_order_element("player"), structure_order_element("walkingSpeed", "float") };
            offsetOrder = { playerOffset, walkingSpeedOffset };
        }
    }

    namespace body {
        //inline void getTransform(HINT_CALLBACK_ARGS) {
        //    uintptr_t teardownStructure = getTeardownStructure(base + 0x36);

        //    int32_t sceneOffset = mem::readInt8(base + 0x3D + 3);

        //    // body = *(_QWORD *)(*(_QWORD *)(scene + 0x9D0) + 8i64 * handle);
        //    int32_t sceneBodies = mem::readInt32(base + 0x51 + 3);
        //    int32_t transformOffset = 0x0;

        //    // This is actually going to be a vector of bodies
        //    structureOrder = { structure_order_element("scene"),  structure_order_element("body", "body*", true), structure_order_element("transform", "teardown::types::transform_t")};
        //    offsetOrder = { sceneOffset, sceneBodies, transformOffset };
        //}
    }

}

#endif // TDMP_FUNCTION_HINTS_H