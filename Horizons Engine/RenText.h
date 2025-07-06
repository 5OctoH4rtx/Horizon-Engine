// RenText.h
#pragma once
#include <string>

namespace hz {
    // Call *once* after you have a valid, current OpenGL context (and HDC)
    // e.g. right after wglMakeCurrent() succeeds.
    bool InitFontBitmaps();  // returns false on failure

    // Renders `msg` in screen?space pixels (top?left = 0,0)
    // r,g,b are 0?1 floats;  bold=true uses a thicker glyph set.
    void RenderTextOnScreen(const std::string& msg,
        int xPixels, int yPixels,
        float r = 1.f, float g = 1.f, float b = 1.f,
        bool bold = false);
}