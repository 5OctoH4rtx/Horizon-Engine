// RenText.cpp
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include "RenText.h"

namespace hz {

    // Two display‑list bases: regular & bold
    static GLuint fontBase = 0;
    static GLuint boldBase = 0;

    bool InitFontBitmaps()
    {
        HDC hdc = wglGetCurrentDC();
        if (!hdc) return false;

        // Build regular glyphs (ASCII 32‑127)
        HFONT hFont = CreateFontA(18, 0, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH, "Consolas");
        if (!hFont) return false;
        SelectObject(hdc, hFont);

        fontBase = glGenLists(96);
        if (!wglUseFontBitmapsA(hdc, 32, 96, fontBase)) {
            DeleteObject(hFont);
            return false;
        }

        // Build bold glyphs
        HFONT hBold = CreateFontA(18, 0, 0, 0, FW_BOLD,
            FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH, "Consolas");
        SelectObject(hdc, hBold);
        boldBase = glGenLists(96);
        wglUseFontBitmapsA(hdc, 32, 96, boldBase);

        DeleteObject(hFont);
        DeleteObject(hBold);
        return true;
    }

    void RenderTextOnScreen(const std::string& msg,
        int x, int y,
        float r, float g, float b,
        bool bold)
    {
        if (!fontBase || !boldBase) return; // InitFontBitmaps() not called

        // Save matrices and state
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        glOrtho(0, vp[2], vp[3], 0, -1, 1); // (0,0) top‑left

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        glColor3f(r, g, b);
        glRasterPos2i(x, y);

        GLuint base = bold ? boldBase : fontBase;
        glListBase(base - 32);
        glCallLists((GLsizei)msg.length(), GL_UNSIGNED_BYTE, msg.c_str());

        // Restore
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

} // namespace hz