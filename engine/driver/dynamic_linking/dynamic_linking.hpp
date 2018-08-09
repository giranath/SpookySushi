#ifndef SPOOKYSUSHI_DRIVER_DYNLINK_HPP
#define SPOOKYSUSHI_DRIVER_DYNLINK_HPP

namespace sushi { namespace driver { namespace dl {

using ObjectHandle = void*;
using SymbolPtr = void*;

ObjectHandle open(const char* path);
int close(ObjectHandle object);

SymbolPtr get_symbol(ObjectHandle object, const char* name);

const char* get_last_error_msg();

}}}

#endif
