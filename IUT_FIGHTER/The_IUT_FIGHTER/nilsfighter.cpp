#include "nilsfighter.h"

NilsFighter::NilsFighter()
    :
      Fighter(100,3,15,"Nils")
{
    itsCaracteristics = {5,12,20};
}

// Throw special attack
void NilsFighter::specialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()*2);
    setItsKickAttackCP(getItsKickAttackCP()*2);
}

// Remove special attack effects
void NilsFighter::removeSpecialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()/2);
    setItsKickAttackCP(getItsKickAttackCP()/2);
}
