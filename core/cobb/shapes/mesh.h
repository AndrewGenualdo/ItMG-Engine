//
// Created by cobble on 11/15/2024.
//

#ifndef EWRENDER_MESH_HPP
#define EWRENDER_MESH_HPP

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <vector>
#include "../../ew/ewMath/ewMath.h"
#include "../../ew/external/glad.h"

using namespace glm;
using namespace ew;

namespace cobb {

    struct ProceduralVertex {
        vec3 pos = vec3(0);
        vec3 normal = vec3(0);
        vec2 uv = vec2(0);

        ProceduralVertex() = default;

        ProceduralVertex(const vec3 &pos, const vec3 &normal, const vec2 &uv) : pos(pos), normal(normal), uv(uv) {

        }
    };

    struct MeshData {
        std::vector<ProceduralVertex> vertices;
        std::vector<unsigned int> indices;
        MeshData() = default;
    };



    class ProceduralMesh {
    public:
        ProceduralMesh() = default;
        explicit ProceduralMesh(const MeshData &meshData);
        void load(const MeshData &meshData);
        void draw() const;
        void bind() const;

    private:
        bool initialized = false;
        unsigned int VAO = 0, VBO = 0, EBO = 0;
        int vertexCount = 0, indexCount = 0;

    };
}



#endif //EWRENDER_MESH_HPP
