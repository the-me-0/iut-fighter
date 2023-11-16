#include "anthonyfighter.h"

AnthonyFighter::AnthonyFighter()
    :
      Fighter(150,10,8,"anthony")
{
    itsCaracteristics = {5,12,20};
}

// Throw special attack
void AnthonyFighter::specialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()*2);
    setItsKickAttackCP(getItsKickAttackCP()*2);
}

// Remove special attack effects
void AnthonyFighter::removeSpecialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()/2);
    setItsKickAttackCP(getItsKickAttackCP()/2);
}
