configure_file(host_infos.in.hpp ${CMAKE_CURRENT_SOURCE_DIR}/host_infos.hpp)

add_library(sushi-platform OBJECT
        optional.hpp
        host_infos.hpp)