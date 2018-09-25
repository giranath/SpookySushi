#include "model_packager.hpp"
#include <mesh_asset.hpp>
#include <types.hpp>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static const uint8_t CURRENT_SERIALIZATION_VERSION = 0x1;

static uint8_t extract_mesh_flags(const aiMesh& mesh) {
    uint8_t flags = sushi::mesh_asset::FLAGS_NONE;

    if(mesh.mNumVertices > std::numeric_limits<uint16_t>::max()) {
        flags |= sushi::mesh_asset::FLAGS_LARGE_INDEX;
    }

    if(mesh.HasFaces()) {
        flags |= sushi::mesh_asset::FLAGS_IS_INDEXED;
    }

    if(mesh.HasNormals()) {
        flags |= sushi::mesh_asset::FLAGS_NORMALS;
    }

    return flags;
}

const aiScene* ModelPackager::load_scene(Assimp::Importer& importer, std::istream& stream) {
    std::vector<char> stream_content;
    std::copy(std::istreambuf_iterator<char>{stream}, std::istreambuf_iterator<char>{},
              std::back_inserter(stream_content));


    return importer.ReadFileFromMemory(stream_content.data(), stream_content.size(), aiProcess_CalcTangentSpace
                                                                                   | aiProcess_Triangulate
                                                                                   | aiProcess_JoinIdenticalVertices
                                                                                   | aiProcess_SortByPType);
}

std::vector<PackagedContent> ModelPackager::package(const std::string& filepath) {
    std::ifstream stream(filepath, std::ios::binary);
    std::vector<PackagedContent> packaged_contents;

    Assimp::Importer importer;
    const aiScene* scene = load_scene(importer, stream);
    if(scene) {

        uint16_t id = 0;
        for(unsigned int mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index, ++id) {
            const aiMesh* mesh = scene->mMeshes[mesh_index];

            if((mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) != 0) {
                sushi::MeshAsset::vertex_array vertices;
                sushi::MeshAsset::normal_array normals;
                sushi::MeshAsset::texture_coord_array uvs;
                sushi::MeshAsset::index_array indices;

                vertices.reserve(mesh->mNumVertices);
                std::transform(mesh->mVertices + 0, mesh->mVertices + mesh->mNumVertices, std::back_inserter(vertices), [](const aiVector3D& vec3) {
                    return sushi::Vec3(vec3.x, vec3.y, vec3.z);
                });

                if(mesh->HasNormals()) {
                    normals.reserve(mesh->mNumVertices);
                    std::transform(mesh->mNormals + 0, mesh->mNormals + mesh->mNumVertices, std::back_inserter(normals), [](const aiVector3D& vec3) {
                        return sushi::Vec3(vec3.x, vec3.y, vec3.z);
                    });
                }

                if(mesh->HasTextureCoords(0)) {
                    uvs.reserve(mesh->mNumVertices);

                    for(uint32_t i = 0; i < mesh->mNumVertices; ++i) {
                        const float x = mesh->mTextureCoords[0][i].x;
                        const float y = mesh->mTextureCoords[0][i].y;

                        uvs.emplace_back(x, y);
                    }
                }

                if(mesh->HasFaces()) {
                    indices.reserve(mesh->mFaces->mNumIndices * mesh->mNumFaces * 3); // A face is a triangle

                    for(uint32_t i = 0; i < mesh->mNumFaces; ++i) {
                        const aiFace& face = mesh->mFaces[i];
                        assert(face.mNumIndices == 3);
                        std::copy(face.mIndices + 0, face.mIndices + face.mNumIndices, std::back_inserter(indices));
                    }
                }

                sushi::MeshAsset asset(vertices, normals, uvs, indices);
                std::ostringstream stream;
                stream << asset;

                const std::string data = stream.str();
                packaged_contents.emplace_back(sushi::PACKAGE_3D_MODEL, data.size(),
                                               reinterpret_cast<const uint8_t*>(data.c_str()));
            }
        }
    }
    else {
        throw UnsupportedFormat{};
    }

    return packaged_contents;
}
