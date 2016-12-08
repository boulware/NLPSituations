#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>

#include <set>
#include <map>
#include <algorithm>
#include <string>

#include "tokenize_file.cpp"

#include "verb.cpp"
#include "world.cpp"
#include "stative_action.cpp"
#include "object.cpp"

enum class relationship_direction
{
    out,
    in,
};

enum class relationship_type
{
    is_a,
};

struct node_relationship
{
    std::string TargetNodeLabel;
    relationship_direction Direction;
    std::string Type;
};

class node
{
    std::string Label;
    std::vector<node_relationship> Relationships;
public:
    node(std::string Label);
    node();

    void AddRelationship(const node_relationship& Relationship)
    {
        Relationships.push_back(Relationship);
    }
};

node::node(std::string Label) : Label(Label)
{
}

node::node() : node("$UNNAMED$")
{
}

class tree
{
    // NOTE(tyler): Node labels must be unique for now. May need to change later.
    std::map<std::string, node> Nodes;
public:
    tree(std::string FileName);
};

tree::tree(std::string FileName)
{
    std::vector<std::string> TreeTokens = xish::TokenizeFile("data/hypo_tree.sif", xish::token_mode::delimiter, '\t');

    for(int i = 0; i < TreeTokens.size() / 3; i++)
    {
        std::string SourceName = TreeTokens[i];
        std::string RelationshipName = TreeTokens[i+1];
        std::string TargetName = TreeTokens[i+2];
        
        // NOTE(tyler): Add the 1st or 3rd element of the 3-pair (which are of the form "word relationship word"),
        // if not already a node in the tree.
        std::map<std::string, node>::iterator it_start = Nodes.find(SourceName);
        std::map<std::string, node>::iterator it_end = Nodes.find(TargetName);
        if(it_start != Nodes.end()) Nodes[SourceName] = node(SourceName);
        if(it_end != Nodes.end()) Nodes[TargetName] = node(TargetName);

        node_relationship SourceRelationship = {TargetName, relationship_direction::out, RelationshipName};
        node_relationship TargetRelationship = {SourceName, relationship_direction::in, RelationshipName};

        Nodes[SourceName].AddRelationship(SourceRelationship);
        Nodes[TargetName].AddRelationship(TargetRelationship);
    }
}

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
