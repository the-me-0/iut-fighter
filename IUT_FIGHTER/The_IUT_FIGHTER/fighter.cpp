#include "fighter.h"


Fighter::Fighter(int baseHP,
                 int punchAttackCP,
                 int kickAttackCP,
                 const QString &itsName,
                 int powerNeed)
    :
      itsBaseHP(baseHP),
      itsHP(baseHP),
      itsPunchAttackCP(punchAttackCP),
      itsKickAttackCP(kickAttackCP),
      itsPowerNeed(powerNeed),
      itsName(itsName)
{
    /* Initialize pixmap */
    QString fileTitle = ":/" + itsName + '_' + AnimationStatesToStr(itsAnimationState) + "1.png";
    itsPixmap = QPixmap(fileTitle).scaled(QSize(268,420));
    setPixmap(itsPixmap);

    groundPosition = scenePos().y(); // = 0


    /* Hitbox Setting */
    itsHitbox = new QGraphicsItemGroup(this);

    /* Hitbox rect surface for calibration */
    itsHitBoxRectSurface = new QGraphicsRectItem(0,0,itsPixmap.width(),itsPixmap.height());
    itsHitBoxRectSurface->setBrush(Qt::green);
    itsHitBoxRectSurface->setOpacity(0.2);
    itsHitbox->addToGroup(itsHitBoxRectSurface);

    /* Hitbox body */
    itsHitBoxBody = new QGraphicsRectItem(0,0,30*hitboxSizeRatio,45*hitboxSizeRatio);
    itsHitBoxBody->setBrush(Qt::yellow);
    itsHitBoxBody->setOpacity(0.5);
    itsHitBoxBody->setPos(pos() +
                          QPointF(itsHitBoxRectSurface->boundingRect().width()/2,
                                  itsHitBoxRectSurface->boundingRect().height()/2) -
                          QPointF(itsHitBoxBody->boundingRect().width()/2,
                                  itsHitBoxBody->boundingRect().height()/1.5));

    itsHitbox->addToGroup(itsHitBoxBody);

    /* Hitbox leg */
    itsHitBoxLeg = new QGraphicsRectItem(0,0,15*hitboxSizeRatio,45*hitboxSizeRatio);
    itsHitBoxLeg->setBrush(Qt::yellow);
    itsHitBoxLeg->setOpacity(0.5);
    itsHitbox->addToGroup(itsHitBoxLeg);
    itsHitBoxLeg->setRotation(-35);
    itsHitBoxLeg->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width()/2.5,
                                 itsHitBoxRectSurface->boundingRect().height()/1.8));
    hitboxLegOrigiPos = itsHitBoxLeg->pos();

    /* Hitbox arm */
    itsHitBoxArm = new QGraphicsRectItem(0,0,10*hitboxSizeRatio,40*hitboxSizeRatio);
    itsHitBoxArm->setBrush(Qt::yellow);
    itsHitBoxArm->setOpacity(0.5);
    itsHitBoxArm->setRotation(-90);
    itsHitBoxArm->setPos(pos() + QPointF(itsHitBoxRectSurface->boundingRect().width()/4,
                                         itsHitBoxRectSurface->boundingRect().height()/3));

    hitboxArmOrigiPos = itsHitBoxArm->pos();

    itsHitbox->addToGroup(itsHitBoxArm);

    /* Hitbox head */
    itsHitBoxHead = new QGraphicsRectItem(0,0,15*hitboxSizeRatio,10*hitboxSizeRatio);
    itsHitBoxHead->setBrush(Qt::yellow);
    itsHitBoxHead->setOpacity(0.5);
    itsHitBoxHead->setPos(pos() + QPointF(itsHitBoxRectSurface->boundingRect().width()/2,
                                          itsHitBoxRectSurface->boundingRect().height()/7));
    itsHitbox->addToGroup(itsHitBoxHead);

    connect(itsUpdateHitboxTimer,&QTimer::timeout, this, &Fighter::updateHitbox);
    itsUpdateHitboxTimer->start(20);

    connect(itsAnimTimer,&QTimer::timeout, this, &Fighter::updateAnimation);
    itsAnimTimer->start(100);


    if(!showHitbox){
        itsHitbox->setOpacity(0);
    }
}


/* Walk toward direction */
void Fighter::walk()
{
    // Lock walk if the fighter is crouching
    if(itsAnimationState != CROUCH && itsAnimationState != PUNCHCROUCH && itsAnimationState != KNOCKOUT)
    {
        //if going right then its positive else its negative to move left
        int direction = 1;

        /*When we are pressing both left and right at the same time
        we are moving away from the enemy
    Then if enemy on the right (faceRight) we prefer to move to left
        same thing the other way
    Handle by doing the if case about the face(left/right) in a different order*/

        //faceRight then moving left function first
        switch (itsDirection) {

        case FACERIGHT:
            if(isGoingLeft)
                direction = -1;
            else if(isGoingRight)
                direction = 1;
            break;
            //into moving right after
        case FACELEFT:
            if(isGoingRight)
                direction = 1;
            else if(isGoingLeft)
                direction = -1;
            break;
        }

        /*Left border :
        If less than the border stop the character
        but if its going right (to go back to the field) it can now moove*/
        if(x() <= -3702/2 && isGoingLeft){
            direction = 0;
        }
        // (!A != !B) to have a XOR door
        // if left is pressed you can't moove further you must only use the other direction
        else if(x() <= -3702/2 && (!isGoingRight != !isGoingLeft) && (!isGoingLeft)){
            direction = 1;
        }

        //Same for the right border but reverse
        if(x() >= 3702/2-boundingRect().width() && isGoingRight){
            direction = 0;
        }
        else if(x() >= 3702/2-boundingRect().width() && (!isGoingRight != !isGoingLeft) && !isGoingRight){
            direction = -1;
        }

        setItsAnimationState(WALKING);

        qreal curPosX;
        for (int nbMovement = 0; nbMovement < hitboxSizeRatio; nbMovement++){
            curPosX = x();
            xAnimation->stop();
            xAnimation->setStartValue(curPosX);
            xAnimation->setEndValue(curPosX + 5*direction*itsSpeed);
            xAnimation->setEasingCurve(QEasingCurve::Linear);
            xAnimation->setDuration(0);
            xAnimation->start();
        }
    }
}


