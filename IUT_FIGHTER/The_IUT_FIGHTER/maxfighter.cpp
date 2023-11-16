#include "maxfighter.h"

MaxFighter::MaxFighter()
    :
      Fighter(85,12,6,"Maxime")
{
    itsCaracteristics = {5,12,20};
}

// Throw special attack
void MaxFighter::specialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()*2);
    setItsKickAttackCP(getItsKickAttackCP()*2);
}

// Remove special attack effects
void MaxFighter::removeSpecialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()/2);
    setItsKickAttackCP(getItsKickAttackCP()/2);
}
