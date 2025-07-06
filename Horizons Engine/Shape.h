// Shape.h
#pragma once

namespace hz {

	// Draw basic shapes
	void DrawTriangle();
	void DrawPlane();
	void InitCircle();   // Call once before drawing circle
	void DrawCircle();

	void DrawCube();
	void DrawPyramid();
	void DrawPrism();

	void DrawSphere(int slices = 16, int stacks = 16);

	void DrawCone(int slices = 32, float height = 1.0f, float radius = 0.5f);

	void DrawCylinder(int slices = 32, float height = 1.0f, float radius = 0.5f);

} // namespace hz