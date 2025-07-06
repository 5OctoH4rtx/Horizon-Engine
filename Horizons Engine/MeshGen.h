#pragma once
#include <windows.h>
#include <vector>
#include <GL/gl.h>

namespace hz {

    struct MeshData {
        std::vector<float> vertices;   // (x, y, z) triplets
        std::vector<GLuint> indices;   // triangle indices
    };

    //basic customizable mesh
    MeshData CreateMesh(float scale = 1.0f);

    // Basic generators
    MeshData CreateCubeMesh(float size = 1.0f);
    MeshData CreatePlaneMesh(float width = 1.0f, float depth = 1.0f);

    // Store/retrieve mesh by name
    void StoreMeshData(const std::string& name, const MeshData& data);
    MeshData GetMeshData(const std::string& name);
    bool HasMeshData(const std::string& name); // Optional check
}