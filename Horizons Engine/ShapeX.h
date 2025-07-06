// ShapeX.h
#pragma once
#include <Windows.h>
#include <vector>

namespace hz {

    enum ShapeType {
        SHAPE_TRIANGLE,
        SHAPE_PLANE,
        SHAPE_CIRCLE,
        SHAPE_CUBE,
        SHAPE_PYRAMID,
        SHAPE_PRISM,
        SHAPE_SPHERE,
        SHAPE_CONE,
        SHAPE_CYLINDER
    };

    struct ShapeInstance {
        ShapeType type;
        float x, y, z;
        float rotX, rotY, rotZ;
        float scale = 1.0f;
        float r = 1.0f, g = 1.0f, b = 1.0f;

        bool  visible = true;
        void Draw(float overrideR = -1, float overrideG = -1, float overrideB = -1) const;
    };

    extern std::vector<ShapeInstance> shapes;

    void GetShapeTypeName();

} // namespace hz