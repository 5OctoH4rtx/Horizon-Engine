// CaStats.cpp
#define WIN32_LEAN_AND_MEAN
#include "CaStats.h"
#include <cmath>

// these globals live in main.cpp; declare them extern so we can read them
extern float eyeX, eyeY, eyeZ;
extern float centerX, centerY, centerZ;
extern float cameraYaw;   // degrees
extern float cameraPitch; // degrees

namespace hz {

    bool GetCameraStats(CameraStats& out)
    {
        // sanity: if eye==center the direction can't be built; still fill basics.
        out.eyeX = eyeX; out.eyeY = eyeY; out.eyeZ = eyeZ;
        out.yawDeg = cameraYaw;
        out.pitchDeg = cameraPitch;

        float dx = centerX - eyeX;
        float dy = centerY - eyeY;
        float dz = centerZ - eyeZ;
        float len = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (len > 0.0001f) {
            out.dirX = dx / len;
            out.dirY = dy / len;
            out.dirZ = dz / len;
        }
        else {
            // fallback: compute forward from yaw/pitch directly
            float yawRad = cameraYaw * 3.14159265f / 180.0f;
            float pitchRad = cameraPitch * 3.14159265f / 180.0f;
            out.dirX = cosf(pitchRad) * cosf(yawRad);
            out.dirY = sinf(pitchRad);
            out.dirZ = cosf(pitchRad) * sinf(yawRad);
        }
        return true;
    }

    const char* FindCamComp()
    {
        // normalise yaw to 0..360 (0° = +X = East)
        float yaw = fmodf(cameraYaw, 360.0f);
        if (yaw < 0) yaw += 360.0f;

        // Map to 8 compass sectors (22.5° each half-sector)
        static const char* dirs[8] = { "East", "South East", "South", "South West", "West", "North West", "North", "North East" };
        int idx = int((yaw + 22.5f) / 45.0f) & 7;  // round to nearest sector
        return dirs[idx];
    }

} // namespace hz
