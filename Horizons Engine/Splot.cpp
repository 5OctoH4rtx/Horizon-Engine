#include <windows.h>
#include "Splot.h"
#include <GL/gl.h>
#include "SelColor.h"  // for GetCurrentCycleColor()

namespace hz {

    void DrawColorSplot(int screenW, int screenH) {
        // Backup current matrices
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, screenW, 0, screenH, -1, 1);  // Bottom-left origin

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Disable depth so it doesn't get clipped
        glDisable(GL_DEPTH_TEST);

        // Get color from palette
        ColorRGB c = GetCurrentCycleColor();
        glColor3f(c.r, c.g, c.b);

        // Bottom center square (40×40 px)
        float squareSize = 40;
        float cx = screenW * 0.5f;
        float cy = 40;  // 40 px from bottom

        glBegin(GL_QUADS);
        glVertex2f(cx - squareSize / 2, cy - squareSize / 2);
        glVertex2f(cx + squareSize / 2, cy - squareSize / 2);
        glVertex2f(cx + squareSize / 2, cy + squareSize / 2);
        glVertex2f(cx - squareSize / 2, cy + squareSize / 2);
        glEnd();

        // Restore matrices and state
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

} // namespace hz