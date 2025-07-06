// Shape.cpp
#include <windows.h>  // Needed before gl.h on Windows
#include <GL/gl.h>
#include <cmath>

namespace hz {

    // ---------- TRIANGLE ----------
    static const float triangleVerts[] = {
        0.0f,  0.5f, 0.0f,  // top
       -0.5f, -0.5f, 0.0f,  // bottom left
        0.5f, -0.5f, 0.0f   // bottom right
    };

    static const GLuint triangleIndices[] = { 0, 1, 2 };

    void DrawTriangle() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_CULL_FACE);   // Disable culling for this shape
        glVertexPointer(3, GL_FLOAT, 0, triangleVerts);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleIndices);
        glEnable(GL_CULL_FACE);    // Re-enable for everything else
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // ---------- PLANE (square) ----------
    static const float planeVerts[] = {
       -0.5f, 0.0f, -0.5f, // 0
        0.5f, 0.0f, -0.5f, // 1
        0.5f, 0.0f,  0.5f, // 2
       -0.5f, 0.0f,  0.5f  // 3
    };

    static const GLuint planeIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    void DrawPlane() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_CULL_FACE);   // Disable culling for this shape
        glVertexPointer(3, GL_FLOAT, 0, planeVerts);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, planeIndices);
        glEnable(GL_CULL_FACE);    // Re-enable for everything else
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // ---------- CIRCLE ----------
    static const int circleSegments = 32;
    static float circleVerts[(circleSegments + 2) * 3]; // +2 for center + wrap-around

    void InitCircle() {
        circleVerts[0] = 0.0f;
        circleVerts[1] = 0.0f;
        circleVerts[2] = 0.0f;

        for (int i = 0; i <= circleSegments; ++i) {
            // NEGATIVE angle to reverse winding (CCW from +Y view)
            float angle = -2.0f * 3.14159265359f * i / circleSegments;
            circleVerts[(i + 1) * 3 + 0] = cosf(angle) * 0.5f;
            circleVerts[(i + 1) * 3 + 1] = 0.0f;
            circleVerts[(i + 1) * 3 + 2] = sinf(angle) * 0.5f;
        }

        // repeat first perimeter vertex at end
        circleVerts[(circleSegments + 1) * 3 + 0] = circleVerts[3];
        circleVerts[(circleSegments + 1) * 3 + 1] = circleVerts[4];
        circleVerts[(circleSegments + 1) * 3 + 2] = circleVerts[5];
    }

    void DrawCircle() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_CULL_FACE);   // Disable culling for this shape
        glVertexPointer(3, GL_FLOAT, 0, circleVerts);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleSegments + 2);
        glEnable(GL_CULL_FACE);    // Re-enable for everything else
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // ---------- CUBE ----------
    static const float cubeVerts[] = {
       -0.5f, -0.5f, -0.5f,  // 0
        0.5f, -0.5f, -0.5f,  // 1
        0.5f,  0.5f, -0.5f,  // 2
       -0.5f,  0.5f, -0.5f,  // 3
       -0.5f, -0.5f,  0.5f,  // 4
        0.5f, -0.5f,  0.5f,  // 5
        0.5f,  0.5f,  0.5f,  // 6
       -0.5f,  0.5f,  0.5f   // 7
    };

    static const GLuint cubeIndices[] = {
        4, 5, 6, 4, 6, 7, // front
        0, 3, 2, 0, 2, 1, // back
        0, 4, 7, 0, 7, 3, // left
        1, 2, 6, 1, 6, 5, // right
        3, 7, 6, 3, 6, 2, // top
        0, 1, 5, 0, 5, 4  // bottom
    };

    void DrawCube() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, cubeVerts);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, cubeIndices);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // ---------- PYRAMID (square base, CCW winding) ----------
    static const float pyramidVerts[] = {
       -0.5f, 0.0f, -0.5f,  // 0: base back-left
        0.5f, 0.0f, -0.5f,  // 1: base back-right
        0.5f, 0.0f,  0.5f,  // 2: base front-right
       -0.5f, 0.0f,  0.5f,  // 3: base front-left
        0.0f, 1.0f,  0.0f   // 4: apex
    };

    static const GLuint pyramidIndices[] = {
        // base (CCW from above)
        0, 2, 1,
        0, 3, 2,

        // sides (all CCW when viewed from outside)
        0, 1, 4,  // back face
        1, 2, 4,  // right face
        2, 3, 4,  // front face
        3, 0, 4   // left face
    };

    void DrawPyramid() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisable(GL_CULL_FACE);   // Disable culling for this shape
        glVertexPointer(3, GL_FLOAT, 0, pyramidVerts);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, pyramidIndices);
        glEnable(GL_CULL_FACE);    // Re-enable for everything else
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // ---------- PRISM (triangular base) ----------
    static const float prismVerts[] = {
        // base triangle bottom
       -0.5f, 0.0f, -0.5f,  // 0
        0.5f, 0.0f, -0.5f,  // 1
        0.0f, 0.0f,  0.5f,  // 2
        // same triangle top
       -0.5f, 1.0f, -0.5f,  // 3
        0.5f, 1.0f, -0.5f,  // 4
        0.0f, 1.0f,  0.5f   // 5
    };

    static const GLuint prismIndices[] = {
        // bottom face
        0, 1, 2,
        // top face
        3, 5, 4,
        // sides
        0, 3, 1,
        1, 3, 4,
        1, 4, 2,
        2, 4, 5,
        2, 5, 0,
        0, 5, 3
    };

    void DrawPrism() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, prismVerts);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, prismIndices);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    // ---------- SPHERE (consistent winding, no culling) ----------
    void DrawSphere(int slices = 16, int stacks = 16)
    {
        glPushAttrib(GL_ENABLE_BIT);     // save current enables
        glDisable(GL_CULL_FACE);         // render both sides

        const float R = 0.5f;            // sphere radius

        for (int i = 0; i < stacks; ++i)
        {
            float lat0 = 3.14159265f * (-0.5f + (float)i / stacks);
            float lat1 = 3.14159265f * (-0.5f + (float)(i + 1) / stacks);

            float z0 = sinf(lat0) * R;
            float zr0 = cosf(lat0) * R;
            float z1 = sinf(lat1) * R;
            float zr1 = cosf(lat1) * R;

            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j <= slices; ++j)
            {
                float lng = 2.0f * 3.14159265f * (float)j / slices;
                float x = cosf(lng);
                float y = sinf(lng);

                glVertex3f(x * zr0, y * zr0, z0); // lower band vertex
                glVertex3f(x * zr1, y * zr1, z1); // upper band vertex
            }
            glEnd();
        }

        glPopAttrib();                   // restore culling state
    }

    // ---------- CONE ----------
    void DrawCone(int slices = 32, float height = 1.0f, float radius = 0.5f) {
        // Draw base circle
        glDisable(GL_CULL_FACE);   // Disable culling for this shape
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0); // center base
        for (int i = 0; i <= slices; ++i) {
            float angle = 2.0f * 3.14159265359f * i / slices;
            float x = cosf(angle) * radius;
            float z = sinf(angle) * radius;
            glVertex3f(x, 0, z);
        }
        glEnd();

        // Draw side triangles
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < slices; ++i) {
            float angle0 = 2.0f * 3.14159265359f * i / slices;
            float angle1 = 2.0f * 3.14159265359f * (i + 1) / slices;

            float x0 = cosf(angle0) * radius;
            float z0 = sinf(angle0) * radius;
            float x1 = cosf(angle1) * radius;
            float z1 = sinf(angle1) * radius;

            glVertex3f(x0, 0, z0);
            glVertex3f(x1, 0, z1);
            glVertex3f(0, height, 0);
        }
        glEnd();
        glEnable(GL_CULL_FACE);    // Re-enable for everything else
    }

    // ---------- CYLINDER ----------
    void DrawCylinder(int slices = 32, float height = 1.0f, float radius = 0.5f) {
        // Draw bottom circle
        glDisable(GL_CULL_FACE);   // Disable culling for this shape
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, 0, 0);
        for (int i = 0; i <= slices; ++i) {
            float angle = 2.0f * 3.14159265359f * i / slices;
            float x = cosf(angle) * radius;
            float z = sinf(angle) * radius;
            glVertex3f(x, 0, z);
        }
        glEnd();

        // Draw top circle
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0, height, 0);
        for (int i = 0; i <= slices; ++i) {
            float angle = 2.0f * 3.14159265359f * i / slices;
            float x = cosf(angle) * radius;
            float z = sinf(angle) * radius;
            glVertex3f(x, height, z);
        }
        glEnd();
        glEnable(GL_CULL_FACE);    // Re-enable for everything else

        // Draw side quads
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; ++i) {
            float angle = 2.0f * 3.14159265359f * i / slices;
            float x = cosf(angle) * radius;
            float z = sinf(angle) * radius;
            glVertex3f(x, 0, z);
            glVertex3f(x, height, z);
        }
        glEnd();
    }

} // namespace hz
