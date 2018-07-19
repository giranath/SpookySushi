add_library(sushi-network
        private/endpoint.cpp
        public/endpoint.hpp
        private/ipv4_address.cpp
        public/ipv4_address.hpp
        private/ipv6_address.cpp
        public/ipv6_address.hpp
        private/tcp_socket.cpp
        public/tcp_socket.hpp
        )
target_include_directories(sushi-network
        PRIVATE "../driver/socket/${SUSHI_DRIVER_SOCKET_NAME}"
        PUBLIC public
        PRIVATE private)
add_dependencies(sushi-network sushi-drivers)
target_link_libraries(sushi-network
        PUBLIC sushi-async)