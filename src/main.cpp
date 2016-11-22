#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include <set>
#include <algorithm>

class object
{
public:
    std::string Name;
    object(std::string Name) : Name(Name) {}
};

enum class stative_verb
{
    have,
};

struct stative_action
/* Examples:
   "I hate bananas"
   "I own a car"
   "I have a pen"
   "I know him"
 */
{   
    stative_verb Verb;
    object& Target; // NOTE(tyler): Really, this could be a phrase like "all people", "some restaurants",
                   // so is this really an "object"?

    bool operator==(const stative_action& Other) const
    {
        if(Verb == Other.Verb &&
           &Target == &Other.Target)
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
        return &Target < &Other.Target;
    }
    
    stative_action(stative_verb Verb, object& Target) : Verb(Verb), Target(Target) {}
};

class person
{
public:
    std::string Name;
    
    bool CheckStativeAction(stative_action Action) const
    {
        for(auto& e : StativeActions)
        {
            if(Action == e) return true;
        }
        return false;
    }
public:
    std::set<stative_action> StativeActions;

    void SetStativeAction(stative_action Action, bool State)
    // NOTE(tyler): We'll assume all elements of StativeActions are in the "on" state.
    {
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

    void Give(object& O, person& I)
    {
        // pre-conditions:
        // 1: "S has O"
        // 2: "I does not have O"
        stative_action pre1(stative_verb::have, O);
        stative_action pre2(stative_verb::have, O);
        if(CheckStativeAction(pre1) == true &&
           I.CheckStativeAction(pre2) == false)
        {
            // post-conditions:
            // 1: "S does not have O"
            // 2: "I has O"
            SetStativeAction(pre1, false);
            I.SetStativeAction(pre2, true);

            std::cout << Name << " gives " << O.Name << " to " << I.Name << "\n";
        }
    }
    
    person(std::string Name) : Name(Name) {}
};


int main()
{
    // NOTE(tyler): Window setup
    unsigned int WindowWidth = 1280;
    unsigned int WindowHeight = 720;
    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "");
    Window.setFramerateLimit(60);

    object Hat("hat");
    stative_action HasHat(stative_verb::have, Hat);

    person A("A");
    person B("B");
    A.SetStativeAction(HasHat, true);
    B.SetStativeAction(HasHat, false);  

    A.Give(Hat, B);
    B.Give(Hat, A);
    
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
