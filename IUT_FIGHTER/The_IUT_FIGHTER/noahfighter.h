#ifndef NOAHFIGHTER_H
#define NOAHFIGHTER_H

#include "fighter.h"

class NoahFighter : public Fighter
{
public:
    NoahFighter();

    // Fighter interface
public:
    void specialAttack();
    void removeSpecialAttack();
};

#endif // NOAHFIGHTER_H
