#if !defined(WORLD_HPP)

#include <cstdint>

#include "object.hpp"

class world
{
private:
    uint32_t IDCounter;
    std::map<uint32_t, object> Objects;
public:
    world();
    // TODO(tyler): Obviously making these silly overloads for animate or inanimate objects is silly.
    // Much of this functionality will be described by object_data as something closely tied to the
    // semantic tree. This is a temporary work-around.
    uint32_t CreateObject(const object_data& Data);
    uint32_t CreateObjects(const object_data& Data, int Count);
    object* GetObject(uint32_t ID);
};

#define WORLD_HPP
#endif
