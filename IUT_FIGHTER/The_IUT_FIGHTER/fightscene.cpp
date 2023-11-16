#include "fightscene.h"

FightScene::FightScene(QGraphicsView *view, QProgressBar *firstPBar, QProgressBar *secondPBar, Key *keyManager, std::vector<QProgressBar *> fightersPowerBars, QObject *parent)
    : QGraphicsScene(parent),
      itsView(view),
      keys(keyManager),
      itsFirstFighterLifeB(firstPBar),
      itsSecondFighterLifeB(secondPBar)
{
    // Center origin
    setSceneRect(-itsSceneWidth/2,-itsSceneHeight/2,itsSceneWidth,itsSceneHeight);

    /* set the power bars */
    itsFirstFighterPowerBar1 = fightersPowerBars[0];
    itsFirstFighterPowerBar2 = fightersPowerBars[1];
    itsFirstFighterPowerBar3 = fightersPowerBars[2];
    itsSecondFighterPowerBar1 = fightersPowerBars[3];
    itsSecondFighterPowerBar2 = fightersPowerBars[4];
    itsSecondFighterPowerBar3 = fightersPowerBars[5];

    // Disable index method (the scene has moving items)
    setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);

    addItem(itsBackground);
    itsBackground->setBg(NULLV);
    // Center background
    itsBackground->setPos(QPointF(0,0) - QPointF(itsBackground->boundingRect().width()/2,
                                                 itsBackground->boundingRect().height()/2));

    connect(itsFightTimer,&QTimer::timeout,this,&FightScene::displayTimerOnFightScene);
    itsFightTimer->start(1000);

    connect(itsUpdateTimer,&QTimer::timeout, this, &FightScene::fightLoop);
}


/* Initialize fight scene :
 * - add fighters
 * - initialize life bars
 * - start update timer
 */
void FightScene::initializeFightScene(Fighter * fFighter, Fighter * sFighter, BackgroundState mapBg)
{
    /* Adding fighters */
    itsFirstFighter = fFighter;
    /*setPos is annoying because it set the reference point (default 0,0) to the new specified one
        that's why there are some gap when calculating... */
    fFighter->setPos(0,0);
    addItem(fFighter);
    itsFirstFighter->setX(-50-itsFirstFighter->boundingRect().width());

    itsSecondFighter = sFighter;
    sFighter->setItsDirection(FACELEFT);
    sFighter->setPos(0,0);
    addItem(sFighter);
    itsSecondFighter->setX(50);

    /* Fighters life bars */
    itsFirstFighterLifeB->setInvertedAppearance(true);
    itsFirstFighterLifeB->setRange(0,itsFirstFighter->getItsBaseHP());
    itsFirstFighterLifeB->setTextVisible(false);
    itsFirstFighterLifeB->setFixedSize(1920/2-100,50);
    itsFirstFighterLifeB->setVisible(true);
    itsFirstFighterLifeB->move(50,30);

    itsSecondFighterLifeB->setRange(0,itsSecondFighter->getItsBaseHP());
    itsSecondFighterLifeB->setTextVisible(false);
    itsSecondFighterLifeB->setFixedSize(1920/2-100,50);
    itsSecondFighterLifeB->setVisible(true);
    itsSecondFighterLifeB->move(1920/2+50,30);

    /* Fighters power bars */
    itsFirstFighterPowerBar1->setRange(0,itsFirstFighter->getItsPowerNeed());
    itsFirstFighterPowerBar1->setTextVisible(false);
    itsFirstFighterPowerBar1->setFixedSize(200,40);
    itsFirstFighterPowerBar1->setVisible(true);
    itsFirstFighterPowerBar1->move(50,itsSceneHeight-100);

    itsFirstFighterPowerBar2->setRange(0,itsFirstFighter->getItsPowerNeed());
    itsFirstFighterPowerBar2->setTextVisible(false);
    itsFirstFighterPowerBar2->setFixedSize(200,40);
    itsFirstFighterPowerBar2->setVisible(true);
    itsFirstFighterPowerBar2->move(255,itsSceneHeight-100);

    itsFirstFighterPowerBar3->setRange(0,itsFirstFighter->getItsPowerNeed());
    itsFirstFighterPowerBar3->setTextVisible(false);
    itsFirstFighterPowerBar3->setFixedSize(200,40);
    itsFirstFighterPowerBar3->setVisible(true);
    itsFirstFighterPowerBar3->move(460,itsSceneHeight-100);

    itsSecondFighterPowerBar1->setInvertedAppearance(true);
    itsSecondFighterPowerBar1->setRange(0,itsSecondFighter->getItsPowerNeed());
    itsSecondFighterPowerBar1->setTextVisible(false);
    itsSecondFighterPowerBar1->setFixedSize(200,40);
    itsSecondFighterPowerBar1->setVisible(true);
    itsSecondFighterPowerBar1->move(1920-250,itsSceneHeight-100);

    itsSecondFighterPowerBar2->setInvertedAppearance(true);
    itsSecondFighterPowerBar2->setRange(0,itsSecondFighter->getItsPowerNeed());
    itsSecondFighterPowerBar2->setTextVisible(false);
    itsSecondFighterPowerBar2->setFixedSize(200,40);
    itsSecondFighterPowerBar2->setVisible(true);
    itsSecondFighterPowerBar2->move(1920-455,itsSceneHeight-100);

    itsSecondFighterPowerBar3->setInvertedAppearance(true);
    itsSecondFighterPowerBar3->setRange(0,itsSecondFighter->getItsPowerNeed());
    itsSecondFighterPowerBar3->setTextVisible(false);
    itsSecondFighterPowerBar3->setFixedSize(200,40);
    itsSecondFighterPowerBar3->setVisible(true);
    itsSecondFighterPowerBar3->move(1920-660,itsSceneHeight-100);

    /*Showing a totem when a fighter win a round*/

    itsRoundWonPixMapForFirstFighter= new QGraphicsPixmapItem(QPixmap(":/star.png").scaled(QSize(60, 60)));
    addItem(itsRoundWonPixMapForFirstFighter);
    itsRoundWonPixMapForFirstFighter->hide();

    itsRoundWonPixMapForSecondFighter= new QGraphicsPixmapItem(QPixmap(":/star.png").scaled(QSize(60, 60)));
    addItem(itsRoundWonPixMapForSecondFighter);
    itsRoundWonPixMapForSecondFighter->hide();

    /*display timer on the fight*/
    /*Timer for fight scene*/
    itsTimerTextForFightScene->setPlainText("0:0");
    itsTimerTextForFightScene->setFont(btnFont);
    itsTimerTextForFightScene->setDefaultTextColor(Qt::white);
    addItem(itsTimerTextForFightScene);

    itsFirstFighter->setItsAnimationState(INTRO);
    itsSecondFighter->setItsAnimationState(INTRO);


    updateLifeBars();

    itsUpdateTimer->start(20);

    itsBackground->setBg(mapBg);


    displayFightersNames();

}


