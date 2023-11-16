#ifndef ARTHURFIGHTER_H
#define ARTHURFIGHTER_H

#include "fighter.h"

class ArthurFighter : public Fighter
{
public:
    ArthurFighter();

    // Fighter interface
public:
    void specialAttack();
    void removeSpecialAttack();
};

#endif // ARTHURFIGHTER_H
