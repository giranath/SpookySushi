#ifndef SPOOKYSUSHI_SERVICE_LOCATOR_HPP
#define SPOOKYSUSHI_SERVICE_LOCATOR_HPP

namespace sushi {

template<typename ServiceType>
class ServiceLocator {
public:
    using service_type = ServiceType;
    using reference = ServiceType&;
    using pointer = ServiceType*;
private:
    static pointer service;

public:
    static void locate(pointer service_ptr) {
        service = service_ptr;
    }

    static reference& get() noexcept {
        return *service;
    }
};

template<typename ServiceType>
ServiceType* ServiceLocator<ServiceType>::service = nullptr;

}

#endif //SPOOKYSUSHI_SERVICE_LOCATOR_HPP