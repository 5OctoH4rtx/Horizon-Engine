#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include <string>
#include <sstream>

#include "ShapeX.h"
#include "DebugMenu.h"
#include "RenText.h"     // RenderTextOnScreen

namespace hz {

    double GetTimeSeconds() {
        static LARGE_INTEGER frequency;
        static BOOL initialized = QueryPerformanceFrequency(&frequency);

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);

        return (double)now.QuadPart / frequency.QuadPart;
    }

    // ------------------------------------------------------------------
    //  FPS
    static LARGE_INTEGER freq, frameStart;
    static float smoothedFPS = 0.f;

    void InitDebugMenu() {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&frameStart);
    }

    static void sampleFPS()
    {
        static double lastTime = GetTimeSeconds();  // last update timestamp
        static double prevFrameTime = GetTimeSeconds();  // for delta calculation
        static float lastFPS = 0.0f;

        double now = GetTimeSeconds();
        double dt = now - prevFrameTime;
        prevFrameTime = now;

        float curFPS = (dt > 0.0) ? float(1.0 / dt) : 0.0f;

        if (now - lastTime >= 0.5) {
            smoothedFPS = curFPS;  // or average multiple frames here if needed
            lastTime = now;
        }
    }

    float GetCurrentFPS() { return smoothedFPS; }

    // ------------------------------------------------------------------
    //  Memory (MB)
    size_t GetMemoryUsageMB() {
        PROCESS_MEMORY_COUNTERS pmc{};
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
            return pmc.WorkingSetSize / (1024 * 1024);
        return 0;
    }

    // ------------------------------------------------------------------
    //  Simple CPU usage % (process time over 1-frame interval)
    static ULONGLONG prevSys = 0, prevProc = 0;
    static float cpuPercent = 0.f;

    static ULONGLONG fileTimeToULL(const FILETIME& ft) {
        return ((ULONGLONG)ft.dwHighDateTime << 32ULL) | ft.dwLowDateTime;
    }

    static void sampleCPU() {
        FILETIME idle, kernel, user;
        GetSystemTimes(&idle, &kernel, &user);
        FILETIME c, e, k, u;
        GetProcessTimes(GetCurrentProcess(), &c, &e, &k, &u);

        ULONGLONG sys = fileTimeToULL(kernel) + fileTimeToULL(user);
        ULONGLONG proc = fileTimeToULL(k) + fileTimeToULL(u);

        if (prevSys) {
            ULONGLONG sysDiff = sys - prevSys;
            ULONGLONG procDiff = proc - prevProc;
            cpuPercent = sysDiff ? (100.f * procDiff / float(sysDiff)) : 0.f;
        }
        prevSys = sys;
        prevProc = proc;
    }

    float GetCPUUsagePercent() { return cpuPercent; }

    // Call once per frame (cheap) even if you don’t render the overlay
    void UpdateDebugStatsEveryFrame() {
        sampleFPS();

        // --- Update CPU usage only once per 0.5 seconds ---
        static double lastCPUTime = 0.0;
        double now = GetTimeSeconds();  // your existing time function
        if (now - lastCPUTime >= 0.5) {
            sampleCPU();  // update only every 0.5 sec
            lastCPUTime = now;
        }
    }

    // ------------------------------------------------------------------
    //  Composite render
    void DisplayDebugMenu(int screenW, int /*screenH*/) {

        std::ostringstream oss;
        oss.setf(std::ios::fixed); oss.precision(1);

        // -- line 1 : header
        RenderTextOnScreen("Horizons Engine Debug Mode", 10, 30, 1, 1, 1, true);

        // -- line 2 : FPS
        oss.str(""); oss << "FPS: " << GetCurrentFPS();
        RenderTextOnScreen(oss.str(), 20, 70, 1, 0.55, 0, false);

        // -- line 3 : Memory
        oss.str(""); oss << "Memory: " << GetMemoryUsageMB() << " MB";
        RenderTextOnScreen(oss.str(), 20, 90, 0, 0, 1, false);

        // -- line 4 : CPU
        oss.str(""); oss << "CPU: " << GetCPUUsagePercent() << " %";
        RenderTextOnScreen(oss.str(), 20, 110, 0, 1, 0, false);
    }

} // namespace hz