/* Fighter jump animation up only */
void Fighter::jump()
{
    if(!isJumping && itsAnimationState != CROUCH && itsAnimationState != PUNCHCROUCH){

        isJumping = true;
        setItsAnimationState(JUMP);

        qreal curPosY = y();

        yAnimation->setDuration(450/itsSpeed);
        yAnimation->setKeyValueAt(0,curPosY);
        //Move the fighter at the size of a fighter
        yAnimation->setKeyValueAt(0.5,curPosY - itsHitBoxRectSurface->boundingRect().height()*1.5);
        yAnimation->setKeyValueAt(1,groundPosition);

        yAnimation->start();

        connect(yAnimation, &QAbstractAnimation::finished, this, [=](){
            this->isJumping = false;
        });
    }
}


/* Fighter jump animation toward right or left */
void Fighter::jumpDirection()
{
    if(!isJumping){

        //To know where the jump have to go (right or left)
        int direction = 1;
        if(isJumpingLeft)
            direction = -1;

        isJumping = true;
        setItsAnimationState(JUMP);

        jumpAnimation->setDuration(450/itsSpeed);

        //retrieve initial coordinate
        setCoord(QPointF(x(),y()));
        //Coordinate that will be use for each animation's segement
        QPointF animCoord;

        //start at this point
        jumpAnimation->setKeyValueAt(0, coord());

        //Values used to have a smoother jump
        float yMultpls[3] = {0.8,1.5,0.8};
        float xMultps[3] = {0.5,1,1.5};
        float steps[4] = {0.25,0.5,0.75,1};

        for (int step=0; step<4; step++){
            if(step <3){
                animCoord = coord() -= QPointF(-boundingRect().width()*direction*xMultps[step],
                                               boundingRect().height()*yMultpls[step]);
            }
            //Final step on the ground
            else{
                animCoord = QPointF(coord().x()+boundingRect().width()*direction*2,
                                    groundPosition);
            }

            /*To make the figther stay within the border during a side jump
                replace the x coordinate with the limit of the screen*/
            if ((animCoord.x() < cameraMinScreenLimit)
                    || (animCoord.x() > cameraMaxScreenLimit - boundingRect().width())){

                if(direction == -1){
                    animCoord.setX(cameraMinScreenLimit);
                }
                else{
                    animCoord.setX(cameraMaxScreenLimit - boundingRect().width());
                }
            }

            jumpAnimation->setKeyValueAt(steps[step], animCoord);
        }


        jumpAnimation->start();

        //Make the jump unable once the animation is finished
        connect(jumpAnimation, &QAbstractAnimation::finished, this, [=](){
            this->isJumping = false;
        });
    }
}





/* Decrease fighter current HP (Defense) */
void Fighter::tank(int damage)
{
    if(itsAnimationState != BLOCKINGCROUCH && itsAnimationState != BLOCKINGSTANDING)
        itsHP -= damage;

    if(itsHP <= 0)
        itsHP = 0;
}



/* If the fighter body intersects the other fighter body, we push them in order to avoid layering */
bool Fighter::intersectOtherFighterBody(Fighter * opponent)
{
    return itsHitBoxBody->mapToScene(
                itsHitBoxBody->boundingRect()).intersects(
                opponent->itsHitBoxBody->mapToScene(
                    opponent->itsHitBoxBody->boundingRect()
                    )
                );
}


/* Detect intersection between fighter's arm and opponent's member */
bool Fighter::fighterArmIntersectOpponent(QGraphicsRectItem * anOpponentMember)
{
    return itsHitBoxArm->mapToScene(
                itsHitBoxArm->boundingRect()).intersects(
                anOpponentMember->mapToScene(
                    anOpponentMember->boundingRect()
                    )
                );
}


/* Detect intersection between fighter's leg and opponent's member */
bool Fighter::fighterLegIntersectOpponent(QGraphicsRectItem *anOpponentMember)
{
    return itsHitBoxLeg->mapToScene(
                itsHitBoxLeg->boundingRect()).intersects(
                anOpponentMember->mapToScene(
                    anOpponentMember->boundingRect()
                    )
                );
}




/* Update animation accordingly with the state, called with a timer loop
 * ANIMATION DONE :
 * STATIC
 * PUNCHSTANDING
 * CROUCH
 * HIGHKICK
 * WIN
 * BLOCKINGCROUCH
 * BLOCKINGSTANDING
 * CROUCHHIT
 * STANDINGHIT
 * JUMP
 * INTRO
 * WALKING
 * STUN
 * PUNCHCROUCH
 * LOST
 *
 *
 * - UNDONE :
 * SPECIALATTACK
 * KNOCKOUT
 * UPPERCUT
 *
    SWEEPING

 */