/* check if a movement is authorized, then only apply it */
void FightScene::moveFighter(Fighter *aFirstFighter, Fighter *aSecondFighter)
{
    if((aFirstFighter->getIsGoingRight() || aFirstFighter->getIsGoingLeft()) && (!fightIsInPause)) {

        /* get the direction */
        int direction = 1;
        if (aFirstFighter->getIsGoingLeft())
            direction = -1;

        /* sets the fighter to it's probable future position */
        aFirstFighter->setX(aFirstFighter->x()+100*direction);

        /* anti-contact rule implementation */
        if(aFirstFighter->intersectOtherFighterBody(aSecondFighter)) {
            aFirstFighter->setX(aFirstFighter->x()-100*direction);
            return;
        }
        aFirstFighter->setX(aFirstFighter->x()-90*direction);

        /* calculates the center of the actual displayed scene on the player's screen */
        int center = itsView->mapToScene(itsView->viewport()->rect().center()).x();
        /* anti-evasion rule implementation */
        if (aFirstFighter->x()<(center-1920/2)
                || aFirstFighter->x()+aFirstFighter->boundingRect().width()>(center+1920/2)) {
            aFirstFighter->setX(aFirstFighter->x()-10*direction);
            return;
        }

        /* re-set the fighter x-position to it's origin (before the verifications) */
        aFirstFighter->setX(aFirstFighter->x()-10*direction);

        aFirstFighter->walk();
    }
}


//Choose the actions to do for the fighter depends on the different flags (booleans like isJumping)
void FightScene::fighterMovementManager(Fighter *aFirstFighter, Fighter *aSecondFighter)
{
    /* Layering resolutions */
    if(aFirstFighter->intersectOtherFighterBody(aSecondFighter)
            //&& aFirstFighter->x()-aFirstFighter->getItsHitBoxRectSurface()->boundingRect().width() > -itsSceneWidth/2
            //&& aFirstFighter->x()+aFirstFighter->getItsHitBoxRectSurface()->boundingRect().width() < itsSceneWidth/2
            && !aSecondFighter->getIsJumping())
    {
        //aSecondFighter->getJumpAnimation()->stop();

        aFirstFighter->getJumpAnimation()->setStartValue(QPointF(aFirstFighter->x(), aFirstFighter->y()));
        if ((aSecondFighter->x() - (-itsSceneWidth/2) < aFirstFighter->boundingRect().width())
                || itsSceneWidth/2 -  (aSecondFighter->x()+aSecondFighter->boundingRect().width())
                < aFirstFighter->boundingRect().width()){

            if(itsSceneWidth/2 -  (aSecondFighter->x()+aSecondFighter->boundingRect().width())
                    < aFirstFighter->boundingRect().width()){
                aFirstFighter->getJumpAnimation()->setEndValue(
                            QPointF(aSecondFighter->x()-aFirstFighter->boundingRect().width()+50,0));
            }

            if(aSecondFighter->x() - (-itsSceneWidth/2) < aFirstFighter->boundingRect().width()){
                aFirstFighter->getJumpAnimation()->setEndValue(
                            QPointF(aSecondFighter->x() + aSecondFighter->boundingRect().width()-50,0));
            }
        }

        else{
            if((aFirstFighter->x()+aFirstFighter->boundingRect().width())/2 < (aSecondFighter->x()+aSecondFighter->boundingRect().width())/2)
                aFirstFighter->getJumpAnimation()->setEndValue(
                            QPointF(aSecondFighter->x()-aFirstFighter->boundingRect().width()+50,0));

            else if((aFirstFighter->x()+aFirstFighter->boundingRect().width())/2 >= (aSecondFighter->x()+aSecondFighter->boundingRect().width())/2)
                aFirstFighter->getJumpAnimation()->setEndValue(
                            QPointF(aSecondFighter->x() + aSecondFighter->boundingRect().width()-50,0));
        }
        aFirstFighter->getJumpAnimation()->setDuration(500);
        aFirstFighter->getJumpAnimation()-> start();

    }

    // aFighter movement
    if (!aFirstFighter->getIsJumping()){
        moveFighter(aFirstFighter, aSecondFighter);

        //To jump right or left
        if((aFirstFighter->getIsGoingRight()||aFirstFighter->getIsGoingLeft()) && aFirstFighter->getCanJump()){

            if(aFirstFighter->getIsGoingLeft()){
                aFirstFighter->setIsJumpingLeft(true);
            }
            else if(aFirstFighter->getIsGoingRight()){
                aFirstFighter->setIsJumpingRight(true);
            }

            //camera limits for fighters' jumps
            int centerCamera = itsView->mapToScene(itsView->viewport()->rect().center()).x();
            int minCamera = centerCamera - 1920/2, maxCamera = centerCamera + 1920/2;

            //Amount of deplacement during a side jump
            //            int jumpSideSize = aFirstFighter->boundingRect().width()*2;

            //The amount of space behind the other player. Two sides for jump to left and right
            //            int rightSpaceBetweenCameraAndSPlayer = maxCamera - (aSecondFighter->x() + aSecondFighter->boundingRect().width());
            //            int leftSpaceBetweenCameraAndSPlayer = aSecondFighter->x() - minCamera;

            /*To make sure we can't move beyond the border of the map and not only the camera
             * By addind / removing this amount from the begin position of the jump
             * Then change the new jump limit *//*
            if((aFirstFighter->x() - rightSpaceBetweenCameraAndSPlayer) > (- itsSceneWidth/2))
                minCamera -= rightSpaceBetweenCameraAndSPlayer;
            if((aFirstFighter->x() + aFirstFighter->boundingRect().x() + leftSpaceBetweenCameraAndSPlayer) < (itsSceneWidth/2))
                maxCamera += leftSpaceBetweenCameraAndSPlayer;*/

            //If the jump would go further than the limit of the map then set the limit at the value of the border
            /*if((aFirstFighter->x() - jumpSideSize) <= - itsSceneWidth/2){
                minCamera = - itsSceneWidth/2;
            }
            if((aFirstFighter->x() + aFirstFighter->boundingRect().width() + jumpSideSize) >= itsSceneWidth/2){
                maxCamera = itsSceneWidth/2;
            }*/

            aFirstFighter->setCameraMinScreenLimit(minCamera);
            aFirstFighter->setCameraMaxScreenLimit(maxCamera);

            aFirstFighter->jumpDirection();

            //Reset the values of jumping
            aFirstFighter->setIsJumpingRight(false);
            aFirstFighter->setIsJumpingLeft(false);
        }

        //To make the fighter jump
        if(aFirstFighter->getCanJump() && aFirstFighter->y() > aSecondFighter->boundingRect().height()*-2.5)
            aFirstFighter->jump();

    }
}

