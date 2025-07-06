#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ShapeX.h"     // shapes vector + ShapeInstance
#include "Select.h"     // selectedShapeIndex
#include "Frame.h"      // GetDeltaTime()
#include "SelRot.h"

namespace hz {

    void UpdateSelectedRotation()
    {
        if (selectedShapeIndex < 0 ||
            selectedShapeIndex >= static_cast<int>(shapes.size()))
            return;                             // nothing selected

        ShapeInstance& s = shapes[selectedShapeIndex];

        // degrees per second; hold Ctrl for faster spin
        float speed = 90.0f * GetDeltaTime();
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
            speed *= 5.0f;

        // clockwise = negative angle by right-hand convention
        if (GetAsyncKeyState('1') & 0x8000) s.rotX -= speed; // rotate about X
        if (GetAsyncKeyState('2') & 0x8000) s.rotY -= speed; // rotate about Y
        if (GetAsyncKeyState('3') & 0x8000) s.rotZ -= speed; // rotate about Z
    }

} // namespace hz
