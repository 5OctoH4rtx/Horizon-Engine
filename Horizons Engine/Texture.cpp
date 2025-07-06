// Texture.cpp
// Basic texture loading and placement utilities for Horizoneer Engine.

#define STB_IMAGE_IMPLEMENTATION
#include <windows.h>
#include "stb_image.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <map>
#include <string>
#include <cmath>
#include <vector>

#include "Texture.h"

#include "MainUtils.h"

extern float eyeX, eyeY, eyeZ;
extern float centerX, centerY, centerZ;

namespace hz {

    //--------------------------------------------------------------------
    // Internal helpers
    //--------------------------------------------------------------------
    static std::map<std::string, GLuint> textureCache;

    // Finds an already loaded texture or returns 0
    GLuint FindTexture(const char* path)
    {
        auto it = textureCache.find(path);
        if (it != textureCache.end())
            return it->second;
        return 0;
    }

    // Loads a texture from disk (and caches it)
    GLuint LoadTexture(const char* path)
    {
        // Return cached version if already loaded
        GLuint existing = FindTexture(path);
        if (existing) return existing;

        int width, height, channels;
        unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
        if (!data) {
            MessageBoxA(nullptr, (std::string("Failed to load texture: ") + path).c_str(),
                "Texture Error", MB_ICONERROR);
            return 0;
        }

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
            GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
        textureCache[path] = id;
        return id;
    }

    // Camera globals (should be defined elsewhere)

    inline void Normalize(float& x, float& y, float& z)
    {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length > 0.0001f) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    // Draw a single camera-facing billboard
    void DrawBillboard(GLuint tex, float px, float py, float pz, float size)
    {
        float fx = centerX - eyeX;
        float fy = centerY - eyeY;
        float fz = centerZ - eyeZ;
        Normalize(fx, fy, fz);

        float ux = 0.0f, uy = 1.0f, uz = 0.0f;

        float rx = fy * uz - fz * uy;
        float ry = fz * ux - fx * uz;
        float rz = fx * uy - fy * ux;
        Normalize(rx, ry, rz);

        float vx = ry * fz - rz * fy;
        float vy = rz * fx - rx * fz;
        float vz = rx * fy - ry * fx;
        Normalize(vx, vy, vz);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(px - (rx + vx) * size * 0.5f, py - (ry + vy) * size * 0.5f, pz - (rz + vz) * size * 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(px + (rx - vx) * size * 0.5f, py + (ry - vy) * size * 0.5f, pz + (rz - vz) * size * 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(px + (rx + vx) * size * 0.5f, py + (ry + vy) * size * 0.5f, pz + (rz + vz) * size * 0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(px - (rx - vx) * size * 0.5f, py - (ry - vy) * size * 0.5f, pz - (rz - vz) * size * 0.5f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void DrawFixedQuad(GLuint texture, float x, float y, float z, float size)
    {
        glDisable(GL_CULL_FACE);  // disables back-face culling
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glPushMatrix();
        glTranslatef(x, y, z);

        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-size, -size, 0);
        glTexCoord2f(1, 0); glVertex3f(size, -size, 0);
        glTexCoord2f(1, 1); glVertex3f(size, size, 0);
        glTexCoord2f(0, 1); glVertex3f(-size, size, 0);
        glEnd();

        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_CULL_FACE);  // disables back-face culling
    }

    static GLuint testTexture = 0;

    void DrawTestTexture()
    {
        static GLuint testTexture = 0;
        if (testTexture == 0) {
            const char* path = "C:\\TexAsset\\TT1.png";
            testTexture = LoadTexture(path);
            if (!testTexture) return;
        }
        if (!showTestTexture) return;

        DrawFixedQuad(testTexture, testTexPosX, testTexPosY, testTexPosZ, 1.0f);
    }

    struct SimpleQuad {
        GLuint tex;
        float x, y, z;
        float size;
    };

    std::vector<SimpleQuad> allSimpleQuads;

    void DrawAllTextures()
    {
        for (const auto& q : allSimpleQuads)
        {
            DrawFixedQuad(q.tex, q.x, q.y, q.z, q.size);
        }
    }

} // namespace hz