void Fighter::updateAnimation()
{

    // Increasing animation counter
    animCounter++;

    /* Animation system */
    QString fileTitle =
            ":/" +
            itsName +
            '_' +
            AnimationStatesToStr(itsAnimationState)  +
            QString::fromStdString(std::to_string(animCounter)) +
            ".png";


    itsPixmap = QPixmap(fileTitle).scaled(QSize(268,420));

    // Mirrored if necessary
    if(itsDirection == FACELEFT){
        itsPixmap = QPixmap::fromImage(itsPixmap.toImage().mirrored(true,false));
    }

    /* sets the principal pixmap animation */
    setPixmap(itsPixmap);

    /* erase the precedent adjust pixmap */
    if(animationAdjustPixmap != nullptr)
    {
        delete animationAdjustPixmap;
        animationAdjustPixmap = nullptr;
    }


    /* STATIC = IDLE */
    if(animCounter >= 5 && itsAnimationState == STATIC){
        animCounter = 0;
    }
    /* PUNCHSTANDING */
    else if(animCounter >= 3 && itsAnimationState == PUNCHSTANDING)
    {
        /* ADJUST PIXMAP */
        if(itsName == "Maxime"){
            animationAdjustPixmap = new QGraphicsPixmapItem(this);
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Maxime_PUNCHSTANDINGAdjust.png").scaled(QSize(164,420))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/Maxime_PUNCHSTANDINGAdjust.png").scaled(QSize(164,420)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }

        punchAttack(); // move fist hitbox on the third frame only

        /* Reset */
        animCounter = 0;

    }
    /* PUNCHSTANDING noah's adjust pixmap */
    else if(animCounter == 2
            && itsAnimationState == PUNCHSTANDING
            && itsName == "NOAH")
    {
        animationAdjustPixmap = new QGraphicsPixmapItem(this);
        if(itsDirection == FACELEFT)
        {
            animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_PUNCHSTANDING2_adjust.png").scaled(30,136)).toImage().mirrored(true,false)));
            animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),-82));
        }
        else if(itsDirection == FACERIGHT)
        {
            animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_PUNCHSTANDING2_adjust.png").scaled(30,136));
            animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),82));
        }

        punchAttack(); // move fist hitbox on the second frame only

    }
    /* PUNCHSTANDING nils's adjust pixmap */
    else if(itsAnimationState == PUNCHSTANDING
            && itsName == "Nils")
    {
        animationAdjustPixmap = new QGraphicsPixmapItem(this);
        if(itsDirection == FACELEFT)
        {
            animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Nils_PUNCHSTANDINGAdjust.png").scaled(67*hitboxSizeRatio,105*hitboxSizeRatio)).toImage().mirrored(true,false)));
            animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
        }
        else if(itsDirection == FACERIGHT)
        {
            animationAdjustPixmap->setPixmap(QPixmap(":/Nils_PUNCHSTANDINGAdjust.png").scaled(67*hitboxSizeRatio,105*hitboxSizeRatio));
            animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
        }

        punchAttack(); // move fist hitbox on the first frame only

        /* Reset */
        animCounter = 0;
    }

    /* CROUCH animation */
    else if(animCounter >= 3 && itsAnimationState == CROUCH)
    {
        // Stay in crouch until the player release the crouch key
        animCounter = 2;
    }
    /* Adjust pixmap for PUNCHCROUCH */
    else if(animCounter >= 3 && itsAnimationState == PUNCHCROUCH)
    {
        int width=164, height=420;

        /* nils's adjust pixmap */
        if(itsName == "Nils"){
            width = 67*hitboxSizeRatio;
            height = 105*hitboxSizeRatio;
        }

        animationAdjustPixmap = new QGraphicsPixmapItem(this);
        if(itsDirection == FACELEFT)
        {
            animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/" + itsName + "_PUNCHCROUCHAdjust.png").scaled(QSize(width,height))).toImage().mirrored(true,false)));
            animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
        }
        else if(itsDirection == FACERIGHT)
        {
            animationAdjustPixmap->setPixmap(QPixmap(":/" + itsName + "_PUNCHCROUCHAdjust.png").scaled(QSize(width,height)));
            animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
        }

        punchAttack(); // move fist hitbox on the third frame only
    }
    /* HIGHKICK */
    else if ((animCounter >= 3 && itsAnimationState == HIGHKICK))
    {
        /* adjust pixmap */
        animationAdjustPixmap = new QGraphicsPixmapItem(this);

        /* maxime's adjust pixmap */
        if(itsName == "Maxime"){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Maxime_HIGHKICKAdjust.png").scaled(QSize(164,420))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/Maxime_HIGHKICKAdjust.png").scaled(QSize(164,420)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }

        /* noah's adjust pixmap */
        if(itsName == "NOAH"){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_HIGHKICK3_adjust.png").scaled(QSize(96,226))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),-83));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_HIGHKICK3_adjust.png").scaled(QSize(96,226)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),83));
            }
        }

        kickAttack(); // move leg hitbox on the third frame only
    }

    /* Nils highKick adjust */
    else if(itsAnimationState == HIGHKICK
            && itsName == "Nils"
            && animCounter == 2)
    {
        /* adjust pixmap */
        animationAdjustPixmap = new QGraphicsPixmapItem(this);

        if(itsDirection == FACELEFT)
        {
            animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Nils_HIGHKICKAdjust1.png").scaled(QSize(67*hitboxSizeRatio,105*hitboxSizeRatio))).toImage().mirrored(true,false)));
            animationAdjustPixmap->setPos(QPointF(-animationAdjustPixmap->pixmap().width(),-34));
        }
        else if(itsDirection == FACERIGHT)
        {
            animationAdjustPixmap->setPixmap(QPixmap(":/Nils_HIGHKICKAdjust1.png").scaled(QSize(67*hitboxSizeRatio,105*hitboxSizeRatio)));
            animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),-34));
        }
    }



    /* WIN */
    else if(animCounter >= 3 && itsAnimationState == WIN && (itsName == "Maxime" || itsName == "Nils")) animCounter = 2; /* exception for maxime's & nils' fighter */
    else if(animCounter >= 4 && itsAnimationState == WIN) animCounter = 3;
    /* BLOCKINGSTANDING */
    else if(animCounter >= 1 && itsAnimationState == BLOCKINGSTANDING)
    {
        animCounter = 0;

        QTimer::singleShot(100,this,[=](){
            itsAnimationState = STATIC;
        });

    }
    /* BLOCKINGCROUCH */
    else if(animCounter >= 1 && itsAnimationState == BLOCKINGCROUCH)
    {
        /* ADJUST PIXMAP */
        if(itsName == "Maxime"){
            animationAdjustPixmap = new QGraphicsPixmapItem(this);

            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Maxime_BLOCKINGCROUCHAdjust.png").scaled(QSize(40,420))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/Maxime_BLOCKINGCROUCHAdjust.png").scaled(QSize(40,420)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }

        /* Reset */
        animCounter = 0;

        QTimer::singleShot(100,this,[=](){
            itsAnimationState = CROUCH;
        });
    }
    /*Nils jump adjustement*/
    if(itsAnimationState == JUMP
            && itsName == "Nils"
            && (animCounter == 2 || animCounter == 3 || animCounter == 6 /*|| animCounter == 7)*/)){
        /* adjust pixmap */
        animationAdjustPixmap = new QGraphicsPixmapItem(this);


        if(itsDirection == FACELEFT)
        {
            animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Nils_JUMPAdjust.png").scaled(QSize(67*hitboxSizeRatio,21*hitboxSizeRatio))).toImage().mirrored(true,false)));
            animationAdjustPixmap->setPos(QPointF(0,-animationAdjustPixmap->pixmap().height()));
        }
        else if(itsDirection == FACERIGHT)
        {
            animationAdjustPixmap->setPixmap(QPixmap(":/Nils_JUMPAdjust.png").scaled(QSize(67*hitboxSizeRatio,21*hitboxSizeRatio)));
            animationAdjustPixmap->setPos(QPointF(0,-animationAdjustPixmap->pixmap().height()));
        }
    }
    /* JUMP */
    else if(animCounter >= 7 && itsAnimationState == JUMP)
    {
        /* Reset */
        animCounter = 0;
        itsAnimationState = STATIC;
    }
    /* LOST */
    else if(animCounter >= 3 && itsAnimationState == LOST && itsName == "Maxime") animCounter = 2;
    else if(animCounter >= 4 && itsAnimationState == LOST && itsName == "Nils") animCounter = 3;
    else if((animCounter >= 2) && itsAnimationState== LOST)
    {
        animationAdjustPixmap = new QGraphicsPixmapItem(this);

        /* noah's adjust pixmap */
        if(itsName == "NOAH" && animCounter == 4){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_LOST4_adjust.png").scaled(QSize(80,140))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),-280));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_LOST4_adjust.png").scaled(QSize(80,140)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),280));
            }
        }
        else if(itsName == "NOAH" && animCounter == 5){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_LOST5_adjust.png").scaled(QSize(80,140))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),-280));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_LOST5_adjust.png").scaled(QSize(80,140)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),280));
            }
        }
        /* cyril's adjust pixmap */
        else if(itsName == "CYRIL" && animCounter == 2){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/CYRIL_LOST2ADJUST.png").scaled(67*hitboxSizeRatio,105*hitboxSizeRatio)).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/CYRIL_LOST2ADJUST.png").scaled(67*hitboxSizeRatio,105*hitboxSizeRatio));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }
        else if(itsName == "CYRIL" && animCounter == 3){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/CYRIL_LOST3ADJUST.png").scaled(67*hitboxSizeRatio,105*hitboxSizeRatio)).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/CYRIL_LOST3ADJUST.png").scaled(67*hitboxSizeRatio,105*hitboxSizeRatio));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }


        /* LOST -after added the adjust- */
        if(animCounter >= 5)
        {
            animCounter = 4;
        }

    }
    /* WALKING */
    else if(animCounter >= 5 && itsAnimationState == WALKING) {
        animCounter = 0;
        itsAnimationState = STATIC;
    }
    /* INTRO */
    else if(animCounter >= 4 && itsAnimationState == INTRO)
    {
        /* noah's adjust pixmap */
        if(itsName == "NOAH"){

            animationAdjustPixmap = new QGraphicsPixmapItem(this);
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_INTRO4_adjust.png").scaled(QSize(188,228))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),40));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_INTRO4_adjust.png").scaled(QSize(188,228)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),40));
            }
        }

        setItsAnimationState(STATIC);
    }

    /* KNOCKOUT animation */
    else if(animCounter >= 1 && itsName == "Maxime" && itsAnimationState == KNOCKOUT) {
        animCounter = 0;
        itsAnimationState = STATIC;
    }
    else if(animCounter >= 2 && itsName == "Nils" && itsAnimationState == KNOCKOUT) {
        animCounter = 0;
        itsAnimationState = STATIC;
    }
    else if(animCounter >= 3 && itsAnimationState == KNOCKOUT) {
        animationAdjustPixmap = new QGraphicsPixmapItem(this);

        /* noah's adjust pixmap */
        if(itsName == "NOAH" && animCounter == 3){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_KNOCKOUT3_adjust.png").scaled(QSize(80,152))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),-270));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_KNOCKOUT3_adjust.png").scaled(QSize(80,152)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),270));
            }
        }

        else if(animCounter >= 5)
        {
            animCounter = 0;
            itsAnimationState = STATIC;
        }

    }
    /* CROUCH HIT animation */
    else if(animCounter >= 2 && itsAnimationState == CROUCHHIT)
    {
        /* Reset */
        animCounter = 2; // Stay in crouch
        itsAnimationState = CROUCH;
    }
    /* STANDING HIT animation */
    else if(animCounter >= 3 && itsAnimationState == STANDINGHIT)
    {
        /* Reset */
        animCounter = 0;
        itsAnimationState = STATIC;
    }
    /* STUN */
    else if(animCounter >= 1 && itsAnimationState == STUN) animCounter = 0;


    /* SWEEPING */
    else if(animCounter == 2 && itsAnimationState == SWEEPING) {
        animationAdjustPixmap = new QGraphicsPixmapItem(this);

        if(itsName == "NOAH"){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/NOAH_SWEEPING2_adjust.png").scaled(QSize(80,152))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),-270));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/NOAH_SWEEPING2_adjust.png").scaled(QSize(80,152)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),270));
            }
        }
        /* nils' adjust pixmap*/
        else if(itsName == "Nils"){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Nils_SWEEPINGAdjust.png").scaled(QSize(67*hitboxSizeRatio,105*hitboxSizeRatio))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/Nils_SWEEPINGAdjust.png").scaled(QSize(67*hitboxSizeRatio,105*hitboxSizeRatio)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }
        else if(itsName == "Maxime"){
            if(itsDirection == FACELEFT)
            {
                animationAdjustPixmap->setPixmap(QPixmap::fromImage((QPixmap(":/Maxime_SWEEPINGAdjust.png").scaled(QSize(54*hitboxSizeRatio,105*hitboxSizeRatio))).toImage().mirrored(true,false)));
                animationAdjustPixmap->setPos(-QPointF(animationAdjustPixmap->pixmap().width(),0));
            }
            else if(itsDirection == FACERIGHT)
            {
                animationAdjustPixmap->setPixmap(QPixmap(":/Maxime_SWEEPINGAdjust.png").scaled(QSize(54*hitboxSizeRatio,105*hitboxSizeRatio)));
                animationAdjustPixmap->setPos(QPointF(itsPixmap.width(),0));
            }
        }


        kickAttack(); // move leg hitbox on the second frame only

    }
    /* SWEEPING MAXIME ADJUST */
    else if(animCounter >= 4
            && itsAnimationState == SWEEPING
            && itsName == "Maxime") {
        /* Reset */
        animCounter = 2;
        itsAnimationState = CROUCH;
    }
}



