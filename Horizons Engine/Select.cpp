// Select.cpp
#include "Select.h"
#include <windows.h>   // for POINT, GetCursorPos, ScreenToClient
#include <GL/glu.h>    // for gluUnProject
#include <algorithm>   // for std::swap
#include <limits>
#include <cmath>
#include "sound.h"

namespace hz {

    int selectedShapeIndex = -1;

    Ray ScreenToRay(int mouseX, int mouseY, int screenWidth, int screenHeight) {
        GLdouble proj[16], model[16];
        GLint viewport[4];
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetIntegerv(GL_VIEWPORT, viewport);

        double x = mouseX;
        double y = viewport[3] - mouseY - 1;

        GLdouble nearX, nearY, nearZ;
        GLdouble farX, farY, farZ;

        gluUnProject(x, y, 0.0, model, proj, viewport, &nearX, &nearY, &nearZ);
        gluUnProject(x, y, 1.0, model, proj, viewport, &farX, &farY, &farZ);

        Ray ray;
        ray.ox = static_cast<float>(nearX);
        ray.oy = static_cast<float>(nearY);
        ray.oz = static_cast<float>(nearZ);

        ray.dx = static_cast<float>(farX - nearX);
        ray.dy = static_cast<float>(farY - nearY);
        ray.dz = static_cast<float>(farZ - nearZ);

        float length = std::sqrt(ray.dx * ray.dx + ray.dy * ray.dy + ray.dz * ray.dz);
        if (length > 0.f) {
            ray.dx /= length;
            ray.dy /= length;
            ray.dz /= length;
        }

        return ray;
    }

    bool RayIntersectsAABB(const Ray& ray, const AABB& box, float& tHit) {
        float tMin = -std::numeric_limits<float>::infinity();
        float tMax = std::numeric_limits<float>::infinity();

        const float origin[3] = { ray.ox, ray.oy, ray.oz };
        const float dir[3] = { ray.dx, ray.dy, ray.dz };
        const float bmin[3] = { box.minX, box.minY, box.minZ };
        const float bmax[3] = { box.maxX, box.maxY, box.maxZ };

        for (int i = 0; i < 3; ++i) {
            if (std::fabs(dir[i]) < 1e-6f) {
                if (origin[i] < bmin[i] || origin[i] > bmax[i])
                    return false;
            }
            else {
                float ood = 1.0f / dir[i];
                float t1 = (bmin[i] - origin[i]) * ood;
                float t2 = (bmax[i] - origin[i]) * ood;
                if (t1 > t2) std::swap(t1, t2);
                tMin = (t1 > tMin) ? t1 : tMin;
                tMax = (t2 < tMax) ? t2 : tMax;
                if (tMin > tMax)
                    return false;
            }
        }

        tHit = (tMin >= 0.f) ? tMin : tMax;
        return true;
    }

    void TrySelectShape(const Ray& ray) {
        float closestT = std::numeric_limits<float>::infinity();
        int closestIndex = -1;

        for (size_t i = 0; i < shapes.size(); ++i) {
            AABB box = CalcAABB(shapes[i]);
            float tHit = 0.f;
            if (RayIntersectsAABB(ray, box, tHit)) {
                if (tHit < closestT) {
                    closestT = tHit;
                    closestIndex = static_cast<int>(i);

                    const char* soundPath = "C:\\AudAsset\\drop_003.wav";
                    hz::PlaySoundFile(soundPath);
                }
            }
        }

        selectedShapeIndex = closestIndex;
    }

    void DrawSelectionOutline() {
        if (selectedShapeIndex < 0 || selectedShapeIndex >= (int)shapes.size())
            return;

        glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT | GL_CURRENT_BIT);

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_POLYGON_OFFSET_LINE);  // NEW
        glPolygonOffset(-1.0f, -1.0f);     // NEW
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(3.0f);
        glColor3f(1, 1, 1);

        shapes[selectedShapeIndex].Draw(1.0f, 1.0f, 1.0f); // pass white override

        glPopAttrib();
    }

}