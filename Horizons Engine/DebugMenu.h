#pragma once
#include <cstddef>     // for size_t

namespace hz {

    // Call once after the GL context is ready.
    void InitDebugMenu();

    // Stats getters
    float  GetCurrentFPS();        // smoothed FPS
    size_t GetMemoryUsageMB();     // process working-set in MB
    float  GetCPUUsagePercent();   // light CPU % estimate

    // Draws the whole debug overlay in white, top-left.
    void DisplayDebugMenu(int screenW, int screenH);

    void UpdateDebugStatsEveryFrame();
}