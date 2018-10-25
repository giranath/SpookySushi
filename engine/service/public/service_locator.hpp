#ifndef SPOOKYSUSHI_SERVICE_LOCATOR_HPP
#define SPOOKYSUSHI_SERVICE_LOCATOR_HPP

#include <cassert>

namespace sushi {

template<typename Service>
struct InvalidServiceLocation{
    using service_type = Service;
};

template<typename ServiceType>
class ServiceLocator {
public:
    using service_type = ServiceType;
    using reference = ServiceType&;
    using pointer = ServiceType*;
private:
    static pointer& service() {
        static pointer _service = nullptr;

        return _service;
    }

public:
    static void locate(pointer service_ptr) {
        if(service_ptr == nullptr) throw InvalidServiceLocation<ServiceType>{};
        ServiceLocator::service() = service_ptr;
    }

    static reference& get() noexcept {
        return *ServiceLocator::service();
    }

    static bool is_located() noexcept {
        return ServiceLocator::service() != nullptr;
    }
};

}

#endif //SPOOKYSUSHI_SERVICE_LOCATOR_HPP