/* Decrease opponent life and throw punch animation */
void Fighter::punchAttack()
{
    if(canAttack && canPunchAttack && itsAnimationState != KNOCKOUT)
    {
        canPunchAttack = false;

        if(itsAnimationState == PUNCHCROUCH){
            switch (itsDirection)
            {
            case FACERIGHT:
                itsHitBoxArm->setPos(QPointF(itsPixmap.width(),itsHitBoxRectSurface->boundingRect().height()/1.5));
                break;
            case FACELEFT:
                itsHitBoxArm->setPos(QPointF(-itsHitBoxArm->rect().height(),itsHitBoxRectSurface->boundingRect().height()/1.5));
                break;
            }
        }
        else{
            switch (itsDirection)
            {
            case FACERIGHT:
                itsHitBoxArm->setPos(QPointF(itsPixmap.width(),itsHitBoxRectSurface->boundingRect().height()/3));
                break;
            case FACELEFT:
                itsHitBoxArm->setPos(QPointF(-itsHitBoxArm->rect().height(),itsHitBoxRectSurface->boundingRect().height()/3));
                break;
            }
        }
    }
}



/* Decrease opponent life and throw kick animation */
void Fighter::kickAttack()
{
    if(canAttack && canKickAttack && itsAnimationState != KNOCKOUT && !legHasHit)
    {
        canKickAttack = false;

        if(itsAnimationState == SWEEPING){
            switch (itsDirection)
            {
            case FACERIGHT:

                itsHitBoxLeg->setRotation(-85);
                itsHitBoxLeg->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width() + 32,
                                             itsHitBoxRectSurface->boundingRect().height() - 35));
                break;

            case FACELEFT:
                itsHitBoxLeg->setRotation(85);
                itsHitBoxLeg->setPos(QPointF(- 39,
                                             itsHitBoxRectSurface->boundingRect().height() - 31 - itsHitBoxLeg->rect().width()));
            }
        }
        else{
            switch (itsDirection)
            {
            case FACERIGHT:
                itsHitBoxLeg->setRotation(-120);
                itsHitBoxLeg->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width()+25,
                                             itsHitBoxRectSurface->boundingRect().height()/2.5 -5));
                break;
            case FACELEFT:
                itsHitBoxLeg->setRotation(120);
                itsHitBoxLeg->setPos(QPointF(0, itsHitBoxRectSurface->boundingRect().height()/2.5 -55));
                break;
            }
        }
    }
}




