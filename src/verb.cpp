#include "verb.hpp"

bool verb::operator==(const verb& Other) const
{
    if(Lemma == Other.Lemma) return true;
    return false;
}

verb::verb(std::string Lemma) : Lemma(Lemma) {}
