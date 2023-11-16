#include "rock.h"

Rock::Rock(int aXPosition,
           QGraphicsScene *scene,
           Fighter *firstFighter,
           Fighter *secondFighter,
           std::vector<Rock *> *rockVector)
    :
      itsScene(scene),
      itsFirstFighter(firstFighter),
      itsSecondFighter(secondFighter),
      itsVector(rockVector)
{
    /* sets the rock position */
    setX(aXPosition);
    setY(-1080/2);

    /* sets the pixmap & hitbox of the rock */
    itsPixmap = QPixmap(":/rock1.png").scaled(150,212);
    setPixmap(itsPixmap);
    itsHitBox = new QGraphicsRectItem(0,0,120,190);
    itsHitBox->setParentItem(this);
    itsHitBox->setVisible(false);

    /* this state is used to know if a rock has to be paused too when the game is paused */
    /* -a rock isn't paused if it touched the ground- */
    itsState = RockState::FALLING;

    /* connecting a timer to update the rock's animation pixmap */
    connect(itsUpdateTimer,&QTimer::timeout,this,&Rock::update);
    itsUpdateTimer->start(20);

    /* start the falling animation */
    yAnimation->setStartValue(y()-200);
    yAnimation->setEndValue(270);
    yAnimation->setEasingCurve(QEasingCurve::Linear);
    yAnimation->setDuration(1000);
    yAnimation->start();
}

/* destructor */
Rock::~Rock()
{
    delete itsHitBox;
}

/* pause method -stops the animation if not already done- */
void Rock::pauseRockAnimation()
{
    if (yAnimation->state() == QPropertyAnimation::Running) yAnimation->pause();
}

/* resume method -resume the animation if not already done- */
void Rock::resumeRockAnimation()
{
    if (yAnimation->state() == QPropertyAnimation::Paused) yAnimation->resume();
}

/* updates the rock pixmap animation when it touches the floor */
void Rock::update()
{
    hitFighters();

    if((y() >= 270) && (itsState == RockState::FALLING)) {
        itsState = RockState::CRASHED;
        itsPixmap = QPixmap(":/rock2.png").scaled(150,212);
        setPixmap(itsPixmap);
        setPos(x(),290-57);
        delete itsHitBox;
        itsHitBox = new QGraphicsRectItem(x(),-1080/2,0,0);
        QTimer::singleShot(500,[=](){
            itsPixmap = QPixmap(":/rock3.png").scaled(150,212);
            setPixmap(itsPixmap);
            setPos(x(),290-47);
        });
        QTimer::singleShot(1000,[=](){
            toBeDeleted = true;
        });
    }
}

/* checks if a fighter is hit -verified at each update- */
void Rock::hitFighters()
{
    /* first fighter verification */
    if((itsHitBox->mapToScene(
            itsHitBox->boundingRect()).intersects(
            itsFirstFighter->getItsHitBoxBody()->mapToScene(
                itsFirstFighter->getItsHitBoxBody()->boundingRect()))) &&
            !hasTouchedFirstFighter) {
        itsFirstFighter->setItsAnimationState(KNOCKOUT);
        itsFirstFighter->tank(9);
        hasTouchedFirstFighter = true;
    }

    /* first fighter verification */
    if((itsHitBox->mapToScene(
            itsHitBox->boundingRect()).intersects(
            itsSecondFighter->getItsHitBoxBody()->mapToScene(
                itsSecondFighter->getItsHitBoxBody()->boundingRect()))) &&
            !hasTouchedSecondFighter) {
        itsSecondFighter->setItsAnimationState(KNOCKOUT);
        itsSecondFighter->tank(9);
        hasTouchedSecondFighter = true;
    }
}

/* returns the deletion boolean -is this rock good to be deleted as he finished its animation or didn't- */
bool Rock::toDelete()
{
    return toBeDeleted;
}

/* sets the deletion to true -it means the fightscene can now delete this rock, as the animation is done- */
void Rock::setToDelete(bool absolutelyYes)
{
    toBeDeleted = absolutelyYes;
}
