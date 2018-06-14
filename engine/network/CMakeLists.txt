add_library(sushi-network
        endpoint.cpp
        endpoint.hpp
        ipv4_address.cpp
        ipv4_address.hpp
        ipv6_address.cpp
        ipv6_address.hpp
        tcp_socket.cpp
        tcp_socket.hpp
        )
target_include_directories(sushi-network
        PRIVATE "../driver/socket/${SUSHI_DRIVER_SOCKET_NAME}")
add_dependencies(sushi-network sushi-drivers)