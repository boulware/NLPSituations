#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>

#include <set>
#include <map>
#include <algorithm>

#include "verb.cpp"

typedef uint32_t uint32;

enum class noun_type
// NOTE(tyler): To enumerate subject, object, and indirect object
{
    S,
    O,
    I,
};

struct object_data;
struct stative_verb;
struct stative_action;
class world;
class dynamic_verb;
class object;

struct object_data
{
    std::string Name;
};

struct stative_verb : public verb
{
    stative_verb(std::string Lemma) : verb(Lemma) {}
};

stative_verb vHave("has");

struct stative_action
{
    stative_verb Verb;
    uint32 TargetID;
    bool Active;

    stative_action(stative_verb Verb, uint32 TargetID, bool Active);
    
    bool operator==(const stative_action& Other) const;
    bool operator<(const stative_action& Other) const;
};

class object
{
private:
    std::set<stative_action> StativeActions;
protected:
    const uint32 ID;
    world& World;
public:
    const object_data Data;
    
    object();
    object(const object_data& Data, world& World, uint32 ID);
    
    void SetStativeActionState(bool State, stative_verb Verb, uint32 ObjectID);
    void PrintStativeActions() const;
    bool CheckStativeActionState(const stative_action& Action) const;
    bool CheckStativeActionState(const stative_verb& V, uint32 TargetID) const;

    bool Do(dynamic_verb& V, uint32 oID, uint32 iID);
};

class world
{
private:
    uint32 IDCounter;
    std::map<uint32, object> Objects;
public:
    world();
    // TODO(tyler): Obviously making these silly overloads for animate or inanimate objects is silly.
    // Much of this functionality will be described by object_data as something closely tied to the
    // semantic tree. This is a temporary work-around.
    uint32 CreateObject(const object_data& Data);
    object* GetObject(uint32 ID);
};


typedef std::pair<noun_type, stative_action> condition;
typedef std::map<noun_type, stative_action> conditions;
struct dynamic_verb : public verb
{
    conditions PreConditions;
    conditions PostConditions;

    bool CheckPreConditions(world& World, uint32 sID, uint32 oID = 0, uint32 iID = 0)
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
    
    dynamic_verb(std::string Lemma) : verb(Lemma) {}
};

stative_action::stative_action(stative_verb Verb, uint32 TargetID = 0, bool Active = true)
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

object::object(const object_data& Data, world& World, uint32 ID) : Data(Data), World(World), ID(ID)
{
}

world NullWorld;
object_data NullData = {"#NULL#"};
object::object() : Data(NullData), World(NullWorld), ID(0) {}

void object::SetStativeActionState(bool State, stative_verb Verb, uint32 ObjectID = 0)
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
bool object::CheckStativeActionState(const stative_verb& V, uint32 TargetID) const
{
        stative_action StativeAction(V, TargetID);
        return CheckStativeActionState(StativeAction);
    return true;
}

world::world() : IDCounter(1)
{
    
}

uint32 world::CreateObject(const object_data& Data)
{
    Objects.emplace(IDCounter, object(Data, *this, IDCounter));
    return IDCounter++;
}

object* world::GetObject(uint32 ID)
{
    std::map<uint32, object>::iterator It = Objects.find(ID);
    if(It != Objects.end())
    {
        return &((*It).second);
    }
    else return nullptr;
}
    
bool object::Do(dynamic_verb& V, uint32 oID = 0, uint32 iID = 0)
{
    uint32 sID = ID; // NOTE(tyler): For clarity of notation.
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
        
int main()
{
    std::cout.setf(std::ios::boolalpha);
    
    // NOTE(tyler): Window setup
    unsigned int WindowWidth = 1280;
    unsigned int WindowHeight = 720;
    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "");
    Window.setFramerateLimit(60);

    world MainWorld;
    
    object_data dCoin = {"coin"};
    object_data dHat = {"hat"};
    object_data dA = {"A"};
    object_data dB = {"B"};

    uint32 Hat = MainWorld.CreateObject(dHat);
    uint32 A = MainWorld.CreateObject(dA);
    uint32 B = MainWorld.CreateObject(dB);

//    MainWorld.GiveObject(dCoin, A, 2);
    
    stative_action HasHat(vHave, Hat, true);
    stative_action NotHasHat(vHave, Hat, false);
    
    dynamic_verb vRun("runs");
    dynamic_verb vGive("gives");
    
    vGive.PreConditions.insert(condition(noun_type::S, HasHat));
    vGive.PreConditions.insert(condition(noun_type::I, NotHasHat));
    vGive.PostConditions.insert(condition(noun_type::S, NotHasHat));
    vGive.PostConditions.insert(condition(noun_type::I, HasHat));

    MainWorld.GetObject(A)->SetStativeActionState(true, vHave, Hat);

    MainWorld.GetObject(A)->Do(vRun);
    MainWorld.GetObject(A)->Do(vGive, Hat, B);
    
/*    while(Window.isOpen())
    {
        sf::Event Event;
        while(Window.pollEvent(Event))
        {
            if(Event.type == sf::Event::Closed)
            {
                Window.close();
            }
        }
        
        Window.clear();

        //Window.draw(stuff);
        
        Window.display();
        }*/
}
