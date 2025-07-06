// main.cpp
//Windows non-project requirements
#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <limits>
#include <gl/GL.h>
#include <gl/GLU.h>

//just in case
#include "miniaudio.h"

//stop being mad at me
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

//in project includes and externs for main
#include "MainUtils.h"

using namespace hz;

    // Get the primary monitor size
int windowWidth = GetSystemMetrics(SM_CXSCREEN);
int windowHeight = GetSystemMetrics(SM_CYSCREEN);

float eyeX = 0.0f, eyeY = 0.0f, eyeZ = 5.0f;
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

float aspectRatio = (float)windowWidth / (float)windowHeight;

float camSpeed = 0.1f;

float cameraYaw = -90.0f;    // Horizontal angle, degrees
float cameraPitch = 0.0f;  // Vertical angle, degrees

int MouseUsePosX = 0; //Usable Mouse Position Global Variable X
int MouseUsePosY = 0; //Usable Mouse Position Global Variable Y

bool wasNPressed = false;
bool wasGPressed = false;
bool lmbWasDown = false;    // tracks previous-frame state
bool wasKPressed = false;

static bool wasBPressed = false;
static int bgColorIndex = 0;

// Define your 6 colors (r,g,b)
static const float bgColors[6][3] = {
    {0.1f, 0.1f, 0.1f},  // dark grey (default)
    {0.1f, 0.1f, 0.3f},  // dark blue
    {0.0f, 0.5f, 0.0f},  // green
    {0.5f, 0.0f, 0.0f},  // dark red
    {0.5f, 0.5f, 0.0f},  // olive
    {0.3f, 0.3f, 0.3f}   // grey
};

bool isVisible = true;

bool showGrid = true;

bool showDebugText = false;

bool showTestTexture = false;
float testTexPosX = 0.0f;
float testTexPosY = 0.0f;
float testTexPosZ = 0.0f;

static bool gridWasVisible = false;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void SetupPixelFormat(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hdc, &pfd);
    if (pf == 0) {
        MessageBox(NULL, "ChoosePixelFormat failed.", "Error", MB_OK);
        exit(1);
    }
    if (!SetPixelFormat(hdc, pf, &pfd)) {
        MessageBox(NULL, "SetPixelFormat failed.", "Error", MB_OK);
        exit(1);
    }
}

void InitScene() {
    hz::InitCircle(); // if you're using circles too
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Add initial cube in front of camera at (0,0,-5)
    hz::AddShape(hz::SHAPE_CUBE, 0.0f, 0.0f, -5.0f, 0, 0, 0, 1.0f, 1, 0, 0); // Red cube
}

void UpdateMouseLook(HWND hwnd) {
    // Get window center point in screen coords
    RECT rect;
    GetClientRect(hwnd, &rect);
    POINT center = { (rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2 };
    ClientToScreen(hwnd, &center);

    // Get current cursor position
    POINT mousePos;
    GetCursorPos(&mousePos);

     MouseUsePosX = mousePos.x;
     MouseUsePosY = mousePos.y;

    // Calculate delta movement
    int dx = mousePos.x - center.x;
    int dy = mousePos.y - center.y;

    const float sensitivity = 0.15f; // Adjust sensitivity here

    cameraYaw += dx * sensitivity;
    cameraPitch -= dy * sensitivity;

    // Clamp pitch to avoid flipping camera
    if (cameraPitch > 89.9f) cameraPitch = 89.9f;
    if (cameraPitch < -89.9f) cameraPitch = -89.9f;

    // Reset cursor to center
    SetCursorPos(center.x, center.y);
}

void UpdateCameraPosition() {
    float forwardX = centerX - eyeX;
    float forwardY = centerY - eyeY;
    float forwardZ = centerZ - eyeZ;
    float length = std::sqrt(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    if (length == 0.0f) return;

    // Convert degrees to radians
    float yawRad = cameraYaw * 3.14159265f / 180.0f;
    float pitchRad = cameraPitch * 3.14159265f / 180.0f;

    // Calculate direction vector
    float dirX = cosf(pitchRad) * cosf(yawRad);
    float dirY = sinf(pitchRad);
    float dirZ = cosf(pitchRad) * sinf(yawRad);

    // Camera position stays eyeX, eyeY, eyeZ, but center is eye + direction
    centerX = eyeX + dirX;
    centerY = eyeY + dirY;
    centerZ = eyeZ + dirZ;

    // Normalize forward vector
    forwardX /= length;
    forwardY /= length;
    forwardZ /= length;

    // Right vector = forward cross up (Y up)
    float rightX = forwardZ;
    float rightY = 0.0f;
    float rightZ = -forwardX;

    // Move with WASD
    if (GetAsyncKeyState('W') & 0x8000) {
        eyeX += forwardX * camSpeed;
        eyeY += forwardY * camSpeed;
        eyeZ += forwardZ * camSpeed;
        centerX += forwardX * camSpeed;
        centerY += forwardY * camSpeed;
        centerZ += forwardZ * camSpeed;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        eyeX -= forwardX * camSpeed;
        eyeY -= forwardY * camSpeed;
        eyeZ -= forwardZ * camSpeed;
        centerX -= forwardX * camSpeed;
        centerY -= forwardY * camSpeed;
        centerZ -= forwardZ * camSpeed;
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        eyeX += rightX * camSpeed;
        eyeZ += rightZ * camSpeed;
        centerX += rightX * camSpeed;
        centerZ += rightZ * camSpeed;
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        eyeX -= rightX * camSpeed;
        eyeZ -= rightZ * camSpeed;
        centerX -= rightX * camSpeed;
        centerZ -= rightZ * camSpeed;
    }
    // Up/down
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        eyeY += camSpeed;
        centerY += camSpeed;
    }
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
        eyeY -= camSpeed;
        centerY -= camSpeed;
    }
}

