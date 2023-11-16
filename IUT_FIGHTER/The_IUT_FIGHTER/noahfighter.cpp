#include "noahfighter.h"

NoahFighter::NoahFighter()
    :
      Fighter(120,7,11,"NOAH")
{
    itsCaracteristics = {5,12,20};
}

// Throw special attack
void NoahFighter::specialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()*2);
    setItsKickAttackCP(getItsKickAttackCP()*2);
}

// Remove special attack effects
void NoahFighter::removeSpecialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()/2);
    setItsKickAttackCP(getItsKickAttackCP()/2);
}