/* main game loop */
void FightScene::fightLoop()
{
    // Side swap
    if((itsFirstFighter->x()) > (itsSecondFighter->x())+((itsSecondFighter->boundingRect().width())/2) &&
            itsFirstFighter->getItsDirection() == FACERIGHT)
    {
        itsFirstFighter->setItsDirection(FACELEFT);
        itsSecondFighter->setItsDirection(FACERIGHT);
    }

    if((itsFirstFighter->x()) < (itsSecondFighter->x())+((itsSecondFighter->boundingRect().width())/2) &&
            itsFirstFighter->getItsDirection() == FACELEFT)
    {
        itsFirstFighter->setItsDirection(FACERIGHT);
        itsSecondFighter->setItsDirection(FACELEFT);
    }

    // First Fighter movement
    fighterMovementManager(itsFirstFighter,itsSecondFighter);

    // Second Fighter movement
    fighterMovementManager(itsSecondFighter,itsFirstFighter);


    /* ====== EVENTS HANDLING ====== */

    updateEventManager();

    if (fightIsInPause) {
        for (Rock *aRock : itsRocks) {
            aRock->pauseRockAnimation();
        }
    }

    for (Rock *aRock : itsRocks){
        if(aRock->toDelete()) {
            removeItem(aRock);
            itsRocks.erase(itsRocks.begin());
            delete aRock;
        }
    }

    /* ====== EVENTS HANDLING END ====== */

    /* Special Attack handler */
    specialAttackHandler();

    /* the freeze event locks the camera */
    if(itsActualEvent != Event::FREEZE) {
        /* middle of the two fighters position */
        int newSceneXPos;
        if(itsFirstFighter->getItsDirection() == FACERIGHT)
            newSceneXPos = (itsFirstFighter->pos().x()+itsFirstFighter->boundingRect().width()+itsSecondFighter->pos().x())/2;
        else
            newSceneXPos = (itsFirstFighter->pos().x()+itsSecondFighter->boundingRect().width()+itsSecondFighter->pos().x())/2;


        /* Checks if a scene limit (left/right) is reached.
         * 1920 here represents the screen size
         */
        if((newSceneXPos-itsSceneWidth/2) > -1920/2) {
            /* left scene limit reached
             * blocking position to this limit...
             */
            newSceneXPos = (-1920/2)+(itsSceneWidth/2);
        } else if((newSceneXPos+itsSceneWidth/2) < 1920/2) {
            /* right scene limit reached
             * blocking position to this limit...
             */
            newSceneXPos = (1920/2)-(itsSceneWidth/2);
        }

        /* center the event img if there is one */
        if (itsEventImg != nullptr)
            itsEventImg->setPos(newSceneXPos-44,-530);

        /* move the SA img if there is one */
        if (itsFirstFighterSpecialAttackImg != nullptr){
            itsFirstFighterSpecialAttackImg->setPos(newSceneXPos-1920/2+50,1080/2-250);
        }

        /* move the SA img if there is one */
        if (itsSecondFighterSpecialAttackImg != nullptr){
            itsSecondFighterSpecialAttackImg->setPos(newSceneXPos+1920/2-160,1080/2-250);
        }

        /* center the view on the fight */
        itsView->centerOn(newSceneXPos,0);
    }

    /* update life bars */
    updateLifeBars();
    updatePowerBars();

    /* Test End of Round */
    if(!endOfRoundIsBeingTested && !endOfRoundIsRunning){
        endOfRoundIsBeingTested = true;
        endOfRound();
    }

    /* Update Players names position & fight timer position */
    int centerCamera = itsView->mapToScene(itsView->viewport()->rect().center()).x();

    itsFirstFighterNameText->setPos(centerCamera-910,-450);

    itsSecondFighterNameText->setPos(centerCamera+910-itsSecondFighterNameText->boundingRect().width(),-450);

    itsTimerTextForFightScene->setPos(centerCamera-itsTimerTextForFightScene->boundingRect().width()/2,-450);

    /* Display if a fighter have won round */
    displayRoundWon();




    /* Collision manager for both player */

    QString hitMessage;

    hitMessage = itsFirstFighter->hitMessageIfHit(itsSecondFighter);
    if( hitMessage != "no message" && itsFirstFighterHitText == nullptr)
    {
        itsFirstFighter->addPower(5);

        itsFirstFighterHitText = new QGraphicsTextItem(hitMessage);
        itsFirstFighterHitText->setFont(btnFont);
        itsFirstFighterHitText->setDefaultTextColor(Qt::white);
        itsFirstFighterHitText->setPos(itsSecondFighter->pos() +
                                       QPointF(itsFirstFighterHitText->boundingRect().width()/2,
                                               (itsFirstFighterHitText->boundingRect().height()/2)-100));
        addItem(itsFirstFighterHitText);


        // Delete first fighter hit message
        if(itsFirstFighterHitText != nullptr){
            QTimer::singleShot(200, itsFirstFighterHitText, [=](){
                removeItem(itsFirstFighterHitText);
                delete itsFirstFighterHitText;
                itsFirstFighterHitText = nullptr;
            });
        }

    }

    hitMessage = itsSecondFighter->hitMessageIfHit(itsFirstFighter);
    if( hitMessage != "no message" && itsSecondFighterHitText == nullptr)
    {
        itsSecondFighter->addPower(5);

        itsSecondFighterHitText = new QGraphicsTextItem(hitMessage);
        itsSecondFighterHitText->setFont(btnFont);
        itsSecondFighterHitText->setDefaultTextColor(Qt::white);
        itsSecondFighterHitText->setPos(itsFirstFighter->pos() +
                                        QPointF(itsSecondFighterHitText->boundingRect().width()/2,
                                                (itsSecondFighterHitText->boundingRect().height()/2)-100));
        addItem(itsSecondFighterHitText);


        // Delete second fighter hit message
        if(itsSecondFighterHitText != nullptr){
            QTimer::singleShot(200, itsSecondFighterHitText, [=](){
                removeItem(itsSecondFighterHitText);
                delete itsSecondFighterHitText;
                itsSecondFighterHitText = nullptr;
            });
        }
    }

    // Refresh scene (anti-aliasing)
    update(sceneRect());
}


