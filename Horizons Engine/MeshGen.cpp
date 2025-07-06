#include <windows.h>
#include <string>
#include "MeshGen.h"
#include <cmath>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace hz {

    static MeshData lastStoredMesh;

    // Internal map to store mesh data by name
    static std::map<std::string, MeshData> meshStorage;

    // Customizable basic pyramid-like mesh for demo
    MeshData CreateMesh(float scale) {
        MeshData mesh;

        float s = scale;
        mesh.vertices = {
            0.0f,  s,  0.0f,   // top
           -s,  0.0f, -s,      // base
            s,  0.0f, -s,
            s,  0.0f,  s,
           -s,  0.0f,  s
        };

        mesh.indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 1,
            1, 2, 3,
            3, 4, 1
        };

        return mesh;
    }

    MeshData CreateCubeMesh(float size) {
        MeshData mesh;
        float s = size * 0.5f;

        mesh.vertices = {
            -s, -s, -s,  s, -s, -s,  s,  s, -s, -s,  s, -s,  // back
            -s, -s,  s,  s, -s,  s,  s,  s,  s, -s,  s,  s   // front
        };

        mesh.indices = {
            0, 1, 2,  2, 3, 0,      // back
            4, 5, 6,  6, 7, 4,      // front
            0, 4, 7,  7, 3, 0,      // left
            1, 5, 6,  6, 2, 1,      // right
            3, 2, 6,  6, 7, 3,      // top
            0, 1, 5,  5, 4, 0       // bottom
        };

        return mesh;
    }

    MeshData CreatePlaneMesh(float width, float depth) {
        MeshData mesh;
        float w = width * 0.5f;
        float d = depth * 0.5f;

        mesh.vertices = {
            -w, 0.0f, -d,
             w, 0.0f, -d,
             w, 0.0f,  d,
            -w, 0.0f,  d
        };

        mesh.indices = {
            0, 1, 2,
            2, 3, 0
        };

        return mesh;
    }

    void StoreMeshData(const std::string& name, const MeshData& data) {
        meshStorage[name] = data;
    }

    MeshData GetMeshData(const std::string& name) {
        if (meshStorage.count(name)) {
            return meshStorage[name];
        }
        else {
            OutputDebugStringA(("Mesh not found: " + name + "\n").c_str());
            return MeshData(); // Return empty
        }
    }

    bool HasMeshData(const std::string& name) {
        return meshStorage.count(name) > 0;
    }
}