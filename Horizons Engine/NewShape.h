// NewShape.h
#pragma once
#include <windows.h>
#include "ShapeX.h"

namespace hz {

    ShapeInstance CreateShape(
        ShapeType type,
        float x, float y, float z,
        float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f,
        float scale = 1.0f,
        float r = 1.0f, float g = 1.0f, float b = 1.0f
    );

    void AddShape(
        ShapeType type,
        float x, float y, float z,
        float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f,
        float scale = 1.0f,
        float r = 1.0f, float g = 1.0f, float b = 1.0f
    );

        void DeleteSelectedShape();
        void CloneSelectedShape();

} // namespace hz