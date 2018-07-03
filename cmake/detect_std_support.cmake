include(CheckCXXSourceCompiles)

# Check if std::optional exists
check_cxx_source_compiles("\
#include<optional>
using optional = std::optional<int>;
int main() {}" SUSHI_COMPILE_OPTIONAL)

# Check if std::experimental::optional exists
check_cxx_source_compiles("\
#include<experimental/optional>
using optional = std::experimental::optional<int>;
int main() {}" SUSHI_COMPILE_EXPERIMENTAL_OPTIONAL)

if(SUSHI_COMPILE_OPTIONAL)
    set(SUSHI_SUPPORT_STD_OPTIONAL 1)
elseif(SUSHI_COMPILE_EXPERIMENTAL_OPTIONAL)
    set(SUSHI_SUPPORT_EXPERIMENTAL_OPTIONAL 1)
endif()