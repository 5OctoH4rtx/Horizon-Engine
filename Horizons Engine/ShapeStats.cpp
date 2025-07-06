#define WIN32_LEAN_AND_MEAN
#include "ShapeStats.h"
#include "ShapeX.h"          // shapes vector + ShapeInstance

namespace hz {

    static const char* TypeName(ShapeType t)
    {
        switch (t) {
        case SHAPE_TRIANGLE:  return "Triangle";
        case SHAPE_PLANE:     return "Plane";
        case SHAPE_CIRCLE:    return "Circle";
        case SHAPE_CUBE:      return "Cube";
        case SHAPE_PYRAMID:   return "Pyramid";
        case SHAPE_PRISM:     return "Prism";
        case SHAPE_SPHERE:    return "Sphere";
        case SHAPE_CONE:      return "Cone";
        case SHAPE_CYLINDER:  return "Cylinder";
        default:              return "Unknown";
        }
    }

    bool GetShapeStats(int index, ShapeStats& s)
    {
        if (index < 0 || index >= static_cast<int>(shapes.size())) {
            s.valid = false;
            return false;
        }

        const ShapeInstance& sh = shapes[index];
        s.valid = true;
        s.type = TypeName(sh.type);

        s.x = sh.x; s.y = sh.y; s.z = sh.z;
        s.rotX = sh.rotX; s.rotY = sh.rotY; s.rotZ = sh.rotZ;
        s.scale = sh.scale;
        s.r = sh.r; s.g = sh.g; s.b = sh.b;
        return true;
    }

} // namespace hz