add_library(sushi-input
        input_event.cpp
        input_event.hpp
        input_factory.cpp
        input_factory.hpp
        input_bus.cpp
        input_bus.hpp input_state.cpp input_state.hpp input_processor.cpp input_processor.hpp axis_input_processor.cpp axis_input_processor.hpp)
target_link_libraries(sushi-input
        PUBLIC sushi-core)