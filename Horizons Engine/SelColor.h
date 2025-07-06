#pragma once
#include "ShapeX.h"
#include "Select.h"

namespace hz {

    struct ColorRGB { float r, g, b; };

    // Call every frame to handle [ / ] cycling and Ctrl+bracket snap-to-grey
    void UpdateColorCycle();

    void UpdateSelectedColorCycle();

    // Return the current palette color
    ColorRGB GetCurrentCycleColor();

    // Don't define variables or functions here!
}
