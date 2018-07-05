add_library(sushi-input
        input_event.cpp
        input_event.hpp
        input_factory.cpp
        input_factory.hpp)
target_link_libraries(sushi-input
        PUBLIC sushi-core)