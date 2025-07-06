// Frame.cpp
#include <windows.h>

static LARGE_INTEGER frequency;
static LARGE_INTEGER lastTime;
static float deltaTime = 0.016f; // Default to ~60 FPS

void InitFrame() {
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);
}

void LimitFrame(float targetFPS = 60.0f) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    double elapsed = double(now.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
    double targetFrameTime = 1.0 / targetFPS;

    while (elapsed < targetFrameTime) {
        QueryPerformanceCounter(&now);
        elapsed = double(now.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
    }

    deltaTime = float(elapsed); // seconds
    lastTime = now;
}

float GetDeltaTime() {
    return deltaTime;
}