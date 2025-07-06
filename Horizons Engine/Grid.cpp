#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <GL/glu.h>
#include "Grid.h"

namespace hz {

    static GLuint gridListId = 0;   // 0 == not created

    //------------------------------------------------------------------
    // Build a display-list version (faster for large static grid)
    void InitGridDisplayList(int halfSize, float cell)
    {
        if (gridListId != 0) return;                 // already built
        gridListId = glGenLists(1);
        glNewList(gridListId, GL_COMPILE);

        glDisable(GL_LIGHTING);
        glColor3f(0.3f, 0.3f, 0.3f);                 // grid line colour
        glBegin(GL_LINES);
        for (int i = -halfSize; i <= halfSize; ++i) {
            float x = i * cell;
            // lines parallel Z (constant X)
            glVertex3f(x, 0.0f, -halfSize * cell);
            glVertex3f(x, 0.0f, halfSize * cell);
            // lines parallel X (constant Z)
            float z = i * cell;
            glVertex3f(-halfSize * cell, 0.0f, z);
            glVertex3f(halfSize * cell, 0.0f, z);
        }
        glEnd();

        glEndList();
    }

    //------------------------------------------------------------------
    void DrawGrid(int halfSize, float cell)
    {
        glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT);
        glLineWidth(1.0f);
        glColor3f(0.3f, 0.3f, 0.3f);

        if (gridListId)
            glCallList(gridListId);
        else {
            // immediate-mode fallback
            glBegin(GL_LINES);
            for (int i = -halfSize; i <= halfSize; ++i) {
                float x = i * cell;
                glVertex3f(x, 0.0f, -halfSize * cell);
                glVertex3f(x, 0.0f, halfSize * cell);
                float z = i * cell;
                glVertex3f(-halfSize * cell, 0.0f, z);
                glVertex3f(halfSize * cell, 0.0f, z);
            }
            glEnd();
        }

        glPopAttrib();
    }

} // namespace hz
