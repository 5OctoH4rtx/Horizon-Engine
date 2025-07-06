#pragma once
#include <string>

namespace hz {

    struct ShapeStats {
        bool   valid = false;        // false if index out of range
        std::string type;

        float x = 0, y = 0, z = 0;
        float rotX = 0, rotY = 0, rotZ = 0;
        float scale = 1.0f;
        float r = 1, g = 1, b = 1;
    };

    // Writes stats of shape[index] into outStats; returns true if valid.
    bool GetShapeStats(int index, ShapeStats& outStats);

} // namespace hz