#pragma once
#include "ShapeX.h"   // for shapes vector + ShapeInstance
#include "Select.h"   // for selectedShapeIndex

namespace hz {
    // Call once per frame to allow 'T' key to cycle selected shape's type
    void UpdateSelectedShapeType();
}