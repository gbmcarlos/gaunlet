#pragma once

#include "polygon-mesh/PolygonMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <vector>

namespace engine {

    class Model {

    public:
        Model() = default;
        explicit Model(const char* filePath);

        inline const std::vector<PolygonMesh>& getMeshes() const {return m_meshes; }

    private:
        std::vector<PolygonMesh> m_meshes;

        void loadModel(const char* filePath);
        void processModelNode(aiNode* node, const aiScene* scene);
        PolygonMesh processNodeMesh(aiMesh *mesh);

    };

}