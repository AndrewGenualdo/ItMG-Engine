//
// Created by cobble on 11/15/2024.
//

#include "mesh.h"

cobb::ProceduralMesh::ProceduralMesh(const cobb::MeshData &meshData) {
    load(meshData);
}

void cobb::ProceduralMesh::load(const cobb::MeshData &meshData) {
    if(!initialized) {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ProceduralVertex), (const void*)offsetof(ProceduralVertex, pos));
        glEnableVertexAttribArray(0);

        //normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ProceduralVertex), (const void*)offsetof(ProceduralVertex, normal));
        glEnableVertexAttribArray(1);

        //uvs
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ProceduralVertex), (const void*)offsetof(ProceduralVertex, uv));
        glEnableVertexAttribArray(2);

        initialized = true;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    if(!meshData.vertices.empty()) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(ProceduralVertex) * meshData.vertices.size(), meshData.vertices.data(), GL_STATIC_DRAW);
    }
    if(!meshData.indices.empty()) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.indices.size(), meshData.indices.data(), GL_STATIC_DRAW);
    }
    vertexCount = meshData.vertices.size();
    indexCount = meshData.indices.size();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cobb::ProceduralMesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void cobb::ProceduralMesh::bind() const {
    glBindVertexArray(VAO);
}