/* Handle key press for players movements and attacks */
void FightScene::keyPressEvent(QKeyEvent *event)
{
    //if we are in break only escape work
    if(!fightIsInPause)
    {
        /* First fighter movement & attacks handling */
        if(event->key() == keys->getLeft1())
        {
            // Blocking if the fighter goes in the opposite way of it's opponent
            if((itsSecondFighter->getItsAnimationState() == PUNCHCROUCH
                || itsSecondFighter->getItsAnimationState() == PUNCHSTANDING)
                    && itsSecondFighter->getItsDirection() == FACELEFT)
            {
                if(itsFirstFighter->getItsAnimationState() == CROUCH)
                    itsFirstFighter->setItsAnimationState(BLOCKINGCROUCH);
                else if(itsFirstFighter->getItsAnimationState() == STATIC)
                    itsFirstFighter->setItsAnimationState(BLOCKINGSTANDING);
            }else
                itsFirstFighter->setIsGoingLeft(true);

        }
        else if (event->key() == keys->getRight1()){

            // Blocking if the fighter goes in the opposite way of it's opponent
            if((itsSecondFighter->getItsAnimationState() == PUNCHCROUCH
                || itsSecondFighter->getItsAnimationState() == PUNCHSTANDING)
                    && itsSecondFighter->getItsDirection() == FACERIGHT)
            {
                if(itsFirstFighter->getItsAnimationState() == CROUCH)
                    itsFirstFighter->setItsAnimationState(BLOCKINGCROUCH);
                else if(itsFirstFighter->getItsAnimationState() == STATIC)
                    itsFirstFighter->setItsAnimationState(BLOCKINGSTANDING);
            }else
                itsFirstFighter->setIsGoingRight(true);

        }
        else if (event->key() == keys->getJump1())
        {
            itsFirstFighter->setCanJump(true);
            QTimer::singleShot(100, itsFirstFighter, [=](){
                itsFirstFighter->setCanJump(false);
            });
        }
        else if (event->key() == keys->getCrouch1())
            itsFirstFighter->setItsAnimationState(CROUCH);
        //Punch Series
        else if (event->key() == keys->getPunch1())
        {
            if (itsFirstFighter->getItsAnimationState() == CROUCH){
                itsFirstFighter->setItsAnimationState(PUNCHCROUCH);
            }
            else
                /* in normal time, we should check if the player CAN attack */
                itsFirstFighter->setItsAnimationState(PUNCHSTANDING);
        }
        //Kick Series
        else if(event->key() == keys->getFootKick1()){
            if(itsFirstFighter->getItsAnimationState() == CROUCH){
                itsFirstFighter->setItsAnimationState(SWEEPING);
            }
            else
                itsFirstFighter->setItsAnimationState(HIGHKICK);
        }
        // else if (event->key() == keys->getFootKick1()) ; // FOOTKICK KEY (not implemented yet)


        /* Second fighter movement & attacks handling */
        if(event->key() == keys->getLeft2())
        {
            // Blocking if the fighter goes in the opposite way of it's opponent
            if((itsFirstFighter->getItsAnimationState() == PUNCHCROUCH
                || itsFirstFighter->getItsAnimationState() == PUNCHSTANDING)
                    && itsFirstFighter->getItsDirection() == FACELEFT)
            {
                if(itsSecondFighter->getItsAnimationState() == CROUCH)
                    itsSecondFighter->setItsAnimationState(BLOCKINGCROUCH);
                else if(itsSecondFighter->getItsAnimationState() == STATIC)
                    itsSecondFighter->setItsAnimationState(BLOCKINGSTANDING);
            }else
                itsSecondFighter->setIsGoingLeft(true); // Walk toward left
        }
        else if (event->key() == keys->getRight2())
        {
            // Blocking if the fighter goes in the opposite way of it's opponent
            if((itsFirstFighter->getItsAnimationState() == PUNCHCROUCH
                || itsFirstFighter->getItsAnimationState() == PUNCHSTANDING)
                    && itsFirstFighter->getItsDirection() == FACERIGHT)
            {
                if(itsSecondFighter->getItsAnimationState() == CROUCH)
                    itsSecondFighter->setItsAnimationState(BLOCKINGCROUCH);
                else if(itsSecondFighter->getItsAnimationState() == STATIC)
                    itsSecondFighter->setItsAnimationState(BLOCKINGSTANDING);
            }else
                itsSecondFighter->setIsGoingRight(true); // Walk toward right
        }
        else if (event->key() == keys->getJump2())
        {
            // Jump
            itsSecondFighter->setCanJump(true);
            QTimer::singleShot(100, itsSecondFighter, [=](){itsSecondFighter->setCanJump(false);});
        }
        else if (event->key() == keys->getCrouch2())
            itsSecondFighter->setItsAnimationState(CROUCH); // Crouch
        //Punch Series
        else if (event->key() == keys->getPunch2())
        {
            if (itsSecondFighter->getItsAnimationState() == CROUCH)
                itsSecondFighter->setItsAnimationState(PUNCHCROUCH);
            else
                itsSecondFighter->setItsAnimationState(PUNCHSTANDING);
        }
        //Kick Series
        else if(event->key() == keys->getFootKick2()){
            if(itsSecondFighter->getItsAnimationState() == CROUCH){
                itsSecondFighter->setItsAnimationState(SWEEPING);
            }
            else
                itsSecondFighter->setItsAnimationState(HIGHKICK);
        }
        // else if (event->key() == keys->getFootKick2()) ; // FOOTKICK KEY (not implemented yet)

        /* ABCDE verify if there is an special attack combo */
    }


    // Pause game key = 'P'
    if(event->key() == Qt::Key_P)
        !fightIsInPause ? gameBreak() : gameResume();


    QGraphicsScene::keyPressEvent(event);

}


/* Handle key release for players movements */
void FightScene::keyReleaseEvent(QKeyEvent *event)
{
    /* First fighter keys */
    if(event->key() == keys->getLeft1())
        itsFirstFighter->setIsGoingLeft(false);
    else if(event->key() == keys->getRight1())
        itsFirstFighter->setIsGoingRight(false);
    else if (event->key() == keys->getCrouch1())
        itsFirstFighter->setItsAnimationState(STATIC);

    /* Second fighter keys */
    else if(event->key() == keys->getLeft2())
        itsSecondFighter->setIsGoingLeft(false);
    else if(event->key() == keys->getRight2())
        itsSecondFighter->setIsGoingRight(false);
    else if (event->key() == keys->getCrouch2())
        itsSecondFighter->setItsAnimationState(STATIC);

    QGraphicsScene::keyReleaseEvent(event);
}


/* Return the correct stylesheet (adaptative color), used by updateLifeBars() */
QString updateStyleSheet(int value, int baseHP)
{
    QString myColor;
    qfloat16 myBaseHP = qfloat16(baseHP);
    qfloat16 myValue = qfloat16(value);
    /*  ex : 70hp/100hp = 0.7, 0.7*100 > 50 -> if condition is valid
    ex : 30hp/100hp = 0.3, 0.3*100 > 50 -> if condition not valid -> else condition starts */
    if(100*(myValue/myBaseHP)>50) { /* set the color from 51 to 100% */
        // green set to 100% and red going from 0 to 255
        myColor = "rgb("+QString::number(255-(255*((myValue-myBaseHP/2)/(myBaseHP/2))))+",255,0);";
    } else { /* set the color from 0 to 50 */
        // red set to 100% and green going from 255 to 0
        myColor = "rgb(255,"+QString::number(255*(myValue/(myBaseHP/2)))+",0);";
    }
    return "QProgressBar { "
           "border: 2px solid grey;"
           "border-radius: 1px;"
           "text-align: center;"
           "}"
           "QProgressBar::chunk {"
           "background-color: " + myColor +
            "width: 1px;"
            "}";
}

