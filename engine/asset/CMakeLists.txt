add_library(sushi-asset
    public/package.hpp
    private/package.cpp
    public/package_types.hpp
    public/mesh_asset.hpp
    private/mesh_asset.cpp)
target_include_directories(sushi-asset
    PUBLIC public
    PRIVATE private)
target_link_libraries(sushi-asset PUBLIC sushi-core)