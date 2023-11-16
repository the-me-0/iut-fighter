#include "arthurfighter.h"

ArthurFighter::ArthurFighter()
    :
      Fighter(100,11,7,"arthur")
{
    itsCaracteristics = {5,12,20};
}

// Throw special attack
void ArthurFighter::specialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()*2);
    setItsKickAttackCP(getItsKickAttackCP()*2);
}

// Remove special attack effects
void ArthurFighter::removeSpecialAttack()
{
    setItsPunchAttackCP(getItsPunchAttackCP()/2);
    setItsKickAttackCP(getItsKickAttackCP()/2);
}
