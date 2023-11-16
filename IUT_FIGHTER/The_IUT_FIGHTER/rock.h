#ifndef ROCK_H
#define ROCK_H

#include "fighter.h"
#include "properties.h"

class Rock : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX) // not necessary
    Q_PROPERTY(qreal y READ y WRITE setY)

private:
    QPixmap itsPixmap;
    QGraphicsRectItem * itsHitBox;

    QGraphicsScene * itsScene;

    bool endedAnimation = false;

    Fighter * itsFirstFighter = nullptr;
    Fighter * itsSecondFighter = nullptr;

    RockState itsState;

    std::vector<Rock *> *itsVector;

    bool toBeDeleted = false;

    QTimer * itsUpdateTimer = new QTimer(this);

    bool hasTouchedFirstFighter = false;
    bool hasTouchedSecondFighter = false;

    QPropertyAnimation *yAnimation = new QPropertyAnimation(this,"y",this);

private slots:
    void update();

public:
    Rock(int aXPosition,
         QGraphicsScene *scene,
         Fighter *firstFighter,
         Fighter *secondFighter,
         std::vector<Rock *> *rockVector);
    ~Rock();

    void pauseRockAnimation();
    void resumeRockAnimation();
    void hitFighters();

    bool toDelete();

    void setToDelete(bool absolutelyYes);
};

#endif // ROCK_H
