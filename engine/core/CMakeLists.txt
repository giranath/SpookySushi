add_library(sushi-core
        public/rect.hpp
        public/string_utils.hpp
        public/types.hpp
        public/string_parser_helper.hpp
        public/xml.hpp
        private/xml.cpp
        public/murmurhash.hpp
        private/murmurhash.cpp
        public/compress.hpp
        private/compress.cpp public/color.hpp)

target_link_libraries(sushi-core
        PUBLIC libGlm
        PUBLIC libRapidxml
        PRIVATE libZlib)
target_include_directories(sushi-core
        PRIVATE private
        PUBLIC public)
