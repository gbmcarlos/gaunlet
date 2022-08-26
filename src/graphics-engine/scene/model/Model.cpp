#include "graphics-engine/scene/model/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace engine::Scene {

    Model::Model(const char *filePath) {
        loadModel(filePath);
    }

    void Model::loadModel(const char *filePath) {

        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw std::runtime_error(importer.GetErrorString());
        }

        processModelNode(scene->mRootNode, scene);

    }

    void Model::processModelNode(aiNode* node, const aiScene* scene) {

        GE_PROFILE_FUNCTION;

        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            Mesh mesh = processNodeMesh(scene->mMeshes[node->mMeshes[i]]);
            m_meshes.push_back(mesh);
        }

        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processModelNode(node->mChildren[i], scene);
        }

    }

    Mesh Model::processNodeMesh(aiMesh *mesh) {

        GE_PROFILE_FUNCTION;

        std::vector<Graphics::Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

            // The vertex's position
            glm::vec4 position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f};

            // The vertex's normal
            glm::vec3 normal = {1.0f, 1.0f, 1.0f};
            if (mesh->HasNormals()) {
                normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            }

            // The vertex's texture coordinates
            glm::vec2 textureCoordinates = {1.0f, 1.0f};
            if (mesh->mTextureCoords[0]) {
                textureCoordinates = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            }

            vertices.emplace_back(position, normal, textureCoordinates);

        }

        // now walk through each of the mesh's faces and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        return {vertices, indices};

    }

}