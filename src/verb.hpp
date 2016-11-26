#if !defined(VERB_HPP)

struct verb
{
    std::string Lemma;

    bool operator==(const verb& Other) const;

    verb(std::string Lemma);
};

#define VERB_HPP
#endif
