// NewShape.cpp
#include <windows.h>
#include "ShapeX.h"
#include "Shape.h"
#include "Select.h"
#include "sound.h"

#pragma once

namespace hz {

    ShapeInstance CreateShape(
        ShapeType type,
        float x, float y, float z,
        float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f,
        float scale = 1.0f,
        float r = 1.0f, float g = 1.0f, float b = 1.0f
    ) {
        ShapeInstance shape;
        shape.type = type;
        shape.x = x;
        shape.y = y;
        shape.z = z;
        shape.rotX = rotX;
        shape.rotY = rotY;
        shape.rotZ = rotZ;
        shape.scale = scale;
        shape.r = r;
        shape.g = g;
        shape.b = b;
        return shape;
    }

    void AddShape(
        ShapeType type,
        float x, float y, float z,
        float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f,
        float scale = 1.0f,
        float r = 1.0f, float g = 1.0f, float b = 1.0f
    ) {
        shapes.push_back(CreateShape(type, x, y, z, rotX, rotY, rotZ, scale, r, g, b));

    }

    void DeleteSelectedShape()
    {
        if (selectedShapeIndex < 0 || selectedShapeIndex >= (int)shapes.size())
            return; // no valid selection

        // Remove the shape at selectedShapeIndex
        shapes.erase(shapes.begin() + selectedShapeIndex);

        // Reset selection index
        selectedShapeIndex = -1;

        const char* soundPath = "C:\\AudAsset\\select_004.wav";
        hz::PlaySoundFile(soundPath);
    }

    void CloneSelectedShape() {
        if (selectedShapeIndex < 0 || selectedShapeIndex >= (int)shapes.size())
            return;

        ShapeInstance clone = shapes[selectedShapeIndex];
        clone.x += 1.0f; // offset clone slightly to the right
        shapes.push_back(clone);

            const char* soundPath = "C:\\AudAsset\\glass_004.wav";
            hz::PlaySoundFile(soundPath);
    }

} // namespace hz
