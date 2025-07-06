// Texture.h
// Texture loading and billboard drawing interface for Horizoneer Engine

#ifndef HZ_TEXTURE_H
#define HZ_TEXTURE_H

#include <GL/gl.h>

namespace hz {

	// Returns the OpenGL texture ID if already loaded, or 0 if not
	GLuint FindTexture(const char* path);

	// Loads a texture from disk and caches it. Returns 0 on failure.
	GLuint LoadTexture(const char* path);

	// Draws a billboard (camera-facing quad) with the given texture at position (x, y, z)
	void DrawBillboard(GLuint tex, float x, float y, float z, float size = 2.0f);

	// Draws the test texture located at C:\TexAsset\TT1.png in front of the camera
	void DrawTestTexture();

	// --- Normalize utility ---
	inline void Normalize(float& x, float& y, float& z);

} // namespace hz

#endif // HZ_TEXTURE_H