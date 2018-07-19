add_library(sushi-core
        public/rect.hpp
        public/string_utils.hpp
        public/types.hpp
        public/string_parser_helper.hpp
        public/xml.hpp
        private/xml.cpp)

target_link_libraries(sushi-core
        PUBLIC libGlm
        PUBLIC libRapidxml)
target_include_directories(sushi-core
        PRIVATE private
        PUBLIC public)