/* Return an hit message in order to display wich part of opponent's body is hit */
QString Fighter::hitMessageIfHit(Fighter * anOpponent)
{
    QString hitMessage = "no message";

    if(!canPunchAttack || !canKickAttack)
    {
        if((anOpponent->getItsAnimationState() != BLOCKINGCROUCH
            || anOpponent->getItsAnimationState() != BLOCKINGSTANDING
            || anOpponent->getItsAnimationState() != KNOCKOUT))
        {
            /******************* PUNCH DAMAGE *******************/
            if(itsAnimationState == PUNCHSTANDING
                    || itsAnimationState == PUNCHCROUCH)
            {
                // This fighter hits opponent's head
                if(fighterArmIntersectOpponent(anOpponent->getItsHitBoxHead()))
                {
                    hitMessage = "HIT HEAD";

                    anOpponent->tank(getItsPunchAttack());

                    /* Update opponent animation */
                    if(anOpponent->getItsAnimationState() == CROUCH)
                        anOpponent->setItsAnimationState(CROUCHHIT);
                    else if(anOpponent->getItsAnimationState() == STATIC)
                        anOpponent->setItsAnimationState(STANDINGHIT);

                    // Reset arm position
                    itsHitBoxArm->setPos(hitboxArmOrigiPos);
                }
                // This fighter hits opponent's leg
                else if(fighterArmIntersectOpponent(anOpponent->getItsHitBoxLeg()))
                {
                    hitMessage = "LEG HIT";

                    anOpponent->tank(getItsPunchAttack()*0.8);

                    /* Update opponent animation */
                    if(anOpponent->getItsAnimationState() == CROUCH)
                        anOpponent->setItsAnimationState(CROUCHHIT);
                    else if(anOpponent->getItsAnimationState() == STATIC)
                        anOpponent->setItsAnimationState(STANDINGHIT);

                    // Reset arm position
                    itsHitBoxArm->setPos(hitboxArmOrigiPos);
                }
                // This fighter hits opponent's body
                else if(fighterArmIntersectOpponent(anOpponent->getItsHitBoxBody()))
                {
                    hitMessage = "BODY HIT";

                    anOpponent->tank(getItsPunchAttack()*0.6);

                    /* Update opponent animation */
                    if(anOpponent->getItsAnimationState() == CROUCH)
                        anOpponent->setItsAnimationState(CROUCHHIT);
                    else if(anOpponent->getItsAnimationState() == STATIC)
                        anOpponent->setItsAnimationState(STANDINGHIT);

                    // Reset arm position
                    itsHitBoxArm->setPos(hitboxArmOrigiPos);
                }
            }

            /******************* KICK DAMAGE *******************/
            else if((itsAnimationState == HIGHKICK
                     || itsAnimationState == SWEEPING)
                    && !legHasHit)
            {
                // This fighter hits opponent's head
                if(fighterLegIntersectOpponent(anOpponent->getItsHitBoxHead()))
                {
                    hitMessage = "HIT HEAD";
                    legHasHit = true;
                    anOpponent->tank(getItsKickAttack());

                    /* Update opponent animation */
                    if(anOpponent->getItsAnimationState() == CROUCH)
                        anOpponent->setItsAnimationState(CROUCHHIT);
                    else if(anOpponent->getItsAnimationState() == STATIC)
                        anOpponent->setItsAnimationState(STANDINGHIT);

                }

                // This fighter hits opponent's leg
                else if(fighterLegIntersectOpponent(anOpponent->getItsHitBoxLeg()))
                {
                    if(itsAnimationState==SWEEPING)
                        anOpponent->setItsAnimationState(KNOCKOUT);
                    hitMessage = "LEG HIT";
                    legHasHit = true;
                    anOpponent->tank(getItsKickAttack()*0.8);

                    /* Update opponent animation */
                    if(anOpponent->getItsAnimationState() == CROUCH)
                        anOpponent->setItsAnimationState(CROUCHHIT);
                    else if(anOpponent->getItsAnimationState() == STATIC)
                        anOpponent->setItsAnimationState(STANDINGHIT);

                }
                // This fighter hits opponent's body
                else if(fighterLegIntersectOpponent(anOpponent->getItsHitBoxBody()))
                {
                    hitMessage = "BODY HIT";
                    legHasHit = true;
                    anOpponent->tank(getItsKickAttack()*0.6);

                    /* Update opponent animation */
                    if(anOpponent->getItsAnimationState() == CROUCH)
                        anOpponent->setItsAnimationState(CROUCHHIT);
                    else if(anOpponent->getItsAnimationState() == STATIC)
                        anOpponent->setItsAnimationState(STANDINGHIT);

                }
                else
                    legHasNotHit = true;
            }


            /* Reset */

            /* Hitbox Arm */
            itsHitBoxArm->setPos(hitboxArmOrigiPos);

            if(itsAnimationState == PUNCHSTANDING || itsAnimationState == HIGHKICK)
            {
                /* Reset */
                animCounter = 0;
                itsAnimationState = STATIC;
            }
            else if(itsAnimationState == PUNCHCROUCH){
                /* Reset */
                animCounter = 2;
                itsAnimationState = CROUCH;
            }
            else if(itsAnimationState == SWEEPING && itsName != "Maxime")
            {
                /* Reset */
                animCounter = 2;
                itsAnimationState = CROUCH;
            }

            canPunchAttack = true;
            canKickAttack = true;
        }

    }
    return hitMessage;
}


