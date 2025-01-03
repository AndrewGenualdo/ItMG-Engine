//
// Created by cobble on 11/15/2024.
//

#include <algorithm>
#include "geometry.h"
#include "glm/geometric.hpp"

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

void cobb::createAsteroid(float radius, float variability, int subDivisions, cobb::MeshData *mesh) {
    mesh->vertices.clear();
    mesh->indices.clear();
    mesh->vertices.reserve((subDivisions + 1) * (subDivisions + 1));
    mesh->indices.reserve(subDivisions * subDivisions * 6);

    float thetaStep = 2 * PI / subDivisions;
    float phiStep = PI / subDivisions;

    for(size_t row = 0; row <= subDivisions; row++) {
        float phi = row * phiStep;

        float firstRadius = radius;

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

            float finalRadius = radius + RandomRange(radius * -variability, radius * variability);
            //this is to make sure the last column's radius lines up with the first column (so theres no gaps in the sphere)
            if(col == 0) firstRadius = finalRadius;
            else if(col == subDivisions) finalRadius = firstRadius;

            pos = normal * finalRadius;

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

std::vector<int> getAdjacentBand(const std::vector<glm::vec3>& points, int poleIndex) {
    std::vector<int> band;

    // Loop over the points, except the pole itself
    for (size_t i = 0; i < points.size(); ++i) {
        if (i != poleIndex) {
            band.push_back(i); // Collect the points forming the band
        }
    }

    return band;
}

void cobb::createBalancedSphere(float radius, int pointCount, cobb::MeshData *mesh) {
    mesh->vertices.clear();
    mesh->indices.clear();
    if(pointCount < 3) return;
    mesh->vertices.reserve(pointCount * 2);
    mesh->indices.reserve(pointCount * 11);

    float phi = PI * (sqrt(5.0f) - 1.0f);
    for(int i=0;i<pointCount;i++) {
        float y = 1 - (i / static_cast<float>(pointCount - 1)) * 2.0f;
        float r = sqrt(1 - y * y);
        float theta = phi * i;
        float x = cos(theta) * r;
        float z = sin(theta) * r;

        vec3 pos = vec3(x, y, z);

        //Note: Due to the vertices not being exactly on the seam, UVs only sort of work
        float u = atan2(z, x) / (2.0f * PI); // Longitude mapping
        u = u < 0.0f ? u + 1.0f : u;         // Normalize to range [0, 1]
        float v = 0.5f - (asin(y) / PI);

        mesh->vertices.emplace_back(pos * radius, pos, vec2(u, v));
    }

    //Generate Indices (Triangulation)
    float threshold = 1.6f * sqrt((4.0f * PI * radius * radius) / static_cast<float>(pointCount)); // Neighbor threshold
    for (size_t i = 0; i < mesh->vertices.size(); ++i) {
        //Find a close point to start a triangle with
        for (size_t j = i + 1; j < mesh->vertices.size(); ++j) {
            // Check distance between points
            if (distance(mesh->vertices[i].pos, mesh->vertices[j].pos) < threshold) {
                // Find a third point to form a triangle
                for (size_t k = j + 1; k < mesh->vertices.size(); ++k) {
                    if (distance(mesh->vertices[i].pos, mesh->vertices[k].pos) < threshold && distance(mesh->vertices[j].pos, mesh->vertices[k].pos) < threshold) {
                        // Add triangle
                        mesh->indices.emplace_back(i);
                        mesh->indices.emplace_back(j);
                        mesh->indices.emplace_back(k);
                    }
                }
            }
        }
    }

}