/* Update the power bars of the fighters accordingly with their actual power and powerneed */
void FightScene::updatePowerBars()
{
    /* update the first power bars */
    int firstValue = itsFirstFighter->getItsPower(); if (firstValue>itsFirstFighter->getItsPowerNeed()) firstValue = itsFirstFighter->getItsPowerNeed();
    itsFirstFighterPowerBar1->setValue(firstValue);
    int secondValue = itsFirstFighter->getItsPower()-itsFirstFighter->getItsPowerNeed(); if (secondValue>itsFirstFighter->getItsPowerNeed()) secondValue = itsFirstFighter->getItsPowerNeed(); else if (secondValue<0) secondValue=0;
    itsFirstFighterPowerBar2->setValue(secondValue);
    int thirdValue = itsFirstFighter->getItsPower()-itsFirstFighter->getItsPowerNeed()*2; if (thirdValue>itsFirstFighter->getItsPowerNeed()) thirdValue = itsFirstFighter->getItsPowerNeed(); else if (thirdValue<0) thirdValue=0;
    itsFirstFighterPowerBar3->setValue(thirdValue);

    /* update the second power bar */
    firstValue = itsSecondFighter->getItsPower(); if (firstValue>itsSecondFighter->getItsPowerNeed()) firstValue = itsSecondFighter->getItsPowerNeed();
    itsSecondFighterPowerBar1->setValue(firstValue);
    secondValue = itsSecondFighter->getItsPower()-itsSecondFighter->getItsPowerNeed(); if (secondValue>itsSecondFighter->getItsPowerNeed()) secondValue = itsSecondFighter->getItsPowerNeed(); else if (secondValue<0) secondValue=0;
    itsSecondFighterPowerBar2->setValue(secondValue);
    thirdValue = itsSecondFighter->getItsPower()-itsSecondFighter->getItsPowerNeed()*2; if (thirdValue>itsSecondFighter->getItsPowerNeed()) thirdValue = itsSecondFighter->getItsPowerNeed(); else if (thirdValue<0) thirdValue=0;
    itsSecondFighterPowerBar3->setValue(thirdValue);

    // set the style sheet with adapted color
    itsFirstFighterPowerBar1->setStyleSheet(updateStyleSheet(itsFirstFighterPowerBar1->value(),itsFirstFighterPowerBar1->maximum()));
    itsFirstFighterPowerBar2->setStyleSheet(updateStyleSheet(itsFirstFighterPowerBar2->value(),itsFirstFighterPowerBar2->maximum()));
    itsFirstFighterPowerBar3->setStyleSheet(updateStyleSheet(itsFirstFighterPowerBar3->value(),itsFirstFighterPowerBar3->maximum()));
    // set the style sheet with adapted color
    itsSecondFighterPowerBar1->setStyleSheet(updateStyleSheet(itsSecondFighterPowerBar1->value(),itsSecondFighterPowerBar1->maximum()));
    itsSecondFighterPowerBar2->setStyleSheet(updateStyleSheet(itsSecondFighterPowerBar2->value(),itsSecondFighterPowerBar2->maximum()));
    itsSecondFighterPowerBar3->setStyleSheet(updateStyleSheet(itsSecondFighterPowerBar3->value(),itsSecondFighterPowerBar3->maximum()));
}

/* Update the life bars of the fighters accordingly with their HP */
void FightScene::updateLifeBars()
{
    /* if statements protects againts useless setters */
    /* timer delay represents the time the attack take to visually reach its opponent (hitbox is faster than sprites) */
    if (itsFirstFighterLifeB->value() != itsFirstFighter->getItsHP()) {
        QTimer::singleShot(200,this,[this](){
            /* update the first life bar */
            itsFirstFighterLifeB->setValue(itsFirstFighter->getItsHP());
            // set the style sheet with adapted color
            itsFirstFighterLifeB->setStyleSheet(updateStyleSheet(itsFirstFighter->getItsHP(),itsFirstFighter->getItsBaseHP()));
        });
    }

    if (itsSecondFighterLifeB->value() != itsSecondFighter->getItsHP()) {
        QTimer::singleShot(200,this,[this](){
            /* update the second life bar */
            itsSecondFighterLifeB->setValue(itsSecondFighter->getItsHP());
            // set the style sheet with adapted color
            itsSecondFighterLifeB->setStyleSheet(updateStyleSheet(itsSecondFighter->getItsHP(),itsSecondFighter->getItsBaseHP()));
        });
    }
}


/* Initialize fighter's names in order to place them above their life bars*/
void FightScene::displayFightersNames()
{
    itsFirstFighterNameText->setPlainText(itsFirstFighter->getItsName());
    itsFirstFighterNameText->setFont(btnFont);
    itsFirstFighterNameText->setDefaultTextColor(Qt::white);
    addItem(itsFirstFighterNameText);

    itsSecondFighterNameText->setPlainText(itsSecondFighter->getItsName());
    itsSecondFighterNameText->setFont(btnFont);
    itsSecondFighterNameText->setDefaultTextColor(Qt::white);
    addItem(itsSecondFighterNameText);
}



/* End the round if one of the fighter is dead */
void FightScene::endOfRound()
{
    if(!endOfRoundIsRunning){
        if(!itsFirstFighter->isAlive() || !itsSecondFighter->isAlive())
        {
            /* set event as not allowed */
            itsActualEvent = Event::NOTALLOWED;

            // If the first fighter is dead the second fighter won a point, else the first fighter won the point
            if(!itsFirstFighter->isAlive())
            {
                itsSecondFighter->setItsRoundWon(itsSecondFighter->getItsRoundWon()+1);
                /*Changing the animation */
                itsSecondFighter->setItsAnimationState(WIN);
                itsFirstFighter->setItsAnimationState(LOST);
            }
            else
            {
                itsFirstFighter->setItsRoundWon(itsFirstFighter->getItsRoundWon()+1);
                /*Changing the animation */
                itsFirstFighter->setItsAnimationState(WIN);
                itsSecondFighter->setItsAnimationState(LOST);
            }

            if(itsSecondFighter->getItsRoundWon() >= 2)
                // Second fighter win
                displayWinner(itsSecondFighter);
            else if(itsFirstFighter->getItsRoundWon() >= 2)
                // First fighter win
                displayWinner(itsFirstFighter);
            else
                displayEndOfRound(); // If none of the fighter have 2 points, we only display the winner of the round

        }
        else if(itsFirstFighter->isAlive() && itsSecondFighter->isAlive())
            endOfRoundIsBeingTested = false; // If they are both alive we continue to test it
    }
}


