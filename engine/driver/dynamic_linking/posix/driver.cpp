#include "driver.hpp"

#include <dlfcn.h>

namespace sushi { namespace driver { namespace dl {

ObjectHandle open(const char* path) {
    return dlopen(path, RTLD_NOW | RTLD_LOCAL);
}

int close(ObjectHandle object) {
    return dlclose(object);
}

SymbolPtr get_symbol(ObjectHandle object, const char* name) {
    return dlsym(object, name);
}

}}}