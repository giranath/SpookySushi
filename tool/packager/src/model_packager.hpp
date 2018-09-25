#ifndef SPOOKYSUSHI_MODEL_PACKAGER_HPP
#define SPOOKYSUSHI_MODEL_PACKAGER_HPP

#include "base_packager.hpp"

class aiScene;
namespace Assimp {
class Importer;
}

class ModelPackager : public BasePackager {
    const aiScene* load_scene(Assimp::Importer& importer, std::istream& stream);
public:
    std::vector<PackagedContent> package(const std::string& filepath) override;
};


#endif //SPOOKYSUSHI_MODEL_PACKAGER_HPP
