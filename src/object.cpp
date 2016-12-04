#include "object.hpp"

object::object(const object_data& Data, world& World, uint32_t ID) : Data(Data), World(World), ID(ID)
{
}

world NullWorld;
object_data NullData = {"#NULL#"};
object::object() : object(NullData, NullWorld, 0) {}

void object::SetStativeActionState(bool State, stative_verb Verb, uint32_t ObjectID = 0)
// NOTE(tyler): We'll assume all elements of StativeActions are in the "on" state.
{
    stative_action Action(Verb, ObjectID);
        
    if(State == true)
    {
        StativeActions.insert(Action);
    }
    else
    {
        std::set<stative_action>::iterator A = StativeActions.find(Action);

        if(A != StativeActions.end())
        {
            StativeActions.erase(A);
        }   
    }
        
}

void object::PrintStativeActions() const
{
    for(auto& e : StativeActions)
    {
        std::cout << ">>" << Data.Name << " " << e.Verb.Lemma << " " << World.GetObject(e.TargetID)->Data.Name << "\n";
    }
}    

bool object::CheckStativeActionState(const stative_action& Action) const
{
    for(auto& e : StativeActions)
    {
        if(Action == e) return true;
    }
    return false;
}
bool object::CheckStativeActionState(const stative_verb& V, uint32_t TargetID) const
{
        stative_action StativeAction(V, TargetID);
        return CheckStativeActionState(StativeAction);
    return true;
}

 
bool object::Do(dynamic_verb& V, uint32_t oID = 0, uint32_t iID = 0)
{
    uint32_t sID = ID; // NOTE(tyler): For clarity of notation.
    if(V.CheckPreConditions(World, sID, oID, iID))
    {            
        std::cout << World.GetObject(sID)->Data.Name << " " << V.Lemma;
        if(oID != 0) std::cout << " " << World.GetObject(oID)->Data.Name;
        if(iID != 0) std::cout << " to " << World.GetObject(iID)->Data.Name;
        std::cout << ".\n";

        std::pair<conditions::iterator, conditions::iterator> Range;
        Range = V.PostConditions.equal_range(noun_type::S);
        for(conditions::iterator i = Range.first; i != Range.second; i++)
        {
            World.GetObject(sID)->SetStativeActionState(i->second.Active, i->second.Verb, i->second.TargetID);
            std::cout << ">set " << i->second.Active << ": " << World.GetObject(sID)->Data.Name << " " << i->second.Verb.Lemma << " " << World.GetObject(i->second.TargetID)->Data.Name << "\n";
        }
        if(oID != 0)
        {
            Range = V.PostConditions.equal_range(noun_type::O);
            for(conditions::iterator i = Range.first; i != Range.second; i++)
            {
                World.GetObject(oID)->SetStativeActionState(i->second.Active, i->second.Verb, i->second.TargetID);
                std::cout << ">set " << i->second.Active << ": " << World.GetObject(oID)->Data.Name << " " << i->second.Verb.Lemma << " " << World.GetObject(i->second.TargetID)->Data.Name << "\n";
            }                
        }
        if(iID != 0)
        {
            Range = V.PostConditions.equal_range(noun_type::I);
            for(conditions::iterator i = Range.first; i != Range.second; i++)
            {
                World.GetObject(iID)->SetStativeActionState(i->second.Active, i->second.Verb, i->second.TargetID);
                std::cout << ">set " << i->second.Active << ": " << World.GetObject(iID)->Data.Name << " " << i->second.Verb.Lemma << " " << World.GetObject(i->second.TargetID)->Data.Name << "\n";
            }                
        }

        // NOTE(tyler): print post-results of S, O, and I
        World.GetObject(sID)->PrintStativeActions();
        if(oID != 0) World.GetObject(oID)->PrintStativeActions();
        if(iID != 0) World.GetObject(iID)->PrintStativeActions();
            
            
        return true;
    }

    return false;
}
   
