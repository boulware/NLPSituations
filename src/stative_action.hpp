#if !defined(STATIVE_ACTION_HPP)

struct stative_verb;

struct stative_action
{
    stative_verb Verb;
    uint32_t TargetID;
    bool Active;

    stative_action(stative_verb Verb, uint32_t TargetID, bool Active);
    
    bool operator==(const stative_action& Other) const;
    bool operator<(const stative_action& Other) const;
};

#define STATIVE_ACTION_HPP
#endif
