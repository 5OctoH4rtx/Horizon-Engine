#include <windows.h>
#include <GL/gl.h>
#include "Cross.h"

// Call this once during setup
void InitCrosshair() {
    glLineWidth(2.0f);
}

// Call this every frame after setting up the camera/projection
void DrawCrosshair(int screenWidth, int screenHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);  // Top-left origin

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);  // White

    int cx = screenWidth / 2;
    int cy = screenHeight / 2;

    glBegin(GL_LINES);
    glVertex2i(cx - 8, cy); glVertex2i(cx + 8, cy); // Horizontal line
    glVertex2i(cx, cy - 8); glVertex2i(cx, cy + 8); // Vertical line
    glEnd();

    glEnable(GL_DEPTH_TEST);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
