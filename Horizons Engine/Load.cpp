#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Load.h"
#include "RenText.h"

int loadWidth = GetSystemMetrics(SM_CXSCREEN);
int loadHeight = GetSystemMetrics(SM_CYSCREEN);

namespace hz {

    void ShowLoadingScreen() {
        const int barWidth = 600;
        const int barHeight = 30;
        const int barX = (loadWidth - barWidth) / 2;
        const int barY = loadHeight / 2 + 40;

        for (int i = 0; i <= 100; i += 10) {
            // Clear background
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set up orthographic projection
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(0, loadWidth, loadHeight, 0);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();


            // Render "Loading..." in white at screen center
            hz::RenderTextOnScreen("...Loading...", 920, 500, 1.0f, 1.0f, 1.0f, true);

            // Draw percentage text
            char percentText[16];
            snprintf(percentText, sizeof(percentText), "%d%%", i);
            hz::RenderTextOnScreen(percentText, loadWidth / 2, barY - 30, 1.0f, 1.0f, 1.0f, true);

            // Draw loading bar background (gray)
            glColor3f(0.1f, 0.1f, 0.1f);
            glBegin(GL_QUADS);
            glVertex2i(barX,     barY);
            glVertex2i(barX + barWidth, barY);
            glVertex2i(barX + barWidth, barY + barHeight);
            glVertex2i(barX,     barY + barHeight);
            glEnd();

            // Draw loading bar filled portion (white)
            int filledWidth = (barWidth * i) / 100;
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glVertex2i(barX,             barY);
            glVertex2i(barX + filledWidth, barY);
            glVertex2i(barX + filledWidth, barY + barHeight);
            glVertex2i(barX,             barY + barHeight);
            glEnd();

            // Restore matrices
            glPopMatrix();
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);

            // Present the frame
            glFlush();
            SwapBuffers(wglGetCurrentDC());

            Sleep(100); // simulate load delay
        }
    }

}