/* Displaying the winner of the round */
void FightScene::displayEndOfRound()
{
    int centerCamera = itsView->mapToScene(itsView->viewport()->rect().center()).x();

    endOfRoundIsRunning = true;

    /* End of the round text */
    itsEndRoundText = new QGraphicsTextItem("ROUND END");
    itsEndRoundText->setFont(btnFont);
    itsEndRoundText->setDefaultTextColor(Qt::white);
    addItem(itsEndRoundText);
    itsEndRoundText->setPos(centerCamera-itsEndRoundText->boundingRect().width()/2,-200);

    /* Display round winner text */
    itsRoundWinnerText = new QGraphicsTextItem("The winner is " + (
                                                   itsFirstFighter->isAlive() ?
                                                       itsFirstFighter->getItsName() : itsSecondFighter->getItsName()));
    itsRoundWinnerText->setFont(btnFont);
    itsRoundWinnerText->setDefaultTextColor(Qt::white);
    addItem(itsRoundWinnerText);

    itsRoundWinnerText->setPos(centerCamera-itsRoundWinnerText->boundingRect().width()/2,-150);


    // Locking fighters' attacks
    itsSecondFighter->setCanAttack(false);
    itsFirstFighter->setCanAttack(false);

    //Locking fighters' movements
    fightIsInPause = true;

    // hide the text after a short time and start the next round
    QTimer::singleShot(1000*4, this, [=]()
    {
        if(itsEndRoundText != nullptr)
        {
            removeItem(itsEndRoundText);
            delete itsEndRoundText;
            itsEndRoundText = nullptr;
        }
        if(itsRoundWinnerText != nullptr){
            removeItem(itsRoundWinnerText);
            delete itsRoundWinnerText;
            itsRoundWinnerText = nullptr;
        }


        resetFightForNewRound();

        /*Changing the animation */
        itsFirstFighter->setItsAnimationState(STATIC);
        itsSecondFighter->setItsAnimationState(STATIC);

        endOfRoundIsRunning = false;
    });
}


/* Display the winner of the fight */
void FightScene::displayWinner(Fighter *aFighter)
{
    int centerCamera = itsView->mapToScene(itsView->viewport()->rect().center()).x();

    /* End of the fight text */
    itsEndGameText->setPlainText("VICTORY");
    itsEndGameText->setFont(btnFont);
    itsEndGameText->setDefaultTextColor(Qt::white);
    addItem(itsEndGameText);
    itsEndGameText->setPos(QPointF(centerCamera-itsEndGameText->boundingRect().width()/2,-200));

    /* Display fight winner text */
    itsFightWinnerText->setPlainText("The winner is " + aFighter->getItsName() );
    itsFightWinnerText->setFont(btnFont);
    itsFightWinnerText->setDefaultTextColor(Qt::white);
    addItem(itsFightWinnerText);
    itsFightWinnerText->setPos(QPointF(centerCamera-itsFightWinnerText->boundingRect().width()/2,-150));



    // Hiding fighter's life bars
    itsFirstFighterLifeB->setVisible(false);
    itsSecondFighterLifeB->setVisible(false);
    itsFirstFighterPowerBar1->setVisible(false);
    itsFirstFighterPowerBar2->setVisible(false);
    itsFirstFighterPowerBar3->setVisible(false);
    itsSecondFighterPowerBar1->setVisible(false);
    itsSecondFighterPowerBar2->setVisible(false);
    itsSecondFighterPowerBar3->setVisible(false);

    //Hiding fighter's name
    removeItem(itsFirstFighterNameText);
    removeItem(itsSecondFighterNameText);

    //Hiding the totem for round's won
    removeItem(itsRoundWonPixMapForFirstFighter);
    removeItem(itsRoundWonPixMapForSecondFighter);

    //Remove the timer of the scene
    removeItem(itsTimerTextForFightScene);

    // Locking fighter's attacks
    itsSecondFighter->setCanAttack(false);
    itsFirstFighter->setCanAttack(false);

    //Locking fighters' movements
    fightIsInPause = true;

    // Going to end game menu
    QTimer::singleShot(1000*3, this, [=](){
        emit returnBackToMenu();
    });
}


