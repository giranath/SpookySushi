add_library(sushi-asset
    public/package.hpp
    private/package.cpp
    public/package_types.hpp
        )
target_include_directories(sushi-asset
        PUBLIC public
        PRIVATE private)