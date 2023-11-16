#include "cyrilfighter.h"

CyrilFighter::CyrilFighter()
    :
      Fighter(110,9,9,"CYRIL")
{
    itsCaracteristics = {5,12,20};
}

// Throw special attack
void CyrilFighter::specialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()*2);
    setItsKickAttackCP(getItsKickAttackCP()*2);
}

// Remove special attack effects
void CyrilFighter::removeSpecialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()/2);
    setItsKickAttackCP(getItsKickAttackCP()/2);
}
