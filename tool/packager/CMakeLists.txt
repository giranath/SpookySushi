add_library(sushi-package
        src/base_packager.hpp
        src/base_packager.cpp
        src/model_packager.hpp
        src/model_packager.cpp src/external_packager.cpp src/external_packager.hpp)
target_link_libraries(sushi-package
        PUBLIC sushi-asset
        PUBLIC libAssimp)

add_executable(sushi-packager
        src/main.cpp)
target_link_libraries(sushi-packager
        PUBLIC sushi-package
        PRIVATE stdc++fs)