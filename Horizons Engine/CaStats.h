// CaStats.h
#pragma once

namespace hz {
    struct CameraStats {
        float eyeX, eyeY, eyeZ;   // camera position
        float dirX, dirY, dirZ;   // forward / look direction (normalized)
        float yawDeg;             // yaw  in degrees
        float pitchDeg;           // pitch in degrees
    };

    // Writes current camera state into outStats and returns true on success.
    bool GetCameraStats(CameraStats& outStats);

    // Returns "N", "NE", "E", "SE", "S", "SW", "W", "NW"
    const char* FindCamComp();   // compass from cameraYaw
}