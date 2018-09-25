#include "external_packager.hpp"
#include <fstream>
#include <iterator>
#include <random>

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

#include <stdio.h>

ExternalPackager::ExternalPackager(const std::string& path) noexcept
: path{path} {

}

std::vector<PackagedContent> ExternalPackager::package(const std::string& filepath) {
    std::vector<PackagedContent> contents;

    char* temp_name = tempnam(fs::temp_directory_path().c_str(), "sushi");
    const fs::path package_output_folder(temp_name);
    free(temp_name);

    fs::create_directory(package_output_folder);

    // Construct the command name
    std::string command_name = path + " \"" + filepath + "\" \"" + package_output_folder.string() + "\"";

    // Launch the external process
    system(command_name.c_str());

    // Iterate over all the created files
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator{package_output_folder}) {
        if(fs::is_regular_file(entry)) {
            PackagedContent content;

            std::ifstream file_stream(entry.path(), std::ios::binary);
            file_stream.read(reinterpret_cast<char*>(&content.type), sizeof(uint16_t));

            std::copy(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>{},
                      std::back_inserter(content.data));
            content.size = content.data.size();

            contents.push_back(std::move(content));
        }
    }

    // Remove the temp directory
    fs::remove_all(package_output_folder);

    return contents;
}

std::vector<std::string> ExternalPackager::supported_extensions() {
    std::vector<std::string> extensions;

    FILE* process = popen(path.c_str(), "r");
    if(process) {
        ssize_t read;
        char* line = nullptr;
        size_t len = 0;
        while((read = getline(&line, &len, process)) != -1) {
            extensions.emplace_back(line);
            extensions.back().pop_back();
        }

        pclose(process);
        free(line);
    }

    return extensions;
}
