#include "../dynamic_linking.hpp"

namespace sushi { namespace driver { namespace dl {

ObjectHandle open(const char* path) {
    return nullptr;
}

int close(ObjectHandle object) {
    return -1;
}

SymbolPtr get_symbol(ObjectHandle object, const char* name) {
    return nullptr;
}

const char* get_last_error_msg() {
    return "";
}

}}}