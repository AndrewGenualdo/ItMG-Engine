//
// Created by andrew.genualdo on 11/8/2024.
//

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "iostream"
#include "../core/shader.h"
#include <assimp/scene.h>

using namespace glm;

namespace cobb {
    class Geometry {

    };

    struct Vertex {
        vec3 Pos;
        vec3 Normal;
        vec2 TexCoords;
        vec3 Tangent;
        vec3 Bitangent;
        int m_BoneIDs[4];
        int m_Weights[4];
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };

    class Model {
    public:
        Model(std::string &path, bool gamma = false) : gammaCorrection(gamma) {
            loadModel(path);
        }

        void Draw(Shader& shader);
    //private:
        //model data
    std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
        bool gammaCorrection;

        void loadModel(const std::string &path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char *path, const std::string &directory);
    };


}




#endif //GEOMETRY_H
