add_library(sushi-core
        rect.hpp
        string_utils.hpp
        types.hpp
        string_parser_helper.hpp
        xml.hpp
        xml.cpp)

target_link_libraries(sushi-core
        PUBLIC libGlm
        PUBLIC libRapidxml)
