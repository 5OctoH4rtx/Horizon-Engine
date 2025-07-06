// SelMove.cpp
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ShapeX.h"     // shapes vector + ShapeInstance
#include "Select.h"     // selectedShapeIndex
#include "Frame.h"      // GetDeltaTime()
#include "SelMove.h"

namespace hz {

    // Call once per frame, e.g. right after UpdateSelectionWithMouse()
    void UpdateSelectedMovement()
    {
        if (selectedShapeIndex < 0 ||
            selectedShapeIndex >= static_cast<int>(shapes.size()))
            return;                                 // nothing selected

        ShapeInstance& s = shapes[selectedShapeIndex];

        // frame-rate-independent speed
        float speed = 2.0f * GetDeltaTime();        // units per second
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000)    // hold SHIFT to move faster
            speed *= 5.0f;

        // X / Y movement with arrow keys
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) s.x -= speed;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) s.x += speed;
        if (GetAsyncKeyState(VK_UP) & 0x8000) s.z -= speed;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) s.z += speed;

        // Y movement with + / –  (both main keyboard and num-pad)
        if ((GetAsyncKeyState(VK_OEM_PLUS) | GetAsyncKeyState(VK_ADD)) & 0x8000)
            s.y += speed;
        if ((GetAsyncKeyState(VK_OEM_MINUS) | GetAsyncKeyState(VK_SUBTRACT)) & 0x8000)
            s.y -= speed;
    }

} // namespace hz