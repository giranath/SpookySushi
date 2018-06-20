if(SUSHI_RENDERING_BACKEND STREQUAL "OpenGL")
    set(SUSHI_USE_OPENGL 1)
endif()

configure_file(host_infos.in.hpp ${CMAKE_CURRENT_SOURCE_DIR}/host_infos.hpp)