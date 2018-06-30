add_library(sushi-service
        job_service.cpp
        job_service.hpp
        log_service.cpp
        log_service.hpp
        static_mesh_builder_service.cpp
        static_mesh_builder_service.hpp
        input_processor_service.cpp
        input_processor_service.hpp)
target_link_libraries(sushi-service
        PUBLIC sushi-core)