/* Update Hitbox accordingly with the direction and the animation state */
void Fighter::updateHitbox()
{
    if(itsAnimationState == CROUCH || itsAnimationState == PUNCHCROUCH || itsAnimationState == SWEEPING)
    {
        /* Hitbox body */
        itsHitBoxBody->setPos(
                    QPointF(itsHitBoxRectSurface->boundingRect().width()/2,
                            itsHitBoxRectSurface->boundingRect().height()/2) -
                    QPointF(itsHitBoxBody->boundingRect().width()/2,
                            itsHitBoxBody->boundingRect().height()*0.01));

        /* Hitbox Head */
        itsHitBoxHead->setPos(QPointF(itsHitBoxHead->pos().x(),
                                      itsHitBoxRectSurface->boundingRect().height()/2.2));

        /* Hitbox Arm */
        itsHitBoxArm->setPos(QPointF(itsHitBoxArm->pos().x(),
                                     itsHitBoxRectSurface->boundingRect().height()/1.5));



    }
    else{
        /* Hitbox body RESET TO STATIC POS */
        itsHitBoxBody->setPos(
                    QPointF(itsHitBoxRectSurface->boundingRect().width()/2,
                            itsHitBoxRectSurface->boundingRect().height()/2) -
                    QPointF(itsHitBoxBody->boundingRect().width()/2,
                            itsHitBoxBody->boundingRect().height()/1.5));


        /* Hitbox Head */
        switch (itsDirection) {
        case FACERIGHT:
            itsHitBoxHead->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width()/1.9,
                                          itsHitBoxRectSurface->boundingRect().height()/7));
            break;
        case FACELEFT:
            itsHitBoxHead->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width()/4,
                                          itsHitBoxRectSurface->boundingRect().height()/7));
            break;
        }
    }

    if(canPunchAttack && canKickAttack)
    {
        // Reset leg position if opponent hit
        if(legHasHit || legHasNotHit){
            /* Reset Leg position */
            switch (itsDirection) {
            case FACERIGHT:
                /* Hitbox leg */
                itsHitBoxLeg->setPos(hitboxLegOrigiPos);
                itsHitBoxLeg->setRotation(-35);
                break;
            case FACELEFT:

                /* Hitbox leg */
                itsHitBoxLeg->setRotation(20);
                itsHitBoxLeg->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width()/4,
                                             itsHitBoxRectSurface->boundingRect().height()/2));
                break;
            }

            legHasHit = false;
            legHasNotHit = false;
        }
    }
}


