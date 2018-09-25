#ifndef SPOOKYSUSHI_EXTERNAL_PACKAGER_HPP
#define SPOOKYSUSHI_EXTERNAL_PACKAGER_HPP

#include "base_packager.hpp"

class ExternalPackager : public BasePackager {
    std::string path;
public:
    ExternalPackager(const std::string& path) noexcept;
    std::vector<PackagedContent> package(const std::string &filepath) override;
    std::vector<std::string> supported_extensions();
};


#endif //SPOOKYSUSHI_EXTERNAL_PACKAGER_HPP
