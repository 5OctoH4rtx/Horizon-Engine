#pragma once
#include <gl/GL.h>

namespace hz
{
    // Call once at startup if you want a display-list (optional).
    void InitGridDisplayList(int halfSize = 25, float cell = 1.0f);

    // Draws an XY-plane grid (Y-up world) centered at the origin.
    // If you called InitGridDisplayList, this just calls the list.
    // Otherwise it renders immediate-mode lines each frame.
    void DrawGrid(int halfSize = 25, float cell = 1.0f);
}