add_library(sushi-input
        private/input_event.cpp
        public/input_event.hpp
        private/input_factory.cpp
        public/input_factory.hpp
        private/input_bus.cpp
        public/input_bus.hpp
        private/input_state.cpp
        public/input_state.hpp
        private/input_processor.cpp
        public/input_processor.hpp
        private/axis_input_processor.cpp
        public/axis_input_processor.hpp)
target_link_libraries(sushi-input
        PUBLIC sushi-core
        PRIVATE sushi-service)
target_include_directories(sushi-input
        PUBLIC public
        PRIVATE private)