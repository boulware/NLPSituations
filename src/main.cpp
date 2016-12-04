#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>

#include <set>
#include <map>
#include <algorithm>

#include "verb.cpp"
#include "world.cpp"
#include "stative_action.cpp"
#include "object.cpp"
   
int main()
{
    std::cout.setf(std::ios::boolalpha);
    
    // NOTE(tyler): Window setup
    unsigned int WindowWidth = 1280;
    unsigned int WindowHeight = 720;
    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "");
    Window.setFramerateLimit(60);

    stative_verb vHave("has");
    stative_verb vBe("is");
    dynamic_verb vRun("runs");
    dynamic_verb vGive("gives");
    dynamic_verb vInsert("insert");
    
    world MainWorld;
    
    object_data dCoin = {"coin"};
    object_data dPouch = {"pouch"};
    object_data dHat = {"hat"};
    object_data dA = {"A"};
    object_data dB = {"B"};

    uint32_t Hat = MainWorld.CreateObject(dHat);
    uint32_t Coin = MainWorld.CreateObject(dCoin);
    uint32_t CoinBag = MainWorld.CreateObject(dPouch);
    uint32_t A = MainWorld.CreateObject(dA);
    uint32_t B = MainWorld.CreateObject(dB);
    
    stative_action HasHat(vHave, Hat, true);
    stative_action NotHasHat(vHave, Hat, false);

//    stative_action IsContainer(vBe, 
    
    vGive.PreConditions.insert(condition(noun_type::S, HasHat));
    vGive.PreConditions.insert(condition(noun_type::I, NotHasHat));
    vGive.PostConditions.insert(condition(noun_type::S, NotHasHat));
    vGive.PostConditions.insert(condition(noun_type::I, HasHat));

//    vInsert.PreCondition.insert(condition

    MainWorld.GetObject(A)->SetStativeActionState(true, vHave, Hat);

    MainWorld.GetObject(A)->Do(vRun);
    MainWorld.GetObject(A)->Do(vGive, Hat, B);
    MainWorld.GetObject(B)->Do(vGive, Hat, A);
    
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