/* Special attack handler verify and apply the effects of a special attack */
void FightScene::specialAttackHandler()
{
    /* handling on first fighter */
    if (firstFighterSARemainingTime == 0 && firstFighterIsSA) {

        itsFirstFighter->removeSpecialAttack();

        /* remove & delete the SA img */
        removeItem(itsFirstFighterSpecialAttackImg);
        delete itsFirstFighterSpecialAttackImg;
        itsFirstFighterSpecialAttackImg = nullptr;

        firstFighterIsSA = false;

    } else if (!firstFighterIsSA) {
        /* check if an SA -can- be started */
        if (itsFirstFighter->getItsPower() >= itsFirstFighter->getItsPowerNeed() && !firstFighterIsSA && !fightIsInPause) {

            /* checks if the current direction is valid for a special attack start */
            bool goodDirection = (itsFirstFighter->getItsDirection()==FACELEFT && itsFirstFighter->getIsGoingLeft()) ||
                    (itsFirstFighter->getItsDirection()==FACERIGHT && itsFirstFighter->getIsGoingRight());

            /* checks if the good key combo is currently applied */
            if (itsFirstFighter->getItsAnimationState()==PUNCHCROUCH && goodDirection) {

                int timeIndicator = (itsFirstFighter->getItsPower()-3*itsFirstFighter->getItsPowerNeed())>=0?2:
                                                                                                             (itsFirstFighter->getItsPower()-2*itsFirstFighter->getItsPowerNeed())>=0?1:0;;

                firstFighterSARemainingTime = itsFirstFighter->itsCaracteristics[timeIndicator];

                itsFirstFighter->specialAttack();

                /* as the special attack is the same for every fighter, display the same LOGO */
                itsFirstFighterSpecialAttackImg = new QGraphicsPixmapItem(QPixmap(":/specialAttackLogo.png").scaled(120,120));
                addItem(itsFirstFighterSpecialAttackImg);

                /* removing SA points & setting the state to true */
                itsFirstFighter->addPower(-itsFirstFighter->getItsPowerNeed()*(timeIndicator+1));
                firstFighterIsSA = true;
            }
        }
    }

    /* handling on second fighter */
    if (secondFighterSARemainingTime == 0 && secondFighterIsSA) {

        itsSecondFighter->removeSpecialAttack();

        /* remove & delete the SA img */
        removeItem(itsSecondFighterSpecialAttackImg);
        delete itsSecondFighterSpecialAttackImg;
        itsSecondFighterSpecialAttackImg = nullptr;

        secondFighterIsSA = false;

    } else if (!secondFighterIsSA) {
        /* check if an SA -can- be started */
        if (itsSecondFighter->getItsPower() >= itsSecondFighter->getItsPowerNeed() && !secondFighterIsSA && !fightIsInPause) {

            /* checks if the current direction is valid for a special attack start */
            bool goodDirection = (itsSecondFighter->getItsDirection()==FACELEFT && itsSecondFighter->getIsGoingLeft()) ||
                    (itsSecondFighter->getItsDirection()==FACERIGHT && itsSecondFighter->getIsGoingRight());

            /* checks if the good key combo is currently applied */
            if (itsSecondFighter->getItsAnimationState()==PUNCHCROUCH && goodDirection) {

                int timeIndicator = (itsSecondFighter->getItsPower()-3*itsSecondFighter->getItsPowerNeed())>=0?2:
                                                                                                               (itsSecondFighter->getItsPower()-2*itsSecondFighter->getItsPowerNeed())>=0?1:0;;

                secondFighterSARemainingTime = itsSecondFighter->itsCaracteristics[timeIndicator];

                itsSecondFighter->specialAttack();

                /* as the special attack is the same for every fighter, display the same LOGO */
                itsSecondFighterSpecialAttackImg = new QGraphicsPixmapItem(QPixmap(":/specialAttackLogo.png").scaled(120,120));
                addItem(itsSecondFighterSpecialAttackImg);

                /* removing SA points & setting the state to true */
                itsSecondFighter->addPower(-itsSecondFighter->getItsPowerNeed()*(timeIndicator+1));
                secondFighterIsSA = true;
            }
        }
    }
}
/* event manager */
void FightScene::updateEventManager()
{
    /* so we don't start a new event while one is already running */
    if(itsRemainingEventTime==0 && !isEventSingleShotRunning) {

        /* an event appears each 15 to 30 seconds in a game. This timer only count time when none event is running */
        remainingTimeBeforeEvent = QRandomGenerator::global()->bounded(15,31); /* 15 is included 31 is excluded */

        isEventSingleShotRunning = true;

    } else if (remainingTimeBeforeEvent == 0) {
        /* additional security */
        if(itsActualEvent != Event::NONE) return;

        /* pick a random event in a list of events */
        itsActualEvent = pickRandomEvent();
        itsEventImg = new QGraphicsPixmapItem(QPixmap(":/"+(strEvent(itsActualEvent))+".png"));
        addItem(itsEventImg);

        /* middle of the two fighters position */
        int newSceneXPos;
        if(itsFirstFighter->getItsDirection() == FACERIGHT)
            newSceneXPos = (itsFirstFighter->pos().x()+itsFirstFighter->boundingRect().width()+itsSecondFighter->pos().x())/2;
        else
            newSceneXPos = (itsFirstFighter->pos().x()+itsSecondFighter->boundingRect().width()+itsSecondFighter->pos().x())/2;


        /* Checks if a scene limit (left/right) is reached.
                 * 1920 here represents the screen size
                 */
        if((newSceneXPos-itsSceneWidth/2) > -1920/2) {
            /* left scene limit reached
                     * blocking position to this limit...
                     */
            newSceneXPos = (-1920/2)+(itsSceneWidth/2);
        } else if((newSceneXPos+itsSceneWidth/2) < 1920/2) {
            /* right scene limit reached
                     * blocking position to this limit...
                     */
            newSceneXPos = (1920/2)-(itsSceneWidth/2);
        }

        /* center the event img if there is one */
        if (itsEventImg != nullptr)
            itsEventImg->setPos(newSceneXPos-44,-530);

        // ==== FLASH EVENT ====
        /* apply the effect of the FLASH event */
        if(itsActualEvent==Event::FLASH){
            //add image flash
            itsRemainingEventTime = 10;

            itsFirstFighter->setItsSpeed(2);
            itsSecondFighter->setItsSpeed(2);
        }

        // ==== INSTANT DEATH EVENT ====
        /* apply the effect of the INSTANT DEATH event */
        if(itsActualEvent==Event::INSTANTDEATH){

            // add death image
            itsRemainingEventTime = 5;

            itsFormerHPFirstFighter = itsFirstFighter->getItsHP();
            itsFirstFighter->setItsHP(1);
            itsFormerHPSecondFighter = itsSecondFighter->getItsHP();
            itsSecondFighter->setItsHP(1);
        }

        // ==== ROCK FALL EVENT ====
        /* apply the effect of the ROCK FALL event */
        if (itsActualEvent==Event::ROCKFALL){

            itsRemainingEventTime = 10;

            /* all effects applied in fighttimer */
        }
        if (itsActualEvent==Event::FREEZE){
            itsRemainingEventTime = 10;
        }
        isEventSingleShotRunning = false;
    }
    else if (itsRemainingEventTime == 0) {
        /* check if an event has come to an end */
        if (itsActualEvent != Event::NONE && itsRemainingEventTime == 0) {


            /* additional security */
            if (itsActualEvent == Event::NONE) return;

            removeItem(itsEventImg);
            delete itsEventImg;
            itsEventImg = nullptr;


            /* reversing effects of each event type */
            switch (itsActualEvent) {
            case Event::FLASH:
                itsFirstFighter->setItsSpeed(1);
                itsSecondFighter->setItsSpeed(1);
                break;
            case Event::INSTANTDEATH:
                itsFirstFighter->setItsHP(itsFormerHPFirstFighter);
                itsSecondFighter->setItsHP(itsFormerHPSecondFighter);
                break;
            default:break;
            }

            /* set the actual event to NONE */
            itsActualEvent = Event::NONE;
        }
    }

}

void FightScene::resetEvent()
{
    itsRemainingEventTime = 0;
}

/*  */
Event FightScene::pickRandomEvent()
{
    std::vector<Event> eventVector {Event::FLASH,Event::FLASH,Event::INSTANTDEATH,Event::FREEZE,Event::FREEZE,Event::ROCKFALL,Event::ROCKFALL,Event::ROCKFALL,Event::ROCKFALL};
    int randnb = QRandomGenerator::global()->bounded(0,9); //0 is included 9 is excluded
    return eventVector[randnb];
}

/* creates a rockfall, with floor alert and rock falling */
void FightScene::createRockFall()
{
    /* get the screen actual center */
    int mapCenter = itsView->mapToScene(itsView->viewport()->rect().center()).x();

    /* get a random pos to place the rockfall -within the screen borders- */
    int xCoord = QRandomGenerator::global()->bounded(mapCenter-1920/2+100,mapCenter+1920/2-100);

    /* adding a floor alert to the scene */
    QGraphicsPixmapItem * aMark = new QGraphicsPixmapItem(QPixmap(":/Mark.png"));
    addItem(aMark);
    aMark->setPos(xCoord,320);
    itsMarks.push_back(aMark);

    QTimer::singleShot(1000,[=](){
        Rock *aRock = new Rock(xCoord,this,itsFirstFighter,itsSecondFighter,&itsRocks);
        addItem(aRock);

        itsRocks.push_back(aRock);
    });
}

