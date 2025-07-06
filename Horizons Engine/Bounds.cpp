#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Bounds.h"

// --- per-shape unit-scale half-sizes (|x|, |y|, |z|) and sphere radii ---
// All shapes in Shape.cpp are centered at the origin and scaled by s.scale.
namespace {
    struct ShapeDims { float hx, hy, hz, radius; };

    ShapeDims dimsFor(hz::ShapeType t)
    {
        using namespace hz;
        switch (t) {
        case SHAPE_CUBE: return { 0.5f, 0.5f, 0.5f, 0.866f }; // sqrt(3)/2
        case SHAPE_SPHERE: return { 0.4f, 0.4f, 0.4f, 0.4f };
        case SHAPE_PYRAMID: return { 0.5f, 1.0f, 0.5f, 1.118f }; // approx
        case SHAPE_PRISM: return { 0.5f, 1.0f, 0.5f, 1.225f };
        case SHAPE_PLANE: return { 0.5f, 0.0f, 0.5f, 0.707f };
        case SHAPE_CIRCLE: return { 0.5f, 0.0f, 0.5f, 0.5f };
        case SHAPE_TRIANGLE: return { 0.5f, 0.5f, 0.0f, 0.707f };
        case SHAPE_CONE: return { 0.5f, 1.0f, 0.5f, 1.118f };
        case SHAPE_CYLINDER: return { 0.5f, 1.0f, 0.5f, 1.118f };
        default: return { 1.0f, 1.0f, 1.0f, 1.732f }; // safe fallback
        }
    }
} // unnamed namespace

// -------- public helpers --------
namespace hz {

    SphereBounds CalcBoundingSphere(const ShapeInstance& s)
    {
        ShapeDims d = dimsFor(s.type);
        return { d.radius * s.scale };
    }

    AABB CalcAABB(const ShapeInstance& s)
    {
        ShapeDims d = dimsFor(s.type);
        float hx = d.hx * s.scale;
        float hy = d.hy * s.scale;
        float hz = d.hz * s.scale;
        return { s.x - hx, s.y - hy, s.z - hz,
                 s.x + hx, s.y + hy, s.z + hz };
    }

} // namespace hz
