#ifndef SPOOKYSUSHI_OPTIONAL_SWITCH_HPP
#define SPOOKYSUSHI_OPTIONAL_SWITCH_HPP

#include "../platform/host_infos.hpp"

#if SUSHI_SUPPORT_STD_OPTIONAL
#include <optional>
#elif SUSHI_SUPPORT_EXPERIMENTAL_OPTIONAL
#include <experimental/optional>
#else
#error "optional type not supported by your toolchain"
#endif

namespace sushi {

#if SUSHI_SUPPORT_STD_OPTIONAL
    template<typename T>
    using Optional = std::optional<T>;
#elif SUSHI_SUPPORT_EXPERIMENTAL_OPTIONAL
    template<typename T>
    using Optional = std::experimental::optional<T>;
#endif

}

#endif