/* Check if the fighter is alive */
bool Fighter::isAlive() { return itsHP > 0; }


/* Translate animation states to string for the animation files system */
QString Fighter::AnimationStatesToStr(AnimationStates state)
{
    QString animStr = "STATIC";

    switch(state)
    {
    case STANDINGHIT:       animStr = "STANDINGHIT";        break;
    case WALKING:           animStr = "WALKING";            break;
    case STATIC:            animStr = "STATIC";             break;
    case PUNCHSTANDING:     animStr = "PUNCHSTANDING";      break;
    case PUNCHCROUCH:       animStr = "PUNCHCROUCH";        break;
    case HIGHKICK:          animStr = "HIGHKICK";           break;
    case UPPERCUT:          animStr = "UPPERCUT";           break;
    case SWEEPING:          animStr = "SWEEPING";           break;
    case SPECIALATTACK:     animStr = "SPECIALATTACK";      break;
    case BLOCKINGCROUCH:    animStr = "BLOCKINGCROUCH";     break;
    case BLOCKINGSTANDING:  animStr = "BLOCKINGSTANDING";   break;
    case CROUCHHIT:         animStr = "CROUCHHIT";          break;
    case KNOCKOUT:          animStr = "KNOCKOUT";           break;
    case CROUCH:            animStr = "CROUCH";             break;
    case JUMP:              animStr = "JUMP";               break;
    case INTRO:             animStr = "INTRO";              break;
    case WIN:               animStr = "WIN";                break;
    case LOST:              animStr = "LOST";               break;
    case STUN:              animStr = "STUN";               break;
    }

    return animStr;
}



void Fighter::addPower(int aPower)
{
    if ((itsPower+aPower)>= itsPowerNeed*3)
        itsPower=itsPowerNeed*3;
    else
        itsPower+=aPower;
}



/* Getters */
int Fighter::getItsPower() const { return itsPower; }

int Fighter::getItsPunchAttackCP() const { return itsPunchAttackCP; }

int Fighter::getItsKickAttackCP() const { return itsKickAttackCP; }

int Fighter::getItsPunchAttack() const { return itsPunchAttackCP; }

int Fighter::getItsKickAttack() const { return itsKickAttackCP; }

Direction Fighter::getItsDirection() const { return itsDirection; }

QPointF Fighter::coord() const { return m_coord; }

bool Fighter::getIsGoingLeft() const { return isGoingLeft; }

bool Fighter::getIsGoingRight() const { return isGoingRight; }

int Fighter::getItsBaseHP() const { return itsBaseHP; }

int Fighter::getItsHP() const { return itsHP; }

int Fighter::getItsRoundWon() const { return itsRoundWon; }

qreal Fighter::x() const {return moveX;}

qreal Fighter::y() const { return moveY; }

