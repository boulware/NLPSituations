#include "stative_action.hpp"

#include "verb.hpp"

stative_action::stative_action(stative_verb Verb, uint32_t TargetID = 0, bool Active = true)
        : Verb(Verb), TargetID(TargetID), Active(Active)
{   
}

bool stative_action::operator==(const stative_action& Other) const
{
    if(Verb == Other.Verb &&
       TargetID == Other.TargetID)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool stative_action::operator<(const stative_action& Other) const
{
    return TargetID < Other.TargetID;
}
