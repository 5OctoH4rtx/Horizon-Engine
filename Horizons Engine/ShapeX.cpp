// ShapeX.cpp
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <GL/gl.h>
#include "Shape.h"
#include "ShapeX.h"

#pragma once

namespace hz {

    // Define the Draw function OUTSIDE the struct
    void ShapeInstance::Draw(float overrideR, float overrideG, float overrideB) const {
        glPushMatrix();

        glTranslatef(x, y, z);
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);
        glRotatef(rotZ, 0, 0, 1);
        glScalef(scale, scale, scale);

        if (overrideR >= 0 && overrideG >= 0 && overrideB >= 0) {
            glColor3f(overrideR, overrideG, overrideB);
        }
        else {
            glColor3f(r, g, b);
        }

        switch (type) {
        case SHAPE_TRIANGLE:  DrawTriangle();  break;
        case SHAPE_PLANE:     DrawPlane();     break;
        case SHAPE_CIRCLE:    DrawCircle();    break;
        case SHAPE_CUBE:      DrawCube();      break;
        case SHAPE_PYRAMID:   DrawPyramid();   break;
        case SHAPE_PRISM:     DrawPrism();     break;
        case SHAPE_SPHERE:    DrawSphere();    break;
        case SHAPE_CONE:      DrawCone();      break;
        case SHAPE_CYLINDER:  DrawCylinder();  break;
        }

        glPopMatrix();
    }

    // Define the global shape list
    std::vector<ShapeInstance> shapes;

    const char* GetShapeTypeName(int shapeType) {
        switch (shapeType) {
        case hz::SHAPE_TRIANGLE: return "Triangle";
        case hz::SHAPE_PLANE: return "Plane";
        case hz::SHAPE_CIRCLE: return "Circle";
        case hz::SHAPE_CUBE: return "Cube";
        case hz::SHAPE_PYRAMID: return "Pyramid";
        case hz::SHAPE_PRISM: return "Prism";
        case hz::SHAPE_SPHERE: return "Sphere";
        case hz::SHAPE_CONE: return "Cone";
        case hz::SHAPE_CYLINDER: return "Cylinder";
        default: return "Unknown";
        }
    }

} // namespace hz