QGraphicsRectItem *Fighter::getItsHitBoxRectSurface() const { return itsHitBoxRectSurface; }

QGraphicsRectItem *Fighter::getItsHitBoxBody() const { return itsHitBoxBody; }

bool Fighter::getIsJumping() const { return isJumping; }

bool Fighter::getIsJumpingRight() const { return isJumpingRight; }

bool Fighter::getIsJumpingLeft() const { return isJumpingLeft; }

const QString &Fighter::getItsName() const { return itsName; }

QPropertyAnimation *Fighter::getXAnimation() const { return xAnimation; }

bool Fighter::getCanJump() const { return canJump; }

int Fighter::getCameraMinScreenLimit() const { return cameraMinScreenLimit; }

int Fighter::getCameraMaxScreenLimit() const { return cameraMaxScreenLimit; }

AnimationStates Fighter::getItsAnimationState() const { return itsAnimationState; }

QGraphicsRectItem *Fighter::getItsHitBoxLeg() const { return itsHitBoxLeg; }

QGraphicsRectItem *Fighter::getItsHitBoxHead() const { return itsHitBoxHead; }

bool Fighter::getCanPunchAttack() const { return canPunchAttack; }

int Fighter::getItsPowerNeed() const { return itsPowerNeed; }

QPropertyAnimation *Fighter::getJumpAnimation() const { return jumpAnimation; }


/* Setters */

// Set direction if it's not the same, reverse the pixmap and set it
void Fighter::setItsDirection(Direction newItsDirection)
{
    if(itsDirection != newItsDirection)
    {
        itsPixmap = QPixmap::fromImage(itsPixmap.toImage().mirrored(true,false));

        itsDirection = newItsDirection;
    }

    setPixmap(itsPixmap);

    switch (itsDirection) {
    case FACERIGHT:
        /* Hitbox leg */
        itsHitBoxLeg->setPos(hitboxLegOrigiPos);
        itsHitBoxLeg->setRotation(-35);
        break;
    case FACELEFT:

        /* Hitbox leg */
        itsHitBoxLeg->setRotation(20);
        itsHitBoxLeg->setPos(QPointF(itsHitBoxRectSurface->boundingRect().width()/4,
                                     itsHitBoxRectSurface->boundingRect().height()/2));
        break;
    }

}

void Fighter::setIsGoingLeft(bool newIsGoingLeft) { isGoingLeft = newIsGoingLeft; }

void Fighter::setIsGoingRight(bool newIsGoingRight) { isGoingRight = newIsGoingRight; }

// move the fighter on the x axis
void Fighter::setX(qreal newX)
{
    moveBy(newX - moveX,0);
    moveX = newX;
}

// move the fighter on the y axis
void Fighter::setY(qreal newY)
{
    moveBy(0,newY - moveY);
    moveY = newY;
}

// change fighter coordinate (move on x and y axis)
void Fighter::setCoord(QPointF newCoord)
{
    m_coord = newCoord;
    setX(newCoord.x());
    setY(newCoord.y());
}

void Fighter::initCoord() { coord() = QPointF(x(),y()); }

void Fighter::setItsRoundWon(int newRoundWon) { itsRoundWon = newRoundWon; }

void Fighter::setIsJumping(bool newIsJumping) { isJumping = newIsJumping; }

void Fighter::setIsJumpingRight(bool newIsJumpingRight) { isJumpingRight = newIsJumpingRight; }

void Fighter::setIsJumpingLeft(bool newIsJumpingLeft) { isJumpingLeft = newIsJumpingLeft; }

void Fighter::setCanJump(bool newCanJump) { canJump = newCanJump; }

void Fighter::setItsHP(int newItsHP) { itsHP = newItsHP; }

void Fighter::setItsSpeed(int newItsSpeed){ itsSpeed = newItsSpeed;}

void Fighter::setItsAnimationState(AnimationStates newItsAnimationState) {
    itsASAnimation = newItsAnimationState;

    // Spam security
    if(newItsAnimationState != itsAnimationState){
        /* anti-spam -windows only- for crouch */
        if(newItsAnimationState==STATIC && itsAnimationState==CROUCH) {
            QTimer::singleShot(40,this,[=](){
                if (itsASAnimation != CROUCH) {
                    animCounter = 0;
                    itsAnimationState = newItsAnimationState;
                }
            });
        } else {
            animCounter = 0;
            itsAnimationState = newItsAnimationState;
        }
    }

}

void Fighter::setCameraMinScreenLimit(int newCameraMinScreenLimit) { cameraMinScreenLimit = newCameraMinScreenLimit; }

void Fighter::setCanAttack(bool newCanAttack){ canAttack = newCanAttack; }

void Fighter::setCameraMaxScreenLimit(int newCameraMaxScreenLimit) { cameraMaxScreenLimit = newCameraMaxScreenLimit; }

void Fighter::setItsPowerNeed(int powerNeed) { itsPowerNeed = powerNeed; }

void Fighter::setItsPunchAttackCP(int newItsPunchAttackCP) { itsPunchAttackCP = newItsPunchAttackCP; }

void Fighter::setItsKickAttackCP(int newItsKickAttackCP) { itsKickAttackCP = newItsKickAttackCP; }

void Fighter::setItsPunchAttack(int newItsPunchAttackCP) { itsPunchAttackCP = newItsPunchAttackCP; }

void Fighter::setItsKickAttack(int newItsKickAttackCP) { itsKickAttackCP = newItsKickAttackCP; }
