#ifndef ANTHONYFIGHTER_H
#define ANTHONYFIGHTER_H

#include "fighter.h"

class AnthonyFighter : public Fighter
{
public:
    AnthonyFighter();

    // Fighter interface
public:
    void specialAttack();
    void removeSpecialAttack();
};

#endif // ANTHONYFIGHTER_H
