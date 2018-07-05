add_library(sushi-input
        input_event.cpp
        input_event.hpp
        input_factory.cpp
        input_factory.hpp
        input_context.cpp
        input_context.hpp
        input_processor.cpp
        input_processor.hpp
        input_handler.cpp
        input_handler.hpp key_state_input_handler.cpp key_state_input_handler.hpp key_axis_input_handler.cpp key_axis_input_handler.hpp mouse_axis_input_handler.cpp mouse_axis_input_handler.hpp)
target_link_libraries(sushi-input
        PUBLIC sushi-core)