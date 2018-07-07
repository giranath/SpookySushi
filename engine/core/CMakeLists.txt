add_library(sushi-core
        optional.hpp
        rect.hpp
        string_utils.hpp
        types.hpp
        temp.cpp)

target_link_libraries(sushi-core
        PUBLIC libGlm)
