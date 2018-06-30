add_library(sushi-core
        optional.hpp
        rect.hpp
        string_utils.hpp
        types.hpp
        temp.cpp)

target_include_directories(sushi-core PUBLIC ${GLM_INCLUDE_DIRS})
