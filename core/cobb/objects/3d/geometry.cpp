//
// Created by cobble on 11/15/2024.
//

#include "geometry.hpp"

void cobb::createSphere(float radius, int subDivisions, cobb::MeshData *mesh) {
    mesh->vertices.clear();
    mesh->indices.clear();
    mesh->vertices.reserve((subDivisions + 1) * (subDivisions + 1));
    mesh->indices.reserve(subDivisions * subDivisions * 6);

    float thetaStep = 2 * PI / subDivisions;
    float phiStep = PI / subDivisions;

    for(size_t row = 0; row <= subDivisions; row++) {
        float phi = row * phiStep;
        for(size_t col = 0; col <= subDivisions; col++) {
            float theta = col * thetaStep;

            vec2 uv;
            vec3 pos;
            vec3 normal;

            uv.x = 1.0f - ((static_cast<float>(col) / subDivisions));
            uv.y = 1.0f - ((static_cast<float>(row) / subDivisions));

            normal.x = cosf(theta) * sinf(phi);
            normal.y = cosf(phi);
            normal.z = sinf(theta) * sinf(phi);

            pos = normal * radius;

            mesh->vertices.emplace_back(pos, normal, uv);
        }
    }

    //indices
    for(size_t row = 0; row < subDivisions; row++) {
        for(size_t col = 0; col < subDivisions; col++) {
            unsigned int bl = row * (subDivisions + 1) + col;
            unsigned int br = bl + 1;
            unsigned int tl = bl + subDivisions + 1;
            unsigned int tr = tl + 1;

            //Triangle 1
            mesh->indices.emplace_back(bl);
            mesh->indices.emplace_back(br);
            mesh->indices.emplace_back(tr);
            //Triangle 2
            mesh->indices.emplace_back(tr);
            mesh->indices.emplace_back(tl);
            mesh->indices.emplace_back(bl);
        }
    }
}
