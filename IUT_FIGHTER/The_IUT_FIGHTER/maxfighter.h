#ifndef MAXFIGHTER_H
#define MAXFIGHTER_H

#include "fighter.h"

class MaxFighter : public Fighter
{

public:
    MaxFighter();

    // Fighter interface
public:
    void specialAttack();
    void removeSpecialAttack();

};

#endif // MAXFIGHTER_H