void DrawAllShapes() {
    for (const auto& shape : hz::shapes)
        if (shape.visible)
            shape.Draw();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    const char* CLASS_NAME = "HorizonsEngineClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_OWNDC; // Important for OpenGL

    RegisterClass(&wc);

    // Create fullscreen window
    HWND hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        CLASS_NAME,
        "Horizons Engine",
        WS_POPUP | WS_VISIBLE,
        0, 0, windowWidth, windowHeight,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, "Failed to create window.", "Error", MB_OK);
        return -1;
    }

    // Get device context and set pixel format
    HDC hdc = GetDC(hwnd);
    SetupPixelFormat(hdc);

    // Create and enable OpenGL rendering context
    HGLRC hglrc = wglCreateContext(hdc);
    if (!hglrc) {
        MessageBox(NULL, "Failed to create OpenGL context.", "Error", MB_OK);
        return -1;
    }
    if (!wglMakeCurrent(hdc, hglrc)) {
        MessageBox(NULL, "Failed to activate OpenGL context.", "Error", MB_OK);
        return -1;
    }

    if (!hz::InitSound()) {
        MessageBoxA(NULL, "Failed to initialize MiniAudio.", "Sound Error", MB_OK);
    }

    // ENABLE VSYNC HERE
    typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT) {
        wglSwapIntervalEXT(1);
    }
    else {
        MessageBox(NULL, "VSync not supported.", "Warning", MB_OK);
    }

    if (!hz::InitFontBitmaps()) {
        MessageBoxA(nullptr, "Font init failed!", "Error", MB_OK);
    }

    // Show the loading screen before starting heavy work
    hz::ShowLoadingScreen();

    ShowCursor(FALSE);
    SetCapture(hwnd);

    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    POINT centerPos = { windowWidth / 2, windowHeight / 2 };
    ClientToScreen(hwnd, &centerPos);  // Convert client coords to screen coords for SetCursorPos
    SetCursorPos(centerPos.x, centerPos.y);

    InitScene();
    InitCrosshair();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  // Default depth test function

    // after you have a valid GL context (once)
    hz::InitDebugMenu();

    InitFrame();  // Initialize frame timing ONCE before the loop

    // Main loop
    MSG msg = { };
    bool running = true;
    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // ---------------------------------------------------------------------------
        //  Input:  detect a *new* left-click (button went up→down this frame)
        // ---------------------------------------------------------------------------
        bool lmbDownNow = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        if (lmbDownNow && !lmbWasDown)            // edge: just clicked
        {
            // Get mouse position in client coords
            POINT p; GetCursorPos(&p); ScreenToClient(hwnd, &p);

            //  Convert to picking ray
            hz::Ray ray = hz::ScreenToRay(p.x, p.y, windowWidth, windowHeight);

            // Try to select the closest shape; sets hz::selectedShapeIndex
            hz::TrySelectShape(ray);
        }
        lmbWasDown = lmbDownNow;     // keep for next frame

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Setup projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, aspectRatio, 0.1, 100.0);

        // Setup camera view
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);


        bool isGPressed = (GetAsyncKeyState('G') & 0x8000) != 0;
        if (isGPressed && !wasGPressed) {
            showGrid = !showGrid;  // Toggle visibility
        }
        wasGPressed = isGPressed;

        static bool wasKPressed = false;
        bool isKPressed = (GetAsyncKeyState('K') & 0x8000) != 0;

        if (isKPressed && !wasKPressed)
        {
            if (!showTestTexture) {
                // Compute forward vector
                float fx = centerX - eyeX;
                float fy = centerY - eyeY;
                float fz = centerZ - eyeZ;
                hz::Normalize(fx, fy, fz);

                // Store position 5 units ahead of camera
                testTexPosX = eyeX + fx * 5.0f;
                testTexPosY = eyeY + fy * 5.0f;
                testTexPosZ = eyeZ + fz * 5.0f;

                showTestTexture = true;
            }

        }
        wasKPressed = isKPressed;

        bool isNPressed = (GetAsyncKeyState('N') & 0x8000) != 0;

        if (isNPressed && !wasNPressed)
        {
            /*----------- 1. forward vector from yaw / pitch ------------*/
            const float yawRad = cameraYaw * 3.14159265f / 180.0f;
            const float pitchRad = cameraPitch * 3.14159265f / 180.0f;

            float fx = cosf(pitchRad) * cosf(yawRad);
            float fy = sinf(pitchRad);
            float fz = cosf(pitchRad) * sinf(yawRad);

            float len = std::sqrt(fx * fx + fy * fy + fz * fz);
            if (len) { fx /= len; fy /= len; fz /= len; }

            const float spawnX = eyeX + fx * 5.0f;
            const float spawnY = eyeY + fy * 5.0f;
            const float spawnZ = eyeZ + fz * 5.0f;

            hz::ColorRGB col = hz::GetCurrentCycleColor();  // get the current color in the cycle

                const char* soundPath = "C:\\AudAsset\\drop_003.wav";
                hz::PlaySoundFile(soundPath);

            /*----------- 3. spawn cube --------------------------------*/
            hz::AddShape(hz::SHAPE_CUBE,
                spawnX, spawnY, spawnZ,
                0.0f, 0.0f, 0.0f,   // rotation
                1.0f,               // scale
                col.r, col.g, col.b);
        }

        wasNPressed = isNPressed; // update for next frame

        static bool wasBackspacePressed = false;
        bool isBackspacePressed = (GetAsyncKeyState(VK_BACK) & 0x8000) != 0;

        if (isBackspacePressed && !wasBackspacePressed)
        {
            hz::DeleteSelectedShape();  // Call the function, don't declare it here
        }

        wasBackspacePressed = isBackspacePressed;

        static bool wasCPressed = false;
        bool isCPressed = (GetAsyncKeyState('C') & 0x8000) != 0;

        if (isCPressed && !wasCPressed)
        {
            hz::CloneSelectedShape();  // Call the function, don't declare it here
        }

        wasCPressed = isCPressed;

        static bool wasTildePressed = false;
        bool isTildePressed = (GetAsyncKeyState(VK_OEM_3) & 0x8000) != 0;  // VK_OEM_3 = tilde key

        if (isTildePressed && !wasTildePressed) {
            showDebugText = !showDebugText;
            const char* soundPath = "C:\\AudAsset\\bong_001.wav";
            hz::PlaySoundFile(soundPath);
        }

        wasTildePressed = isTildePressed;

        // Update camera position using delta time
        UpdateMouseLook(hwnd);
        UpdateCameraPosition();

        if (showGrid != gridWasVisible) {  // grid visibility changed
            const char* soundPath = "C:\\AudAsset\\bong_001.wav";
            hz::PlaySoundFile(soundPath);
        }
        gridWasVisible = showGrid;  // update for next frame

        if (showGrid) {
            glPushMatrix();
            glTranslatef(0.0f, -1.0f, 0.0f);
            hz::DrawGrid();
            glPopMatrix();
        }

        // Draw all shapes
        hz::UpdateSelectedMovement();
        hz::UpdateSelectedShapeType();
        hz::UpdateSelectedRotation();
        hz::UpdateSelectedScale();

        hz::UpdateSelectedColorCycle();

        DrawAllShapes();
        hz::DrawSelectionOutline();  // outline (only if something is selected)

        hz::DrawColorSplot(windowWidth, windowHeight);

        DrawCrosshair(windowWidth, windowHeight);\


            if (showDebugText) {

                // Existing debug lines
                hz::DisplayDebugMenu(windowWidth, windowHeight);

                // ---- Selected-shape readout ----
                hz::ShapeStats st;
                if (hz::GetShapeStats(hz::selectedShapeIndex, st)) {
                    char buf[128];

                    hz::RenderTextOnScreen("Selected Shape:", 20, 150, 1, 1, 1, true);

                    snprintf(buf, sizeof(buf), "Type : %s", st.type.c_str());
                    hz::RenderTextOnScreen(buf, 25, 170, 1, 1, 1, false);

                    snprintf(buf, sizeof(buf), "Pos  : %.2f  %.2f  %.2f", st.x, st.y, st.z);
                    hz::RenderTextOnScreen(buf, 25, 190, 1, 1, 1, false);

                    snprintf(buf, sizeof(buf), "Rot  : %.1f  %.1f  %.1f", st.rotX, st.rotY, st.rotZ);
                    hz::RenderTextOnScreen(buf, 25, 210, 1, 1, 1, false);

                    snprintf(buf, sizeof(buf), "Scale: %.2f", st.scale);
                    hz::RenderTextOnScreen(buf, 25, 230, 1, 1, 1, false);

                    snprintf(buf, sizeof(buf), "Color: %.2f  %.2f  %.2f", st.r, st.g, st.b);
                    hz::RenderTextOnScreen(buf, 25, 250, st.r, st.g, st.b, false);
                }
                else {

                    hz::RenderTextOnScreen("Selected Shape:", 20, 150, 1, 1, 1, true);

                    hz::RenderTextOnScreen("Type  : Unknown", 25, 170, 1, 1, 1, false);

                    hz::RenderTextOnScreen("Pos  : Unknown", 25, 190, 1, 1, 1, false);

                    hz::RenderTextOnScreen("Rot  : Unknown", 25, 210, 1, 1, 1, false);

                    hz::RenderTextOnScreen("Scale  : Unknown", 25, 230, 1, 1, 1, false);

                    hz::RenderTextOnScreen("Color  : Unknown", 25, 250, 1, 1, 1, false);
                }

                hz::CameraStats cam;
                if (hz::GetCameraStats(cam)) {
                    char buf[128];

                    hz::RenderTextOnScreen("Camera Stats:", 20, 290, 1, 1, 1, true);

                    snprintf(buf, sizeof(buf), "Cam Pos : %.2f %.2f %.2f", cam.eyeX, cam.eyeY, cam.eyeZ);
                    hz::RenderTextOnScreen(buf, 25, 310, 1, 1, 1, false);

                    snprintf(buf, sizeof(buf), "Yaw/Pitch: %.1f / %.1f", cam.yawDeg, cam.pitchDeg);
                    hz::RenderTextOnScreen(buf, 25, 330, 1, 1, 1, false);

                    snprintf(buf, sizeof(buf), "Dir Vec : %.2f %.2f %.2f", cam.dirX, cam.dirY, cam.dirZ);
                    hz::RenderTextOnScreen(buf, 25, 350, 1, 1, 1, false);

                    const char* facing = hz::FindCamComp();
                    snprintf(buf, sizeof(buf), "Facing: %s", facing);
                    hz::RenderTextOnScreen(buf, 25, 370, 1, 1, 1, false);
                }
            }

        if (showTestTexture) {
            hz::DrawTestTexture();
        }

        // Swap buffers first to avoid stalls
        SwapBuffers(hdc);

        // Frame limiting after rendering to maintain steady FPS
        LimitFrame(60.0f);

        hz::UpdateDebugStatsEveryFrame();   // MUST be called every frame
    }

    // Cleanup
    hz::ShutdownSound();
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
