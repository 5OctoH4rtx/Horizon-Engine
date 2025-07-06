#define NOMINMAX

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <algorithm>
#include "ShapeX.h"     // shapes vector + ShapeInstance
#include "Select.h"     // selectedShapeIndex
#include "Frame.h"      // GetDeltaTime
#include "SelScale.h"

namespace hz {

    void UpdateSelectedScale()
    {
        if (selectedShapeIndex < 0 ||
            selectedShapeIndex >= static_cast<int>(shapes.size()))
            return;  // nothing selected

        ShapeInstance& s = shapes[selectedShapeIndex];

        // units per second; Ctrl speeds it up
        float delta = 1.0f * GetDeltaTime();
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
            delta *= 5.0f;

        // VK_OEM_COMMA  (',')   scale down
        if (GetAsyncKeyState(VK_OEM_COMMA) & 0x8000)
            s.scale = std::max(0.05f, s.scale - delta);

        // VK_OEM_PERIOD ('.')   scale up
        if (GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000)
            s.scale += delta;
    }

} // namespace hz
