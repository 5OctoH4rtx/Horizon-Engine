// Select.h
#pragma once
#include "ShapeX.h"
#include "Bounds.h"

namespace hz {

    // Stores the index of the currently selected shape, or -1 if none
    extern int selectedShapeIndex;

    // Ray origin and direction in world space
    struct Ray {
        float ox, oy, oz; // origin
        float dx, dy, dz; // direction (normalized)
    };

    // Converts screen mouse coordinates to a picking ray in world space
    Ray ScreenToRay(int mouseX, int mouseY, int screenWidth, int screenHeight);

    // Tests if the ray intersects an AABB. Returns true if hit.
    bool RayIntersectsAABB(const Ray& ray, const AABB& box, float& tHit);

    // Selects the first shape hit by the ray (closest to camera)
    void TrySelectShape(const Ray& ray);

    // Draws a white outline around the currently selected shape
    void DrawSelectionOutline();

}
