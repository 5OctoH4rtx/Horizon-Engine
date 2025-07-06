#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "SelColor.h"

namespace hz {

    // ---------- palette ---------------------------------------------------------
    static const ColorRGB colorCycle[] = {
        {1.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, {0.3f, 0.3f, 0.3f}, {0.0f, 0.0f, 0.0f},
        {0.6f, 0.3f, 0.1f}, {0.8f, 0.6f, 0.4f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f},
        {0.3f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.84f, 0.0f},
        {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {0.0f, 0.5f, 0.5f},
        {0.0f, 1.0f, 1.0f}, {0.6f, 0.8f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.5f},
        {0.7f, 0.6f, 1.0f}, {0.5f, 0.0f, 0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.75f, 0.8f}
    };
    static constexpr int paletteSize = sizeof(colorCycle) / sizeof(ColorRGB);
    static int colorIndex = 1; // start at grey

    bool colorChanged = false;

    // ---------- helpers ---------------------------------------------------------
    static void applyColorToSelected()
    {
        const auto& c = colorCycle[colorIndex];
        shapes[selectedShapeIndex].r = c.r;
        shapes[selectedShapeIndex].g = c.g;
        shapes[selectedShapeIndex].b = c.b;
    }

    void UpdateColorCycle()
    {
        static bool leftLast = false;
        static bool rightLast = false;

        bool ctrlDown = (GetAsyncKeyState(VK_LCONTROL) | GetAsyncKeyState(VK_RCONTROL)) & 0x8000;
        bool leftNow = (GetAsyncKeyState(VK_OEM_4) & 0x8000);  // '[' key
        bool rightNow = (GetAsyncKeyState(VK_OEM_6) & 0x8000); // ']' key

        if (ctrlDown && ((leftNow && !leftLast) || (rightNow && !rightLast))) {
            colorIndex = 1; // reset to grey
            bool colorChanged = true;
        }
        else {
            if (leftNow && !leftLast) {
                colorIndex = (colorIndex - 1 + paletteSize) % paletteSize;
                bool colorChanged = true;
            }
            if (rightNow && !rightLast) {
                colorIndex = (colorIndex + 1) % paletteSize;
                bool colorChanged = true;
            }
        }

        leftLast = leftNow;
        rightLast = rightNow;


        // Only apply color to selected shape if color just changed this frame
        if (colorChanged && selectedShapeIndex >= 0 && selectedShapeIndex < (int)shapes.size()) {
            const auto& c = colorCycle[colorIndex];
            shapes[selectedShapeIndex].r = c.r;
            shapes[selectedShapeIndex].g = c.g;
            shapes[selectedShapeIndex].b = c.b;
        }
    }

    void UpdateSelectedColorCycle()
    {
        static bool leftLast = false;
        static bool rightLast = false;

        bool ctrlDown = (GetAsyncKeyState(VK_LCONTROL) | GetAsyncKeyState(VK_RCONTROL)) & 0x8000;
        bool leftNow = (GetAsyncKeyState(VK_OEM_4) & 0x8000);  // '[' key
        bool rightNow = (GetAsyncKeyState(VK_OEM_6) & 0x8000); // ']' key

        bool colorChanged = false;

        if (ctrlDown && ((leftNow && !leftLast) || (rightNow && !rightLast))) {
            colorIndex = 1; // reset to grey
            colorChanged = true;
        }
        else {
            if (leftNow && !leftLast) {
                colorIndex = (colorIndex - 1 + paletteSize) % paletteSize;
                colorChanged = true;
            }
            if (rightNow && !rightLast) {
                colorIndex = (colorIndex + 1) % paletteSize;
                colorChanged = true;
            }
        }

        if (colorChanged && selectedShapeIndex >= 0 && selectedShapeIndex < (int)shapes.size()) {
            const auto& c = colorCycle[colorIndex];
            shapes[selectedShapeIndex].r = c.r;
            shapes[selectedShapeIndex].g = c.g;
            shapes[selectedShapeIndex].b = c.b;
        }

        leftLast = leftNow;
        rightLast = rightNow;
    }


    ColorRGB GetCurrentCycleColor()
    {
        return colorCycle[colorIndex];
    }

} // namespace hz
