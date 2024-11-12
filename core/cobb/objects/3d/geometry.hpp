//
// Created by andrew.genualdo on 11/8/2024.
//

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "../../shader.hpp"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "iostream"
#include <assimp/scene.h>

using namespace std;
using namespace glm;

namespace cobb {
    class Geometry {

    };

    struct Vertex {
        vec3 Pos;
        vec3 Normal;
        vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        string type;
    };

    class Mesh {
    public:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };

    class Model {
    public:
        Model(char* path) {
            loadModel(path);
        }

        void Draw(Shader& shader);
    private:
        //model data
        vector<Mesh> meshes;
        string directory;

        void loadModel(string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };


}




#endif //GEOMETRY_H
