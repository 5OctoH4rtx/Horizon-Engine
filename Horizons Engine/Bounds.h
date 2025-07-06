#pragma once
#include "ShapeX.h"   // for ShapeType and ShapeInstance

namespace hz {

    // -------- data structures --------
    struct SphereBounds {
        float radius;                  // always positive
    };

    struct AABB {                      // Axis-aligned bounding box
        float minX, minY, minZ;
        float maxX, maxY, maxZ;
    };

    // -------- helper functions --------
    SphereBounds  CalcBoundingSphere(const ShapeInstance& s);
    AABB          CalcAABB(const ShapeInstance& s);

} // namespace hz
