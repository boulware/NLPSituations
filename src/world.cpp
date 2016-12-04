#include "world.hpp"

world::world() : IDCounter(1)
{
    
}

uint32_t world::CreateObject(const object_data& Data)
{
    Objects.emplace(IDCounter, object(Data, *this, IDCounter));
    return IDCounter++;
}

uint32_t world::CreateObjects(const object_data& Data, int Count)
{
    uint32_t InitialID = IDCounter;
    for(int i = 0; i < Count; i++) CreateObject(Data);
    return InitialID;
}

object* world::GetObject(uint32_t ID)
{
    std::map<uint32_t, object>::iterator It = Objects.find(ID);
    if(It != Objects.end())
    {
        return &((*It).second);
    }
    else return nullptr;
}
