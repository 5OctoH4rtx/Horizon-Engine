#include <windows.h>
#include "Aplex.h"
#include "Texture.h"
#include "MeshGen.h"
#include "Shape.h" // for DrawTriangle, DrawCube, etc.

#include <GL/gl.h>

namespace hz {

    void BindTexture(GLuint texID) {
        glBindTexture(GL_TEXTURE_2D, texID);
    }

    void ApplyTextureTo(const std::string& shapeName, const std::string& texturePath) {
        GLuint texID = LoadTexture(texturePath.c_str());
        if (texID == 0) {
            OutputDebugStringA(("Failed to load texture: " + texturePath + "\n").c_str());
            return;
        }

        glEnable(GL_TEXTURE_2D);
        BindTexture(texID);

        // Set basic OpenGL state for texturing
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        // CASES — apply to hardcoded shape draw functions
        if (shapeName == "Triangle") {
            DrawTriangle();
        }
        else if (shapeName == "Plane") {
            DrawPlane();
        }
        else if (shapeName == "Circle") {
            InitCircle();
            DrawCircle();
        }
        else if (shapeName == "Cube") {
            DrawCube();
        }
        else if (shapeName == "Pyramid") {
            DrawPyramid();
        }
        else if (shapeName == "Prism") {
            DrawPrism();
        }
        else if (shapeName == "Sphere") {
            DrawSphere();
        }
        else if (shapeName == "Cone") {
            DrawCone();
        }
        else if (shapeName == "Cylinder") {
            DrawCylinder();
        }

        // If it’s a stored mesh:
        else if (HasMeshData(shapeName)) {
            MeshData mesh = GetMeshData(shapeName);

            glBegin(GL_TRIANGLES);
            for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                for (int j = 0; j < 3; ++j) {
                    GLuint index = mesh.indices[i + j];
                    float x = mesh.vertices[index * 3 + 0];
                    float y = mesh.vertices[index * 3 + 1];
                    float z = mesh.vertices[index * 3 + 2];

                    // Very basic planar mapping (XZ)
                    float u = (x + 1.0f) * 0.5f;
                    float v = (z + 1.0f) * 0.5f;
                    glTexCoord2f(u, v);

                    glVertex3f(x, y, z);
                }
            }
            glEnd();
        }

        glDisable(GL_TEXTURE_2D);
    }

}
