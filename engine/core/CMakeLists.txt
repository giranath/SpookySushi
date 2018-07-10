add_library(sushi-core
        rect.hpp
        string_utils.hpp
        types.hpp
        temp.cpp)

target_link_libraries(sushi-core
        PUBLIC libGlm)
