#ifndef SPOOKYSUSHI_INPUT_PROCESSOR_SERVICE_HPP
#define SPOOKYSUSHI_INPUT_PROCESSOR_SERVICE_HPP


namespace sushi {

class InputProcessorFacade;

class InputProcessorService {
    static InputProcessorFacade* location;
public:
    static void locate(InputProcessorFacade* engine);
    static InputProcessorFacade& get() noexcept;
};
}

#endif //SPOOKYSUSHI_INPUT_PROCESSOR_SERVICE_HPP
