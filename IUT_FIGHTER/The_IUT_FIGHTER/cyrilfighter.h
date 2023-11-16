#ifndef CYRILFIGHTER_H
#define CYRILFIGHTER_H

#include "fighter.h"

class CyrilFighter : public Fighter
{
public:
    CyrilFighter();

    // Fighter interface
public:
    void specialAttack();
    void removeSpecialAttack();
};

#endif // CYRILFIGHTER_H
