#pragma once

#include "gaunlet/scene/model/Mesh.h"

#include "gaunlet/pch.h"

#include <assimp/scene.h>

namespace gaunlet::Scene {

    class Model {

    public:
        Model() = default;
        Model(const Mesh& mesh);
        explicit Model(const char* filePath);

        inline const std::vector<Mesh>& getMeshes() const {return m_meshes; }

        std::vector<Mesh> m_meshes;
    private:

        void loadModel(const char* filePath);
        void processModelNode(aiNode* node, const aiScene* scene);
        Mesh processNodeMesh(aiMesh *mesh);

    };

}