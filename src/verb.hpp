#if !defined(VERB_HPP)

class stative_action;
class world;

enum class noun_type
// NOTE(tyler): To enumerate subject, object, and indirect object
{
    S,
    O,
    I,
};

struct verb
{
    std::string Lemma;

    bool operator==(const verb& Other) const;

    verb(std::string Lemma);
};

struct stative_verb : public verb
{
    stative_verb(std::string Lemma);
};

typedef std::pair<noun_type, stative_action> condition;
typedef std::map<noun_type, stative_action> conditions;
struct dynamic_verb : public verb
{
    conditions PreConditions;
    conditions PostConditions;

    bool CheckPreConditions(world& World, uint32_t sID, uint32_t oID, uint32_t iID);
    
    dynamic_verb(std::string Lemma);
};

#define VERB_HPP
#endif
