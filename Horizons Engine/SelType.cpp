// SelType.cpp
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ShapeX.h"
#include "Select.h"
#include "SelType.h"

namespace hz {

    // total number of shapes in your ShapeType enum (update if you add more)
    static constexpr int kShapeCount = 9; // TRIANGLE..CYLINDER

    void UpdateSelectedShapeType()
    {
        if (selectedShapeIndex < 0 || selectedShapeIndex >= static_cast<int>(shapes.size()))
            return; // nothing selected

        static bool tLast = false;
        bool tNow = (GetAsyncKeyState('T') & 0x8000) != 0;

        if (tNow && !tLast) {
            ShapeInstance& s = shapes[selectedShapeIndex];
            int next = (static_cast<int>(s.type) + 1) % kShapeCount;
            s.type = static_cast<ShapeType>(next);
        }
        tLast = tNow;
    }

} // namespace hz