/* Reset the fight for a new round */
void FightScene::resetFightForNewRound()
{
    // Fighters can attack
    itsSecondFighter->setCanAttack(true);
    itsFirstFighter->setCanAttack(true);

    /* delete all rocks if some are still on the scene */
    for (Rock *aRock : itsRocks) {
        aRock->setToDelete(true);
    }

    // Fighters can move
    fightIsInPause = false;

    //Reset Fighters life
    itsFirstFighter->setItsHP(itsFirstFighter->getItsBaseHP());
    itsSecondFighter->setItsHP(itsSecondFighter->getItsBaseHP());

    /* reset power points */
    itsFirstFighter->addPower(-itsFirstFighter->getItsPower());
    itsSecondFighter->addPower(-itsSecondFighter->getItsPower());

    // Reset first fighter
    itsFirstFighter->setItsDirection(FACERIGHT);
    addItem(itsFirstFighter);
    itsFirstFighter->setX(-50-itsFirstFighter->boundingRect().width());

    // Reset second fighter
    itsSecondFighter->setItsDirection(FACELEFT);
    addItem(itsSecondFighter);
    itsSecondFighter->setX(50);

    //Changing the animation of fighter's
    itsFirstFighter->setItsAnimationState(STATIC);
    itsSecondFighter->setItsAnimationState(STATIC);

    /* middle of the two fighters position */
    int newSceneXPos=itsSceneWidth;

    /* center the view on the fight */
    itsView->centerOn(newSceneXPos,0);

    // We can now continue to test if one of the fighter is dead
    endOfRoundIsBeingTested = false;

    // reset events
    resetEvent();
}


/* Reset the fight for a replay */
void FightScene::resetFightForReplayAndStartIt()
{
    // Reset fighter's score
    itsFirstFighter->setItsRoundWon(0);
    itsSecondFighter->setItsRoundWon(0);

    /* delete all rocks if some are still on the scene */
    for (Rock *aRock : itsRocks) {
        aRock->setToDelete(true);
    }

    /* reset SA remaining time */
    firstFighterSARemainingTime = 0;
    secondFighterSARemainingTime = 0;

    /* reset event */
    itsRemainingEventTime = 0;

    // Showing fighter's life bars back
    itsFirstFighterLifeB->setVisible(true);
    itsSecondFighterLifeB->setVisible(true);
    itsSecondFighterLifeB->setVisible(true);
    itsFirstFighterPowerBar1->setVisible(true);
    itsFirstFighterPowerBar2->setVisible(true);
    itsFirstFighterPowerBar3->setVisible(true);
    itsSecondFighterPowerBar1->setVisible(true);
    itsSecondFighterPowerBar2->setVisible(true);
    itsSecondFighterPowerBar3->setVisible(true);

    /* reset power points */
    itsFirstFighter->addPower(-itsFirstFighter->getItsPower());
    itsSecondFighter->addPower(-itsSecondFighter->getItsPower());

    /*Showing fighter's name*/
    addItem(itsFirstFighterNameText);
    addItem(itsSecondFighterNameText);

    //initialize the timer of the scene
    addItem(itsTimerTextForFightScene);
    seconde = 0;

    /*Iniatialize the totem for roundWon*/
    addItem(itsRoundWonPixMapForFirstFighter);
    addItem(itsRoundWonPixMapForSecondFighter);

    itsRoundWonPixMapForFirstFighter->hide();
    itsRoundWonPixMapForSecondFighter->hide();


    // Showing texts
    itsEndGameText->hide();
    itsFightWinnerText->hide();

    resetFightForNewRound();
}


/* Translate event to QString */
QString FightScene::strEvent(Event anEvent)
{
    QString eventStr = "NONE";
    switch (anEvent)
    {
    case Event::FREEZE :        eventStr = "FREEZE";        break;
    case Event::FLASH :         eventStr="FLASH";           break;
    case Event::INSTANTDEATH :  eventStr="INSTANTDEATH";    break;
    case Event::ROCKFALL :      eventStr= "ROCKFALL";       break;
    }
    return eventStr;
}




/* Pause the fight */
void FightScene::gameBreak()
{
    int centerCamera = itsView->mapToScene(itsView->viewport()->rect().center()).x();

    fightIsInPause = true;

    // Locking fighter's attacks
    itsSecondFighter->setCanAttack(false);
    itsFirstFighter->setCanAttack(false);

    itsBreakText = new QGraphicsTextItem("PRESS P KEY TO RESUME");
    itsBreakText->setFont(btnFont);
    itsBreakText->setDefaultTextColor(Qt::white);
    itsBreakText->setPos(centerCamera-itsBreakText->boundingRect().width()/2,-200);
    addItem(itsBreakText);
}


/* Resume the fight */
void FightScene::gameResume()
{
    itsBreakText->hide();

    for (Rock *aRock : itsRocks) {
        aRock->resumeRockAnimation();
    }

    fightIsInPause = false;

    // Fighters can attack back
    itsSecondFighter->setCanAttack(true);
    itsFirstFighter->setCanAttack(true);
}

void FightScene::displayRoundWon()
{
    int centerCamera = itsView->mapToScene(itsView->viewport()->rect().center()).x();

    if(itsFirstFighter->getItsRoundWon() == 1)
    {
        if(!itsRoundWonPixMapForFirstFighter->isVisible())
        {
            itsRoundWonPixMapForFirstFighter->show();
        }
        itsRoundWonPixMapForFirstFighter->setPos(centerCamera-910,-400);
    }
    if(itsSecondFighter->getItsRoundWon()==1)
    {
        if(!itsRoundWonPixMapForSecondFighter->isVisible())
        {
            itsRoundWonPixMapForSecondFighter->show();
        }
        itsRoundWonPixMapForSecondFighter->setPos(centerCamera+850,-400);
    }
}

void FightScene::displayTimerOnFightScene()
{
    if (!fightIsInPause) {
        seconde++;

        // To have the format 2:05
        QString secondeQstring;
        if(seconde%60 < 10)
            secondeQstring = "0"+QString::number(seconde % 60);
        else
            secondeQstring = QString::number(seconde % 60);

        QString minuteQstring = QString::number(seconde / 60);

        itsTimerTextForFightScene->setPlainText(minuteQstring+" : "+secondeQstring);


        /* ==== some event handling ==== */
        /* handling rock event */
        if (itsActualEvent == Event::ROCKFALL) {
            createRockFall();
        }
        for (QGraphicsPixmapItem *item : toBeDeletedMarks) {
            removeItem(item);
            delete item;
        }
        toBeDeletedMarks.clear();
        for (QGraphicsPixmapItem *item : itsMarks) {
            toBeDeletedMarks.push_back(item);
        }
        itsMarks.clear();

        /* decreasing remaining event time */
        itsRemainingEventTime --;
        if (remainingTimeBeforeEvent != 0)
            remainingTimeBeforeEvent --;

        /* decreasing remaining SA time */
        if (firstFighterSARemainingTime!=0) firstFighterSARemainingTime --;
        if (secondFighterSARemainingTime!=0) secondFighterSARemainingTime --;

        itsFirstFighter->addPower(1);
        itsSecondFighter->addPower(1);
    }
}

Event FightScene::getItsActualEvent() const { return itsActualEvent; }

void FightScene::setItsActualEvent(Event newItsActualEvent) { itsActualEvent = newItsActualEvent; }
