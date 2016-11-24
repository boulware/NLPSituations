#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include <set>
#include <algorithm>

class object;

struct verb
{
    std::string Lemma;

    bool operator==(const verb& Other) const
    {
        if(Lemma == Other.Lemma) return true;
        return false;
    }

    verb(std::string Lemma) : Lemma(Lemma) {}
};

struct stative_verb : public verb
{
    stative_verb(std::string Lemma) : verb(Lemma) {}
};

struct stative_action
{
public:
    stative_verb Verb;
    object* Target; // NOTE(tyler): Really, this could be a phrase like "all people", "some restaurants",
                    // so is this really an "object"?
    bool Active;

    bool operator==(const stative_action& Other) const
    {
        if(Verb == Other.Verb &&
           Target == Other.Target)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool operator<(const stative_action& Other) const
    {
        return Target < Other.Target;
    }
    
    stative_action(stative_verb Verb, object* Target = nullptr, bool Active = true) : Verb(Verb), Target(Target), Active(Active) {}
};


class object
{
private:
    std::set<stative_action> StativeActions;
public:
    void SetStativeActionState(bool State, stative_verb Verb, object* Object = nullptr)
    // NOTE(tyler): We'll assume all elements of StativeActions are in the "on" state.
    {
        stative_action Action(Verb, Object);
        
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

    void PrintStativeActions()
    {
        for(auto& e : StativeActions)
        {
            std::cout << ">>" << this->Name << " " << e.Verb.Lemma << " " << e.Target->Name << "\n";
        }
    }    
public:
    bool CheckStativeAction(stative_action Action) const
    {
        for(auto& e : StativeActions)
        {
            if(Action == e) return true;
        }
        return false;
    }
    
    std::string Name;
    object(std::string Name) : Name(Name) {}
};

enum class noun_type
// NOTE(tyler): To enumerate subject, object, and indirect object
{
    S,
    O,
    I,
};

typedef std::pair<noun_type, stative_action> condition;
typedef std::map<noun_type, stative_action> conditions;
struct dynamic_verb : public verb
{
    conditions PreConditions;
    conditions PostConditions;

    bool CheckPreConditions(object& S, object* O = nullptr, object* I = nullptr)
    {
        std::pair<conditions::iterator, conditions::iterator> Range;
        Range = PreConditions.equal_range(noun_type::S);
        for(conditions::iterator i = Range.first; i != Range.second; i++)
        {
            if(S.CheckStativeAction(i->second) != i->second.Active) return false;
        }
        if(O != nullptr)
        {
            Range = PreConditions.equal_range(noun_type::O);
            for(conditions::iterator i = Range.first; i != Range.second; i++)
            {
                if(O->CheckStativeAction(i->second) != i->second.Active) return false;
            }
        }
        if(I != nullptr)
        {
            Range = PreConditions.equal_range(noun_type::I);
            for(conditions::iterator i = Range.first; i != Range.second; i++)
            {
                if(I->CheckStativeAction(i->second) != i->second.Active) return false;
            }            
        }

        return true;
    }

    bool Do(object& S, object* O = nullptr, object* I = nullptr)
    {
        if(CheckPreConditions(S, O, I))
        {            
            std::cout << S.Name << " " << Lemma;
            if(O != nullptr) std::cout << " " << O->Name;
            if(I != nullptr) std::cout << " to " << I->Name;
            std::cout << ".\n";

            std::pair<conditions::iterator, conditions::iterator> Range;
            Range = PostConditions.equal_range(noun_type::S);
            for(conditions::iterator i = Range.first; i != Range.second; i++)
            {
                S.SetStativeActionState(i->second.Active, i->second.Verb, i->second.Target);
                std::cout << ">set " << i->second.Active << ": " << S.Name << " " << i->second.Verb.Lemma << " " << i->second.Target->Name << "\n";
            }
            if(O != nullptr)
            {
                Range = PostConditions.equal_range(noun_type::O);
                for(conditions::iterator i = Range.first; i != Range.second; i++)
                {
                    O->SetStativeActionState(i->second.Active, i->second.Verb, i->second.Target);
                    std::cout << ">set " << i->second.Active << ": " << O->Name << " " << i->second.Verb.Lemma << " " << i->second.Target->Name << "\n";
                }                
            }
            if(I != nullptr)
            {
                Range = PostConditions.equal_range(noun_type::I);
                for(conditions::iterator i = Range.first; i != Range.second; i++)
                {
                    I->SetStativeActionState(i->second.Active, i->second.Verb, i->second.Target);
                    std::cout << ">set " << i->second.Active << ": " << I->Name << " " << i->second.Verb.Lemma << " " << i->second.Target->Name << "\n";
                }                
            }

            // NOTE(tyler): print post-results of S, O, and I
            S.PrintStativeActions();
            if(O != nullptr) O->PrintStativeActions();
            if(I != nullptr) I->PrintStativeActions();
            
            
            return true;
        }

        return false;
    }
    
    dynamic_verb(std::string Lemma) : verb(Lemma) {}
};

// TODO(tyler): Move to somewhere else.
//stative_verb vHave("have");
//stative_verb vGive("give");

/*
   // pre-conditions:
        // 1: "S has O"
        // 2: "I does not have O"
        stative_action pre1(ToHave, &O);
        stative_action pre2(ToHave, &O);
*/


int main()
{
    // NOTE(tyler): Window setup
    unsigned int WindowWidth = 1280;
    unsigned int WindowHeight = 720;
    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "");
    Window.setFramerateLimit(60);

    object Coin("coin");
    object Hat("hat");
   
    object A("A");
    object B("B");
    
    stative_verb vHave("has");
    
    stative_action HasHat(vHave, &Hat, true);
    stative_action NotHasHat(vHave, &Hat, false);
    
    dynamic_verb vRun("runs");
    dynamic_verb vGive("gives");
    
    vGive.PreConditions.insert(condition(noun_type::S, HasHat));
    vGive.PreConditions.insert(condition(noun_type::I, NotHasHat));
    vGive.PostConditions.insert(condition(noun_type::S, NotHasHat));
    vGive.PostConditions.insert(condition(noun_type::I, HasHat));

    A.SetStativeActionState(true, vHave, &Hat);
    B.SetStativeActionState(false, vHave, &Hat);

    vRun.Do(A);
    vGive.Do(A, &Hat, &B);
    
    


    
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
