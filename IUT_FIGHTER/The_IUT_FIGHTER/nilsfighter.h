#ifndef NILSFIGHTER_H
#define NILSFIGHTER_H

#include "fighter.h"

class NilsFighter : public Fighter
{
public:
    NilsFighter();

    // Fighter interface
public:
    void specialAttack();
    void removeSpecialAttack();

};

#endif // NILSFIGHTER_H
