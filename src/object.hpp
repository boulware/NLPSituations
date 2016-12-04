#if !defined(OBJECT_HPP)

#include <cstdint>

#include "verb.hpp"

class world;

struct object_data
{
    std::string Name;
};
     
class object
{
private:
    std::set<stative_action> StativeActions;
protected:
    const uint32_t ID;
    world& World;
public:
    const object_data Data;
    
    object();
    object(const object_data& Data, world& World, uint32_t ID);
    
    void SetStativeActionState(bool State, stative_verb Verb, uint32_t ObjectID);
    void PrintStativeActions() const;
    bool CheckStativeActionState(const stative_action& Action) const;
    bool CheckStativeActionState(const stative_verb& V, uint32_t TargetID) const;

    bool Do(dynamic_verb& V, uint32_t oID, uint32_t iID);
};

#define OBJECT_HPP
#endif
