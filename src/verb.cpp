#include "verb.hpp"
#include "world.hpp"
#include "stative_action.hpp"

// verb
bool verb::operator==(const verb& Other) const
{
    if(Lemma == Other.Lemma) return true;
    return false;
}

verb::verb(std::string Lemma) : Lemma(Lemma) {}

// stative_verb
stative_verb::stative_verb(std::string Lemma) : verb(Lemma) {}

// dynamic_verb
bool dynamic_verb::CheckPreConditions(world& World, uint32_t sID, uint32_t oID = 0, uint32_t iID = 0)
{
    std::pair<conditions::iterator, conditions::iterator> Range;
    Range = PreConditions.equal_range(noun_type::S);
    for(conditions::iterator i = Range.first; i != Range.second; i++)
    {
        if(World.GetObject(sID)->CheckStativeActionState(i->second) != i->second.Active) return false;
    }
    if(oID != 0)
    {
        Range = PreConditions.equal_range(noun_type::O);
        for(conditions::iterator i = Range.first; i != Range.second; i++)
        {
            if(World.GetObject(oID)->CheckStativeActionState(i->second) != i->second.Active) return false;
        }
    }
    if(iID != 0)
    {
        Range = PreConditions.equal_range(noun_type::I);
        for(conditions::iterator i = Range.first; i != Range.second; i++)
        {
            if(World.GetObject(iID)->CheckStativeActionState(i->second) != i->second.Active) return false;
        }            
    }

    return true;
}
    
dynamic_verb::dynamic_verb(std::string Lemma) : verb(Lemma) {}
