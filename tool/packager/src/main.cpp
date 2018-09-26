#include "base_packager.hpp"
#include "model_packager.hpp"
#include "external_packager.hpp"
#include <murmurhash.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <unordered_map>

#ifdef __has_include
    #if __has_include(<filesystem>)
        #include <filesystem>
        namespace fs = std::filesystem;
    #elif __has_include(<experimental/filesystem>)
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #else
        #error "need c++ filesystem support"
    #endif
#else
    #error "need c++ filesystem support"
#endif

#ifdef _WIN32
#else
// To get home directory on Linux
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

void usage() {
    std::cout << "usage: sushi-package [-o <package_file>] <asset_files>..." << std::endl;
}

void help() {
    std::cout << "sushi-package\n"
              << "packages assets for the sushi engine\n\n"
              << "OPTIONS:\n"
              << " -o, --output      specifies the package file to create\n"
              << "                   when nothing is specified, default.spkg will be used\n"
              << " -z, --compress    compress the package with zlib\n"
              << "     --no-compress disable the package compression\n"
              << "\n"
              << "ASSET FILES:\n"
              << "you can specify as mush files as you want to store in a single package file" << std::endl;
}

void version() {
    std::cout << "sushi-package (sushi) 1.0.0" << std::endl;
}

struct Arguments {
    std::string package_file;
    std::vector<std::string> asset_files;
    bool should_compress;

    Arguments()
    : package_file{"default.spkg"}
    , asset_files{}
    , should_compress{false} {

    }

    static Arguments parse(int argc, char* argv[]) {
        Arguments parsed_args;
        std::vector<std::string> arguments{argv, argv + argc};

        bool parse_options = true;
        for(int i = 1; i < argc; ++i) {
            const std::string& current_argument = arguments[i];

            if(current_argument == "--") {
                parse_options = false;
            }
            else if(current_argument[0] == '-' && parse_options) {
                if(current_argument == "--output" || current_argument == "-o") {
                    if(i < argc - 1) {
                        parsed_args.package_file = arguments[i + 1];
                        ++i;
                    }
                    else {
                        usage();
                        exit(1);
                    }
                }
                else if(current_argument == "--compress" || current_argument == "-z") {
                    parsed_args.should_compress = true;
                }
                else if(current_argument == "--no-compress") {
                    parsed_args.should_compress = false;
                }
            }
            else {
                parsed_args.asset_files.push_back(current_argument);
            }
        }

        return parsed_args;
    }
};

void setup_packagers(std::vector<std::unique_ptr<BasePackager>>& packagers, std::unordered_map<std::string, BasePackager*>& mapping) {
    std::unique_ptr<ModelPackager> packager = std::make_unique<ModelPackager>();
    mapping["fbx"] = packager.get();
    mapping["dae"] = packager.get();
    mapping["blend"] = packager.get();
    mapping["3ds"] = packager.get();
    mapping["ase"] = packager.get();
    mapping["obj"] = packager.get();
    mapping["md1"] = packager.get();
    mapping["md2"] = packager.get();
    mapping["md3"] = packager.get();
    mapping["mdc"] = packager.get();
    mapping["md5"] = packager.get();
    mapping["3d"] = packager.get();
    mapping["irrmesh"] = packager.get();

    packagers.push_back(std::move(packager));
}

fs::path home_directory() {
    const char* homedir;
    if((homedir = getenv("HOME")) == nullptr) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    return fs::path(homedir);
}

fs::path config_directory() {
    return home_directory() / ".config" / "sushi";
}

fs::path share_directory() {
    return home_directory() / ".local" / "share" / "sushi";
}

fs::path packagers_directory() {
    return share_directory() / "packagers";
}

const bool is_executable(const fs::file_status& status) {
    return (status.permissions() & fs::perms::owner_exec) != fs::perms::none
         | (status.permissions() & fs::perms::group_exec) != fs::perms::none
         | (status.permissions() & fs::perms::others_exec) != fs::perms::none;
}

int main(int argc, char* argv[]) {
    Arguments args = Arguments::parse(argc, argv);
    if(args.asset_files.empty()) {
        std::cerr << "you must specify at least one asset file" << std::endl;
        usage();
        return 1;
    }

    std::vector<std::unique_ptr<BasePackager>> packagers;
    std::unordered_map<std::string, BasePackager*> mapping;
    setup_packagers(packagers, mapping);

    if(fs::exists(packagers_directory())) {
        for (const fs::directory_entry &entry : fs::directory_iterator{packagers_directory()}) {
            if (!fs::is_directory(entry) && is_executable(entry.status())) {
                std::unique_ptr<ExternalPackager> external_packager = std::make_unique<ExternalPackager>(
                        entry.path());

                for (const std::string &extension : external_packager->supported_extensions()) {
                    mapping[extension] = external_packager.get();
                }

                packagers.push_back(std::move(external_packager));
            }
        }
    }

    std::vector<PackagedContent> packaged_contents;

    for(const std::string& asset_file : args.asset_files) {
        std::string extension;
        auto it = asset_file.find_last_of('.');
        if(it != std::string::npos) {
            extension = asset_file.substr(it + 1);

            const auto it = mapping.find(extension);
            if(it != std::end(mapping)) {
                const std::vector<PackagedContent> entries = it->second->package(asset_file);
                packaged_contents.reserve(packaged_contents.size() + entries.size());
                std::copy(std::begin(entries), std::end(entries), std::back_inserter(packaged_contents));
            }
        }
        else {
            std::cerr << "missing file extension of '" << asset_file << "'" << std::endl;
        }
    }

    if(packaged_contents.size() > std::numeric_limits<uint16_t>::max()) {
        std::cerr << "too many assests in package" << std::endl;
        return 1;
    }

    uint16_t package_flags = sushi::PackageHeader::MaskLittleEndian;
    if(args.should_compress) {
        package_flags |= sushi::PackageHeader::MaskCompressed;
    }

    sushi::Package package{sushi::PackageHeader{1, package_flags}};
    uint64_t offset = 0;
    for(uint16_t id = 0; id < packaged_contents.size(); ++id) {
        const PackagedContent& content = packaged_contents[id];
        sushi::PackageEntry entry;
        entry.identifier = id;
        entry.type = static_cast<sushi::PackageType>(content.type);
        entry.size = content.size;
        entry.offset = offset;
        entry.checksum = sushi::murmur_hash3_x86_32(content.data.data(), content.size, 0);

        offset += entry.size;
        package.add_entry(entry, content.data);
    }

    std::ofstream package_file(args.package_file, std::ios::binary);
    package_file << package;
}