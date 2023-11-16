#include "menuscene.h"

MenuScene::MenuScene(Key * keyManager, QObject *parent) : QGraphicsScene(parent),itsKeyManager(keyManager)
{
    // Center origin
    setSceneRect(-itsSceneWidth/2,-itsSceneHeight/2,itsSceneWidth,itsSceneHeight);

    // Disable index method (the scene has moving items)
    setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);

    // Add scene's background and initialize it
    addItem(itsBackground);
    itsBackground->setOpacity(0);
    itsBackground->setBg(MENUBACKGROUND);
    // Center background
    itsBackground->setPos(QPointF(0-itsSceneWidth/2,0-itsSceneHeight/2)
                          - QPointF(itsBackground->boundingRect().width()*0.66666,
                                    itsBackground->boundingRect().height()*0.66666));

    // Initialize menu state
    setItsState(STARTMENU);
}

MenuScene::~MenuScene()
{
    hideGraphics();
    delete itsBackground;
}


/* Set scene's state and update the scene accordingly */
void MenuScene::setItsState(MenuState newItsState)
{
    hideGraphics();

    /* Stopping unused timers when we change the scene's state */
    if(itsStartMenuAnimTimer->isActive())
        itsStartMenuAnimTimer->stop();

    if(itsMenuState != newItsState)
        itsMenuState = newItsState;

    /* Stopping unused timers when we change the scene's state */
    if(itsChoiceMenuAnimTimer->isActive())
        itsChoiceMenuAnimTimer->stop();

    switch (itsMenuState)
    {
    case STARTMENU:
        addStartMenuGraphics();
        break;
    case CHOICECONTROLS:

        /* Reset game mode selection for next game */
        itsSelectedGameMode = GameMode::NULLV;

        if(!controlsAreShown){
            controlsAreShown = true;
            showControls();
        }
        break;
    case CHOICEGAMEMODE:
        itsBackground->setOpacity(1); // Lock background opacity
        addGameModeChoiceMenu();
        break;
    case CHOICEMAP:
        itsFirstPlayerMapSelected = NULLV;
        itsSecondPlayerMapSelected = NULLV;
        addMapChoiceMenu();
        break;
    case CHOICEFIGHTER:
        itsFirstPlayerFighterSelected = nullptr;
        itsSecondPlayerFighterSelected = nullptr;
        addFighterChoiceMenu();
        break;
    case CHOICEENDGAME :
        addEndGameChoiceMenu();
        break;
    case REPLAY :
        emit askForReplay();
        break;
    }
}


/* Hide graphics that are on the scene (Reset scene) */
void MenuScene::hideGraphics()
{
    /* Start menu */
    if(itsStartMenuLogo != nullptr){
        removeItem(itsStartMenuLogo);
        delete itsStartMenuLogo;
        itsStartMenuLogo = nullptr;
    }
    if(itsStartInstructionText != nullptr){
        removeItem(itsStartInstructionText);
        delete itsStartInstructionText;
        itsStartInstructionText = nullptr;
    }
    if(itsWhiteRect != nullptr){
        removeItem(itsWhiteRect);
        delete itsWhiteRect;
        itsWhiteRect = nullptr;
    }


    /* Game mode choice Menu */
    if(arcadeModeII != nullptr){
        removeItem(arcadeModeII);
        delete arcadeModeII;
        arcadeModeII = nullptr;
    }
    if(arcadeModeTextForII != nullptr){
        removeItem(arcadeModeTextForII);
        delete arcadeModeTextForII;
        arcadeModeTextForII = nullptr;
    }
    if(campaignModeII != nullptr){
        removeItem(campaignModeII);
        delete campaignModeII;
        campaignModeII = nullptr;
    }
    if(campaignModeTextForII != nullptr){
        removeItem(campaignModeTextForII);
        delete campaignModeTextForII;
        campaignModeTextForII = nullptr;
    }
    if(controlsII != nullptr){
        removeItem(controlsII);
        delete controlsII;
        controlsII = nullptr;
    }
    if(controlsTextForII != nullptr){
        removeItem(controlsTextForII);
        delete controlsTextForII;
        controlsTextForII = nullptr;
    }
    if(itsStarCursor != nullptr){
        removeItem(itsStarCursor);
        delete itsStarCursor;
        itsStarCursor = nullptr;
    }


    /* Fighter choice Menu */
    if(itsChooseFighterText != nullptr){
        removeItem(itsChooseFighterText);
        delete itsChooseFighterText;
        itsChooseFighterText = nullptr;
    }
    for (QGraphicsPixmapItem * item : itsFightersMugShotsGPI) {
        removeItem(item);
        delete item;
        item = nullptr;
    }
    itsFightersMugShotsGPI.clear();

    if(itsFirstPlayerSelector != nullptr){
        removeItem(itsFirstPlayerSelector);
        delete itsFirstPlayerSelector;
        itsFirstPlayerSelector = nullptr;
    }
    if(itsSecondPlayerSelector != nullptr){
        removeItem(itsSecondPlayerSelector);
        delete itsSecondPlayerSelector;
        itsSecondPlayerSelector = nullptr;
    }

    /* Map choice Menu */
    if(itsChooseMapText != nullptr){
        removeItem(itsChooseMapText);
        delete itsChooseMapText;
        itsChooseMapText = nullptr;
    }
    for (QGraphicsPixmapItem * item : itsMapsMugShotsGPI) {
        removeItem(item);
        delete item;
        item = nullptr;
    }
    itsMapsMugShotsGPI.clear();


    /* End Game Choice Menu */
    if(replayII != nullptr){
        removeItem(replayII);
        delete replayII;
        replayII = nullptr;
    }
    if(replayTextForII != nullptr){
        removeItem(replayTextForII);
        delete replayTextForII;
        replayTextForII = nullptr;
    }
    if(endGameChoiceMenuII != nullptr){
        removeItem(endGameChoiceMenuII);
        delete endGameChoiceMenuII;
        endGameChoiceMenuII = nullptr;
    }
    if(endGameChoiceMenuTextForII != nullptr){
        removeItem(endGameChoiceMenuTextForII);
        delete endGameChoiceMenuTextForII;
        endGameChoiceMenuTextForII = nullptr;
    }



    /* Controls graphics */
    controlsAreShown = false;

    if(itsGoBackButtonII != nullptr){
        removeItem(itsGoBackButtonII);
        delete itsGoBackButtonII;
        itsGoBackButtonII = nullptr;
    }
    if(itsSaveButtonII != nullptr){
        removeItem(itsSaveButtonII);
        delete itsSaveButtonII;
        itsSaveButtonII = nullptr;
    }
    if(itsPlayerOneName != nullptr){
        removeItem(itsPlayerOneName);
        delete itsPlayerOneName;
        itsPlayerOneName = nullptr;
    }
    if(itsPlayerTwoName != nullptr){
        removeItem(itsPlayerTwoName);
        delete itsPlayerTwoName;
        itsPlayerTwoName = nullptr;
    }
    if(itsPlayersSeparationLine != nullptr){
        removeItem(itsPlayersSeparationLine);
        delete itsPlayersSeparationLine;
        itsPlayersSeparationLine = nullptr;
    }
    for (InteractiveImage * item : itsControlsIICollection) {
        removeItem(item);
        delete item;
        item = nullptr;
    }
    itsControlsIICollection.clear();
    if(!itsControlsTextForIICollection.empty()){
        for (QGraphicsTextItem * item : itsControlsTextForIICollection) {
            removeItem(item);
            delete item;
            item = nullptr;
        }
        itsControlsTextForIICollection.clear();
    }
    for (QGraphicsTextItem * item : itsControlsIndicationTextCollection) {
        removeItem(item);
        delete item;
        item = nullptr;
    }
    itsControlsIndicationTextCollection.clear();
    if(itsSaveButtonTextForII != nullptr){
        removeItem(itsSaveButtonTextForII);
        delete itsSaveButtonTextForII;
        itsSaveButtonTextForII = nullptr;
    }
    if(itsGoBackButtonTextForII != nullptr){
        removeItem(itsGoBackButtonTextForII);
        delete itsGoBackButtonTextForII;
        itsGoBackButtonTextForII = nullptr;
    }
}


/* Add game choice menu :
 * the player choose between arcade mode and campaign mode,
 * by moving a star cursor.
 */
void MenuScene::addGameModeChoiceMenu()
{
    /* Start menu white rect */
    itsWhiteRect = new QGraphicsPixmapItem();
    addItem(itsWhiteRect);


    /* Arcade mode button */
    arcadeModeII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(300,54)));
    addItem(arcadeModeII);
    arcadeModeII->setPos(QPointF(0,0) - QPointF(arcadeModeII->boundingRect().width()/2,
                                                (arcadeModeII->boundingRect().height()/2)+150));

    connect(arcadeModeII,&InteractiveImage::clickOnBtnImg,[=](){
        itsSelectedGameMode = GameMode::ARCADE;

        setItsState(CHOICEFIGHTER);
    });

    arcadeModeTextForII = new QGraphicsTextItem("ARCADE");
    arcadeModeTextForII->setFont(btnFont);
    arcadeModeTextForII->setDefaultTextColor(Qt::white);
    addItem(arcadeModeTextForII);
    arcadeModeTextForII->setPos(QPointF(0,0) - QPointF(arcadeModeTextForII->boundingRect().width()/2,
                                                       (arcadeModeTextForII->boundingRect().height()/2)+150));


    /* Campaign mode button */
    campaignModeII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(300,54)));
    addItem(campaignModeII);
    campaignModeII->setPos(QPointF(0,0) - QPointF(campaignModeII->boundingRect().width()/2,
                                                  (campaignModeII->boundingRect().height()/2)+50));

    connect(campaignModeII,&InteractiveImage::clickOnBtnImg,[=](){
        itsSelectedGameMode = GameMode::CAMPAIGN;

        setItsState(CHOICEFIGHTER);
    });

    campaignModeTextForII = new QGraphicsTextItem("CAMPAIGN");
    campaignModeTextForII->setFont(btnFont);
    campaignModeTextForII->setDefaultTextColor(Qt::white);
    addItem(campaignModeTextForII);
    campaignModeTextForII->setPos(QPointF(0,0) - QPointF(campaignModeTextForII->boundingRect().width()/2,
                                                         (campaignModeTextForII->boundingRect().height()/2)+50));


    /* Controls menu button */
    controlsII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(300,54)));
    addItem(controlsII);
    controlsII->setPos(QPointF(0,0) - QPointF(controlsII->boundingRect().width()/2,
                                              (controlsII->boundingRect().height()/2)-50));

    connect(controlsII,&InteractiveImage::clickOnBtnImg,[=](){
        setItsState(CHOICECONTROLS);
    });

    controlsTextForII = new QGraphicsTextItem("CONTROLS");
    controlsTextForII->setFont(btnFont);
    controlsTextForII->setDefaultTextColor(Qt::white);
    addItem(controlsTextForII);
    controlsTextForII->setPos(QPointF(0,0) - QPointF(controlsTextForII->boundingRect().width()/2,
                                                     (controlsTextForII->boundingRect().height()/2)-50));



    /* Star cursor */
    itsStarCursor = new QGraphicsPixmapItem(QPixmap(":/star.png").scaled(QSize(100,100)));
    addItem(itsStarCursor);
    itsStarCursor->setPos(QPointF(0,0) - QPointF(itsStarCursor->boundingRect().width()/2,
                                                 (itsStarCursor->boundingRect().height()/2)) - QPointF(arcadeModeII->boundingRect().width(),0));
    itsStarCursor->setPos(QPointF(itsStarCursor->pos().x(),
                                  arcadeModeII->pos().y()-arcadeModeII->boundingRect().height()/2));



    /* Start menu white rect Settings */
    itsWhiteRect->setPixmap(QPixmap(":/WhiteRectangle.png").scaled(QSize(arcadeModeII->boundingRect().width()*1.5,
                                                                         arcadeModeII->boundingRect().height()*3+150)));

    itsWhiteRect->setOffset(-QPointF(itsWhiteRect->boundingRect().width()/2,
                                     (itsWhiteRect->boundingRect().height()/2)));

    itsWhiteRect->setPos(campaignModeII->pos() + QPointF(campaignModeII->boundingRect().width()/2,
                                                         (campaignModeII->boundingRect().height()/2)));
}


/* Add map choice menu (only in arcade mode) :
 * the players choose a map, where they'll fight,
 * by moving two selectors.
 */
void MenuScene::addMapChoiceMenu()
{

    if(itsSelectedGameMode == GameMode::ARCADE)
    {

        /* White rect */
        itsWhiteRect = new QGraphicsPixmapItem();
        addItem(itsWhiteRect);


        /* Text asking the players to choose their map */
        itsChooseMapText = new QGraphicsTextItem("Choose your Map !");
        itsChooseMapText->setFont(btnFont);
        itsChooseMapText->setDefaultTextColor(Qt::red);
        addItem(itsChooseMapText);
        itsChooseMapText->setPos(QPointF(0,0) - QPointF(itsChooseMapText->boundingRect().width()/2,
                                                        (itsChooseMapText->boundingRect().height()/2)+475));

        /* Displaying Map mugshots */
        int z = 0;
        int y = -1; // specific value

        for (int i = 1; i < 7 ; i++ )
        {

            itsMapMugshot = new QGraphicsPixmapItem();

            QString fileTitle = QString::fromStdString(":/mapMugshot" + std::to_string(i) + ".jpg");
            itsMapMugshot->setPixmap(QPixmap(fileTitle).scaled(QSize(768,224)));
            addItem(itsMapMugshot);

            itsMapsMugShotsGPI.push_back(itsMapMugshot);

            itsMapMugshot->setPos(QPointF(z*itsMapMugshot->boundingRect().width()*1.2,y*itsMapMugshot->boundingRect().height()*1.5)
                                  - QPointF(itsMapMugshot->boundingRect().width()/2,itsMapMugshot->boundingRect().height()/2));

            itsMapMugshot->setPos(itsMapMugshot->pos() - QPointF(itsSceneWidth/4,0));

            if(y >= 1){
                z++;
                y = -2;
            }
            y++;

        }
    }

    /* Start menu white rect Settings */
    itsWhiteRect->setPixmap(QPixmap(":/WhiteRectangle.png").scaled(QSize(itsMapMugshot->boundingRect().width()*4,
                                                                         itsMapMugshot->boundingRect().height()*3+itsMapMugshot->boundingRect().height()*1.5)));

    itsWhiteRect->setOffset(-QPointF(itsWhiteRect->boundingRect().width()/2,
                                     (itsWhiteRect->boundingRect().height()/2)));

    itsWhiteRect->setPos(0,0);


    /* Adding players selectors */
    itsFirstPlayerSelector = new QGraphicsPixmapItem(QPixmap(":/firstPlayerSelector.png").scaled(QSize(768,224)));
    addItem(itsFirstPlayerSelector);
    itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());

    itsSecondPlayerSelector = new QGraphicsPixmapItem(QPixmap(":/secondPlayerSelector.png").scaled(QSize(768,224)));
    addItem(itsSecondPlayerSelector);
    itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());

}


/* Manager for selectors movements for map selection :
                       * - Q: Left for first player selector
                       * - D: Right for first player selector
                       * - S: Down for first player selector
                       * - Z: Up for first player selector
                       *
                       * - 6: Right for second player selector
                       * - 4: Left for second player selector
                       * - 5: Down for second player selector
                       * - 8: Up for second player selector
                       *
                       * - 0: Validate choice for second player
                       * - Space: Validate choice for first player
                       */
void MenuScene::selectorMovementsForMapSelection(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        if(itsFirstPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(3)->pos().x())
        {
            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(1)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(2)->pos());
        }
        break;
    case Qt::Key_D:
        if(itsFirstPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(0)->pos().x())
        {
            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(3)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(4)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(5)->pos());
        }
        break;
    case Qt::Key_6:
        if(itsSecondPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(0)->pos().x())
        {
            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(3)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(4)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(5)->pos());
        }
        break;
    case Qt::Key_4:
        if(itsSecondPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(3)->pos().x())
        {
            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(1)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(2)->pos());
        }
        break;
    case Qt::Key_S:
        if(itsFirstPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(0)->pos().x())
        {
            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(1)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(2)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());
        }
        if(itsFirstPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(3)->pos().x())
        {
            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(4)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(5)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(3)->pos());
        }
        break;
    case Qt::Key_5:
        if(itsSecondPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(0)->pos().x())
        {
            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(1)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(2)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());
        }
        if(itsSecondPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(3)->pos().x())
        {
            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(4)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(5)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(3)->pos());
        }
        break;
    case Qt::Key_8:
        if(itsSecondPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(0)->pos().x())
        {
            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(2)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(1)->pos());
        }
        if(itsSecondPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(3)->pos().x())
        {
            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(5)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(3)->pos());
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsSecondPlayerSelector->setPos(itsMapsMugShotsGPI.at(4)->pos());
        }
        break;
    case Qt::Key_Z:
        if(itsFirstPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(0)->pos().x())
        {
            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(2)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(0)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(1)->pos());
        }
        if(itsFirstPlayerSelector->pos().x() == itsMapsMugShotsGPI.at(3)->pos().x())
        {
            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(5)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(3)->pos());
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsFirstPlayerSelector->setPos(itsMapsMugShotsGPI.at(4)->pos());
        }
        break;
    case Qt::Key_Space:
        if(!isBeingValidated){
            isBeingValidated = true;
            QTimer::singleShot(50, this, [=](){isBeingValidated = false;});

            if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsFirstPlayerMapSelected = MAPMAX;
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsFirstPlayerMapSelected = MAPNOAH;
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsFirstPlayerMapSelected = MAPANTHO;
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsFirstPlayerMapSelected = MAPARTHUR;
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsFirstPlayerMapSelected = MAPCYRIL;
            else if(itsFirstPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsFirstPlayerMapSelected = MAPNILS;

        }
        break;
    case Qt::Key_0:
        if(!isBeingValidated){
            isBeingValidated = true;
            QTimer::singleShot(50, this, [=](){isBeingValidated = false;});

            if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(0)->pos())
                itsSecondPlayerMapSelected = MAPMAX;
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(1)->pos())
                itsSecondPlayerMapSelected = MAPNOAH;
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(2)->pos())
                itsSecondPlayerMapSelected = MAPANTHO;
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(3)->pos())
                itsSecondPlayerMapSelected = MAPARTHUR;
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(4)->pos())
                itsSecondPlayerMapSelected = MAPCYRIL;
            else if(itsSecondPlayerSelector->pos() == itsMapsMugShotsGPI.at(5)->pos())
                itsSecondPlayerMapSelected = MAPNILS;
        }
        break;
    }

    /* If the two players have chosen their maps */
    QTimer::singleShot(50, this, [=](){
        if(itsFirstPlayerMapSelected != NULLV && itsSecondPlayerMapSelected != NULLV)
        {
            // If they haven't chosen the same map, we randomize the chosen map between their choices
            if(itsFirstPlayerMapSelected != itsSecondPlayerMapSelected){
                int randnb = QRandomGenerator::global()->bounded(1, 3);//1 is included 3 is excluded
                itsSelectedMap = randnb%2 == 0 ? itsFirstPlayerMapSelected : itsSecondPlayerMapSelected;
            }
            // else if they have chosen the same map, they play on it
            else
                itsSelectedMap = itsFirstPlayerMapSelected;

            // When they made every choice, we start the fight
            askForNewFight();
        }
    });


    QGraphicsScene::keyPressEvent(event);
}


/* Start the fight + security in order to start it only once
                       * Hide the menuscene graphics.
                       */
void MenuScene::askForNewFight()
{
    if(!gameIsStarted)
    {
        gameIsStarted = true;

        /* Reset game mode selection for next game */
        itsSelectedGameMode = GameMode::NULLV;

        hideGraphics();
        emit startArcadeFight(itsFirstPlayerFighterSelected,
                              itsSecondPlayerFighterSelected);
    }
}


/* Display start menu */
void MenuScene::addStartMenuGraphics()
{

    /* Start menu logo */
    itsStartMenuLogo = new QGraphicsPixmapItem(QPixmap(":/IUTFighterLogo.png").scaled(QSize(436*3,236*3)));
    itsStartMenuLogo->setOpacity(0);
    itsStartMenuLogo->setPos(QPointF(0,0) - QPointF(itsStartMenuLogo->boundingRect().width()/2,
                                                    (itsStartMenuLogo->boundingRect().height()/2)+150));
    addItem(itsStartMenuLogo);


    /* Text asking the player(s) to press space to start */
    itsStartInstructionText = new QGraphicsTextItem("Press SPACE to start !");
    itsStartInstructionText->setFont(QFont("Upheaval TT (BRK)", 50, QFont::Bold));
    itsStartInstructionText->setOpacity(0);
    itsStartInstructionText->setDefaultTextColor(Qt::red);

    /* Start menu white rect for logo */
    itsWhiteRect = new QGraphicsPixmapItem(QPixmap(":/WhiteRectangle.png").scaled(QSize(itsStartInstructionText->boundingRect().width()*2,
                                                                                        itsStartInstructionText->boundingRect().height()*2)));
    itsWhiteRect->setOpacity(0);
    itsWhiteRect->setPos(QPointF(0,0) - QPointF(itsWhiteRect->boundingRect().width()/2,
                                                (itsWhiteRect->boundingRect().height()/2)-400));
    addItem(itsWhiteRect);

    itsStartInstructionText->setPos(QPointF(0,0) - QPointF(itsStartInstructionText->boundingRect().width()/2,
                                                           (itsStartInstructionText->boundingRect().height()/2)-400));
    addItem(itsStartInstructionText);


    /* set animation timer to animate graphics opacity */
    tempOpacity = 0;
    connect(itsStartMenuAnimTimer, &QTimer::timeout, [=]() mutable{
        // Update background opacity = flash effect + cleaner start
        itsBackground->setOpacity(tempOpacity-0.3);

        if(tempOpacity <= 1){
            itsStartMenuLogo->setOpacity(tempOpacity);
            itsWhiteRect->setOpacity(tempOpacity);
            tempOpacity = tempOpacity + 0.04;
        }
        /* Timer loop to animate the "press space" text opacity */
        else if( tempOpacity >= 1 && tempOpacity <= 2 ){
            itsStartInstructionText->setOpacity(tempOpacity-0.8);
            tempOpacity = tempOpacity + 0.02;
            if(tempOpacity >= 2){ tempOpacity--;}
        }
    });

    itsStartMenuAnimTimer->start(30);
    /* This timer is stopped when we update the scene's state, by pressing space */

}



/* Add fighters choice menu :
 * the player(s) choose a(their) fighter(s),
 * by moving a(two) selector(s).
 */
void MenuScene::addFighterChoiceMenu()
{
    /* Select path accordingly with selected gamemode */
    if(itsSelectedGameMode == GameMode::CAMPAIGN)
    {
        // WIP
        QMessageBox errorBox;
        errorBox.setWindowTitle("Work in progress");
        errorBox.setText(" This module is still in progress ");
        errorBox.setIcon(QMessageBox::Critical);
        errorBox.setStandardButtons(QMessageBox::Close);
        errorBox.setDefaultButton(QMessageBox::Close);
        errorBox.exec();

        setItsState(CHOICEGAMEMODE); // Go to previous menu
    }
    else if(itsSelectedGameMode == GameMode::ARCADE) // Adding players selectors
    {
        /* White rect */
        itsWhiteRect = new QGraphicsPixmapItem();
        addItem(itsWhiteRect);


        /* Text asking the player(s) to choose a(their) fighter(s) */
        itsChooseFighterText = new QGraphicsTextItem("Choose your fighters !");
        itsChooseFighterText->setFont(btnFont);
        itsChooseFighterText->setDefaultTextColor(Qt::red);
        addItem(itsChooseFighterText);
        itsChooseFighterText->setPos(QPointF(0,0) - QPointF(itsChooseFighterText->boundingRect().width()/2,
                                                            (itsChooseFighterText->boundingRect().height()/2)+200));


        /* Displaying fighters mugshots */
        int z = -1;
        int y = 0; // specific value

        for (int i = 1; i< 7 ; i++ )
        {
            QString fileTitle = QString::fromStdString(":/mugshot" + std::to_string(i) + ".png");
            itsFighterMugShot = new QGraphicsPixmapItem(QPixmap(fileTitle).scaled(QSize(200,200)));
            addItem(itsFighterMugShot);

            itsFightersMugShotsGPI.push_back(itsFighterMugShot);

            itsFighterMugShot->setPos(QPointF(z*itsFighterMugShot->boundingRect().width(),y)
                                      - QPointF(itsFighterMugShot->boundingRect().width()/2,itsFighterMugShot->boundingRect().height()/2));

            if(z >= 1){
                z = -2;
                y = y+itsFighterMugShot->boundingRect().height()*1.5;
            }
            z++;

        }

        /* White rect settings*/
        itsWhiteRect->setPixmap(QPixmap(":/WhiteRectangle.png").scaled(QSize(itsFighterMugShot->boundingRect().width()*4,
                                                                             itsFighterMugShot->boundingRect().height()*3.5)));
        itsWhiteRect->setOffset(-QPointF(itsWhiteRect->boundingRect().width()/2,
                                         (itsWhiteRect->boundingRect().height()/2)-100));
        itsWhiteRect->setPos(QPointF(0,0));


        itsFirstPlayerSelector = new QGraphicsPixmapItem(QPixmap(":/firstPlayerSelector.png").scaled(QSize(200,200)));
        addItem(itsFirstPlayerSelector);
        itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(0)->pos());

        itsSecondPlayerSelector = new QGraphicsPixmapItem(QPixmap(":/secondPlayerSelector.png").scaled(QSize(200,200)));
        addItem(itsSecondPlayerSelector);
        itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(0)->pos());
    }
}


/* Manager for selectors movements for fighters selection :
                       * - Q: Left for first player selector
                       * - D: Right for first player selector
                       * - S: Down for first player selector
                       * - Z: Up for first player selector
                       *
                       * - 6: Right for second player selector
                       * - 4: Left for second player selector
                       * - 5: Down for second player selector
                       * - 8: Up for second player selector
                       *
                       * - 0: Validate choice for second player
                       * - Space: Validate choice for first player
                       */
void MenuScene::selectorMovementsForFighterSelection(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        if(itsFirstPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(0)->pos().y())
        {
            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(0)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(2)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(1)->pos());
        }
        else if(itsFirstPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(3)->pos().y())
        {
            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(3)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(5)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(4)->pos());
        }
        break;
    case Qt::Key_D:
        if(itsFirstPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(0)->pos().y())
        {
            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(2)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(0)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(1)->pos());
        }
        else if(itsFirstPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(3)->pos().y())
        {

            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(5)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(3)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(4)->pos());
        }
        break;
    case Qt::Key_6:
        if(itsSecondPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(0)->pos().y())
        {
            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(2)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(0)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(1)->pos());
        }
        else if(itsSecondPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(3)->pos().y())
        {

            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(5)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(3)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(4)->pos());
        }
        break;
    case Qt::Key_4:
        if(itsSecondPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(0)->pos().y())
        {
            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(0)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(2)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(1)->pos());
        }
        else if(itsSecondPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(3)->pos().y())
        {
            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(3)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(5)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(4)->pos());
        }
        break;
    case Qt::Key_S:
        if(itsFirstPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(0)->pos().y())
        {
            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(0)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(3)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(4)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(2)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(5)->pos());
        }
        break;
    case Qt::Key_5:
        if(itsSecondPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(0)->pos().y())
        {
            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(0)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(3)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(4)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(2)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(5)->pos());
        }
        break;
    case Qt::Key_8:
        if(itsSecondPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(3)->pos().y())
        {
            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(3)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(0)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(1)->pos());
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(5)->pos())
                itsSecondPlayerSelector->setPos(itsFightersMugShotsGPI.at(2)->pos());
        }
        break;
    case Qt::Key_Z:
        if(itsFirstPlayerSelector->pos().y() == itsFightersMugShotsGPI.at(3)->pos().y())
        {
            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(3)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(0)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(1)->pos());
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(5)->pos())
                itsFirstPlayerSelector->setPos(itsFightersMugShotsGPI.at(2)->pos());
        }
        break;
    case Qt::Key_Space:
        if(!isBeingValidated){
            isBeingValidated = true;
            QTimer::singleShot(50, this, [=](){isBeingValidated = false;});

            if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(0)->pos())
                itsFirstPlayerFighterSelected = new MaxFighter();
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsFirstPlayerFighterSelected = new NoahFighter(); // NOAH
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(2)->pos())
                itsFirstPlayerFighterSelected = new CyrilFighter(); // CYRIL
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(3)->pos())
                itsFirstPlayerFighterSelected = new ArthurFighter();  // ARTHUR
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsFirstPlayerFighterSelected = new AnthonyFighter(); // ANTHONY
            else if(itsFirstPlayerSelector->pos() == itsFightersMugShotsGPI.at(5)->pos())
                itsFirstPlayerFighterSelected = new NilsFighter();// NILS
        }
        break;
    case Qt::Key_0:
        if(!isBeingValidated){
            isBeingValidated = true;
            QTimer::singleShot(50, this, [=](){isBeingValidated = false;});

            if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(0)->pos())
                itsSecondPlayerFighterSelected = new MaxFighter();
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(1)->pos())
                itsSecondPlayerFighterSelected = new NoahFighter(); // NOAH
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(2)->pos())
                itsSecondPlayerFighterSelected = new CyrilFighter(); // CYRIL
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(3)->pos())
                itsSecondPlayerFighterSelected = new ArthurFighter(); // ARATHUR
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(4)->pos())
                itsSecondPlayerFighterSelected = new AnthonyFighter(); // ANTHONY
            else if(itsSecondPlayerSelector->pos() == itsFightersMugShotsGPI.at(5)->pos())
                itsSecondPlayerFighterSelected = new NilsFighter(); // NILS
        }
        break;
    }

    if(itsMenuState != CHOICEMAP)
        if(itsSecondPlayerFighterSelected != nullptr && itsFirstPlayerFighterSelected != nullptr){
            setItsState(CHOICEMAP);
        }

    QGraphicsScene::keyPressEvent(event);
}


/* Manager for Game mode star cursor
                       * - S,5: Down
                       * - Z,8: Up
                       * - 0,Space: Validate choice
                       */
void MenuScene::starMovementsForGameModeSelection(QKeyEvent *event)
{
    QPointF topStarPosition(itsStarCursor->pos().x(),arcadeModeII->pos().y()-arcadeModeII->boundingRect().height()/2);
    QPointF middleStarPosition(itsStarCursor->pos().x(),campaignModeII->pos().y()-campaignModeII->boundingRect().height()/2);
    QPointF bottomStarPosition(itsStarCursor->pos().x(),controlsII->pos().y()-controlsII->boundingRect().height()/2);

    switch (event->key())
    {
    case Qt::Key_5:
    case Qt::Key_S:
        // Go down star cursor
        if(itsStarCursor->pos().y() == campaignModeII->pos().y()-campaignModeII->boundingRect().height()/2)
            itsStarCursor->setPos(bottomStarPosition);
        else if(itsStarCursor->pos().y() == arcadeModeII->pos().y()-arcadeModeII->boundingRect().height()/2)
            itsStarCursor->setPos(middleStarPosition);
        else if(itsStarCursor->pos().y() == controlsII->pos().y()-controlsII->boundingRect().height()/2)
            itsStarCursor->setPos(topStarPosition);
        break;
    case Qt::Key_8:
    case Qt::Key_Z:
        // Go up star cursor
        if(itsStarCursor->pos().y() == arcadeModeII->pos().y()-arcadeModeII->boundingRect().height()/2)
            itsStarCursor->setPos(bottomStarPosition);
        else if(itsStarCursor->pos().y() == campaignModeII->pos().y()-campaignModeII->boundingRect().height()/2)
            itsStarCursor->setPos(topStarPosition);
        else if(itsStarCursor->pos().y() == controlsII->pos().y()-controlsII->boundingRect().height()/2)
            itsStarCursor->setPos(middleStarPosition);
        break;
    case Qt::Key_Space:
    case Qt::Key_0:
        if(!isBeingValidated){
            isBeingValidated = true;
            QTimer::singleShot(50, this, [=](){isBeingValidated = false;});
            // Validate choice with star cursor
            if(itsStarCursor->pos().y() == campaignModeII->pos().y()-campaignModeII->boundingRect().height()/2)
                itsSelectedGameMode = GameMode::CAMPAIGN;
            else if(itsStarCursor->pos().y() == arcadeModeII->pos().y()-arcadeModeII->boundingRect().height()/2)
                itsSelectedGameMode = GameMode::ARCADE;
            else if(itsStarCursor->pos().y() == controlsII->pos().y()-controlsII->boundingRect().height()/2)
                setItsState(CHOICECONTROLS);

            if((itsSelectedGameMode == GameMode::CAMPAIGN || itsSelectedGameMode == GameMode::ARCADE) && itsMenuState != CHOICEFIGHTER)
                setItsState(CHOICEFIGHTER);

        }
        break;
    }

    QGraphicsScene::keyPressEvent(event);
}


/* Key press event handler managing cursor movements in menus */
void MenuScene::keyReleaseEvent(QKeyEvent *event)
{
    // Send event to the manager for selectors movements, if the players are selecting their fighters
    if(itsMenuState == CHOICEFIGHTER && itsSelectedGameMode == GameMode::ARCADE)
        selectorMovementsForFighterSelection(event);

    // Send event to the manager for star movements, if the players are selecting their fighters
    if(itsMenuState == CHOICEGAMEMODE)
        starMovementsForGameModeSelection(event);

    // Send event to the manager for selectors movements, if the players are selecting their map
    if(itsMenuState == CHOICEMAP)
        selectorMovementsForMapSelection(event);

    // Send event to the manager for star movements, if the players are selecting their end game choice (replay or go back to main menu)
    if(itsMenuState == CHOICEENDGAME)
        starMovementsForEndGameChoiceSelection(event);

    // Send event to the manager for controls choices
    if(itsMenuState == CHOICECONTROLS)
        controlsSelectionManager(event);

    /* Display gamemode choice when space key is pressed */
    if(itsMenuState == STARTMENU && event->key() == Qt::Key_Space)
        setItsState(CHOICEGAMEMODE);


    QGraphicsScene::keyReleaseEvent(event);
}


/* Add End game choice menu :
                       * The player can replay the fight or go back to the main menu.
                       */
void MenuScene::addEndGameChoiceMenu()
{
    /* White rect */
    itsWhiteRect = new QGraphicsPixmapItem();
    addItem(itsWhiteRect);

    /* Replay mode button */
    replayII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(300,54)));
    addItem(replayII);
    replayII->setPos(QPointF(0,0) - QPointF(replayII->boundingRect().width()/2,
                                            (replayII->boundingRect().height()/2)+150));

    connect(replayII,&InteractiveImage::clickOnBtnImg,[=](){
        setItsState(REPLAY);
    });

    replayTextForII = new QGraphicsTextItem("REPLAY");
    replayTextForII->setFont(btnFont);
    replayTextForII->setDefaultTextColor(Qt::white);
    addItem(replayTextForII);
    replayTextForII->setPos(QPointF(0,0) - QPointF(replayTextForII->boundingRect().width()/2,
                                                   (replayTextForII->boundingRect().height()/2)+150));


    /* Main menu button */
    endGameChoiceMenuII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(300,54)));
    addItem(endGameChoiceMenuII);
    endGameChoiceMenuII->setPos(QPointF(0,0) - QPointF(endGameChoiceMenuII->boundingRect().width()/2,
                                                       (endGameChoiceMenuII->boundingRect().height()/2)+50));

    connect(endGameChoiceMenuII,&InteractiveImage::clickOnBtnImg,[=](){
        setItsState(CHOICEGAMEMODE);
    });

    endGameChoiceMenuTextForII = new QGraphicsTextItem("MENU");
    endGameChoiceMenuTextForII->setFont(btnFont);
    endGameChoiceMenuTextForII->setDefaultTextColor(Qt::white);
    addItem(endGameChoiceMenuTextForII);
    endGameChoiceMenuTextForII->setPos(QPointF(0,0) - QPointF(endGameChoiceMenuTextForII->boundingRect().width()/2,
                                                              (endGameChoiceMenuTextForII->boundingRect().height()/2)+50));


    /* Star cursor */
    itsStarCursor = new QGraphicsPixmapItem(QPixmap(":/star.png").scaled(QSize(100,100)));
    addItem(itsStarCursor);
    itsStarCursor->setPos(QPointF(0,0) - QPointF(itsStarCursor->boundingRect().width()/2,
                                                 (itsStarCursor->boundingRect().height()/2)) - QPointF(replayII->boundingRect().width(),0));
    itsStarCursor->setPos(QPointF(itsStarCursor->pos().x(),replayII->pos().y()-replayII->boundingRect().height()/2));


    /* End menu white rect Settings */
    itsWhiteRect->setPixmap(QPixmap(":/WhiteRectangle.png").scaled(QSize(replayII->boundingRect().width()*1.5,
                                                                         replayII->boundingRect().height()*3+25)));

    itsWhiteRect->setOffset(-QPointF(itsWhiteRect->boundingRect().width()/2,
                                     (itsWhiteRect->boundingRect().height()/2)));

    itsWhiteRect->setPos(replayII->pos() + QPointF(replayII->boundingRect().width()/2,
                                                   (replayII->boundingRect().height()/2)+50));
}


/* Manager for End game choice star cursor
                       * - S,5: Down
                       * - Z,8: Up
                       * - 0,Space: Validate choice
                       */
void MenuScene::starMovementsForEndGameChoiceSelection(QKeyEvent *event)
{
    QPointF topStarPosition(itsStarCursor->pos().x(),replayII->pos().y()-replayII->boundingRect().height()/2);
    QPointF bottomStarPosition(itsStarCursor->pos().x(),endGameChoiceMenuII->pos().y()-endGameChoiceMenuII->boundingRect().height()/2);

    switch (event->key())
    {
    case Qt::Key_5:
    case Qt::Key_S:
        // Go down star cursor
        if(itsStarCursor->pos().y() == endGameChoiceMenuII->pos().y()-endGameChoiceMenuII->boundingRect().height()/2)
            itsStarCursor->setPos(topStarPosition);
        else if(itsStarCursor->pos().y() == replayII->pos().y()-replayII->boundingRect().height()/2)
            itsStarCursor->setPos(bottomStarPosition);
        break;
    case Qt::Key_8:
    case Qt::Key_Z:
        // Go up star cursor
        if(itsStarCursor->pos().y() == replayII->pos().y()-replayII->boundingRect().height()/2)
            itsStarCursor->setPos(bottomStarPosition);
        else if(itsStarCursor->pos().y() == endGameChoiceMenuII->pos().y()-endGameChoiceMenuII->boundingRect().height()/2)
            itsStarCursor->setPos(topStarPosition);
        break;
    case Qt::Key_Space:
    case Qt::Key_0:
        if(!isBeingValidated){
            isBeingValidated = true;
            QTimer::singleShot(50, this, [=](){isBeingValidated = false;});
            // Validate choice with star cursor
            if(itsStarCursor->pos().y() == replayII->pos().y()-replayII->boundingRect().height()/2)
                setItsState(REPLAY);
            else if(itsStarCursor->pos().y() == endGameChoiceMenuII->pos().y()-endGameChoiceMenuII->boundingRect().height()/2)
                setItsState(CHOICEGAMEMODE);
        }
        break;
    }
    QGraphicsScene::keyPressEvent(event);
}


/* Display controls */
void MenuScene::showControls()
{
    /* White rect */
    itsWhiteRect = new QGraphicsPixmapItem();
    addItem(itsWhiteRect);
    itsWhiteRect->setPixmap(QPixmap(":/WhiteRectangle.png").scaled(QSize(1920-50,1080-150)));

    itsWhiteRect->setOffset(-QPointF(itsWhiteRect->boundingRect().width()/2,
                                     (itsWhiteRect->boundingRect().height()/2)));

    itsWhiteRect->setPos(QPointF(0,0));


    /* Players names */
    itsPlayerOneName = new QGraphicsTextItem("Player one");
    itsPlayerOneName->setFont(btnFont);
    itsPlayerOneName->setDefaultTextColor(Qt::red);
    itsPlayerOneName->setPos(-itsSceneWidth/2+50,-itsSceneHeight/2+100);
    addItem(itsPlayerOneName);

    itsPlayerTwoName = new QGraphicsTextItem("Player two");
    itsPlayerTwoName->setFont(btnFont);
    itsPlayerTwoName->setDefaultTextColor(Qt::red);
    itsPlayerTwoName->setPos(-itsSceneWidth/2+50,-itsSceneHeight/2+550);
    addItem(itsPlayerTwoName);


    // Separation line
    itsPlayersSeparationLine = new QGraphicsLineItem(-itsSceneWidth/2+50,-itsSceneHeight/2+550,100,-itsSceneHeight/2+550);
    QPen myPen;
    myPen.setWidth(8);
    myPen.setColor(Qt::red);
    itsPlayersSeparationLine->setPen(myPen);
    addItem(itsPlayersSeparationLine);


    /* Displaying first player controls : jump1, left1, right1, crouch1, punch1, footkick1 */
    /* Displaying second player controls : jump2, left2, right2, crouch2, punch2, footkick2 */
    int heightRatio = 0;
    for (int i = 0; i < 12 ; i++ )
    {
        if(heightRatio > 2) heightRatio = 0;
        heightRatio++;

        /* Control button */
        if(i < 6){
            itsControlII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(itsKeyManager->keyToQString(itsKeyManager->getItsFirstPlayerKeys().at(i)).size()*60,50)));
            itsControlII->setPos(QPointF((itsControlII->boundingRect().width())-800 * (i > 2 ? 0 : 1),
                                         (itsPlayerOneName->pos().y()/2)+100*heightRatio) + QPointF(0,-210));
        }else if( i >= 6){
            itsControlII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(QSize(itsKeyManager->keyToQString(itsKeyManager->getItsSecondPlayerKeys().at(i-6)).size()*60,50)));
            itsControlII->setPos(QPointF(180 -(itsControlII->boundingRect().width())-800 * (i > 8 ? 0 : 1),
                                         (itsPlayerTwoName->pos().y()/2)+100*heightRatio));
        }

        addItem(itsControlII);

        if(itsControlII->pos().x() == 300)
            itsControlII->setPos(QPointF(60,itsControlII->pos().y()));
        else if(itsControlII->pos().x() == -860)
            itsControlII->setPos(QPointF(-740,itsControlII->pos().y()));

        itsControlsIICollection.push_back(itsControlII);

        /* Text for key on control button */
        if(i < 6){
            itsControlTextForII = new QGraphicsTextItem(itsKeyManager->keyToQString(itsKeyManager->getItsFirstPlayerKeys().at(i)));
        }else if( i >= 6){
            itsControlTextForII = new QGraphicsTextItem(itsKeyManager->keyToQString(itsKeyManager->getItsSecondPlayerKeys().at(i-6)));
        }

        itsControlTextForII->setFont(btnFont);
        itsControlTextForII->setDefaultTextColor(Qt::white);
        addItem(itsControlTextForII);
        itsControlTextForII->setPos(itsControlII->pos()
                                    + QPointF(itsControlII->boundingRect().width()/2,
                                              (itsControlII->boundingRect().height()/2))
                                    - QPointF(itsControlTextForII->boundingRect().width()/2,
                                              (itsControlTextForII->boundingRect().height()/2)));

        itsControlsTextForIICollection.push_back(itsControlTextForII);



        /* Indication text : jump, left, right, ... */
        itsControlIndicationText = new QGraphicsTextItem();
        itsControlIndicationText->setFont(btnFont);
        itsControlIndicationText->setDefaultTextColor(Qt::red);
        itsControlIndicationText->setPos(itsControlII->pos()
                                         + QPointF(itsControlII->boundingRect().width()/2,
                                                   (itsControlII->boundingRect().height()/2))
                                         - QPointF(itsControlIndicationText->boundingRect().width()/2,
                                                   (itsControlIndicationText->boundingRect().height()/2))
                                         + QPointF(itsControlTextForII->boundingRect().width(),0));

        itsControlsIndicationTextCollection.push_back(itsControlIndicationText);

        switch (i) {
        case 0:case 6:itsControlIndicationText->setPlainText("  Jump"); break;
        case 2:case 8:itsControlIndicationText->setPlainText("  Left"); break;
        case 3:case 9:itsControlIndicationText->setPlainText(" Right"); break;
        case 1:case 7:itsControlIndicationText->setPlainText("  Crouch"); break;
        case 4:case 10:itsControlIndicationText->setPlainText("  Punch"); break;
        case 5:case 11:itsControlIndicationText->setPlainText("  Kick "); break;
        }

        addItem(itsControlIndicationText);


        /* Connect each II in order to change the key selected */
        connect(itsControlII,&InteractiveImage::clickOnBtnImg,[=]()
        {
            if(i < 6){
                itsSelectedKey = itsKeyManager->getItsFirstPlayerKeys().at(i);
            }
            else if( i >= 6){
                itsSelectedKey = itsKeyManager->getItsSecondPlayerKeys().at(i-6);
            }
            linkedControlTextForII = itsControlsTextForIICollection.at(i);
            linkedControlII = itsControlsIICollection.at(i);
            linkedControlIndicationText = itsControlsIndicationTextCollection.at(i);
        });
    }


    /* Basic actions buttons */
    // Go back to previous menu button
    itsGoBackButtonII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(300,50));
    itsGoBackButtonII->setOffset(-QPointF(itsGoBackButtonII->boundingRect().width()/2,
                                          itsGoBackButtonII->boundingRect().height()/2));
    itsGoBackButtonII->setPos(itsPlayerTwoName->pos().x()/2-itsGoBackButtonII->boundingRect().width(),itsPlayerTwoName->pos().y()+400);
    addItem(itsGoBackButtonII);

    connect(itsGoBackButtonII,&InteractiveImage::clickOnBtnImg,this,[=](){
        setItsState(CHOICEGAMEMODE); // Go back to main menu
    });

    itsGoBackButtonTextForII = new QGraphicsTextItem("GO BACK");
    itsGoBackButtonTextForII->setFont(btnFont);
    itsGoBackButtonTextForII->setDefaultTextColor(Qt::white);
    itsGoBackButtonTextForII->setPos(itsGoBackButtonII->pos()
                                     - QPointF(itsGoBackButtonTextForII->boundingRect().width()/2,
                                               itsGoBackButtonTextForII->boundingRect().height()/2));
    addItem(itsGoBackButtonTextForII);

    // Save button
    itsSaveButtonII = new InteractiveImage(QPixmap(":/nullimg.png").scaled(300,50));
    itsSaveButtonII->setOffset(-QPointF(itsSaveButtonII->boundingRect().width()/2,
                                        itsSaveButtonII->boundingRect().height()/2));
    itsSaveButtonII->setPos(-itsPlayerTwoName->pos().x()-itsSaveButtonII->boundingRect().width(),itsPlayerTwoName->pos().y()+400);
    addItem(itsSaveButtonII);


    connect(itsSaveButtonII,&InteractiveImage::clickOnBtnImg,this,[=](){

        QString presetQStr = "Default config";
        for(QGraphicsTextItem *item : itsControlsTextForIICollection){
            presetQStr +=  ';' + item->toPlainText();
        }

        itsKeyManager->updateKeyPreset(presetQStr);

        qDebug() << presetQStr;

        QMessageBox validationBox;
        validationBox.setWindowTitle("Keys changed");
        validationBox.setText(" Your key preset is saved ! ");
        validationBox.setIcon(QMessageBox::Information);
        validationBox.setStandardButtons(QMessageBox::Ok);
        validationBox.setDefaultButton(QMessageBox::Ok);
        validationBox.exec();

    });

    itsSaveButtonTextForII = new QGraphicsTextItem("SAVE");
    itsSaveButtonTextForII->setFont(btnFont);
    itsSaveButtonTextForII->setDefaultTextColor(Qt::white);
    itsSaveButtonTextForII->setPos(itsSaveButtonII->pos()
                                   - QPointF(itsSaveButtonTextForII->boundingRect().width()/2,
                                             itsSaveButtonTextForII->boundingRect().height()/2));
    addItem(itsSaveButtonTextForII);




    // Once every graphics is set, we can change the controls
    canBeChanged = true;


}


void MenuScene::controlsSelectionManager(QKeyEvent *event)
{
    if(canBeChanged && itsKeyManager->isValidKey(event->key())){

        switch (event->key())
        {
        case Qt::Key_A:     itsSelectedKey = Qt::Key_A; break;
        case Qt::Key_B:     itsSelectedKey = Qt::Key_B; break;
        case Qt::Key_C:     itsSelectedKey = Qt::Key_C; break;
        case Qt::Key_D:     itsSelectedKey = Qt::Key_D; break;
        case Qt::Key_E:     itsSelectedKey = Qt::Key_E; break;
        case Qt::Key_F:     itsSelectedKey = Qt::Key_F; break;
        case Qt::Key_G:     itsSelectedKey = Qt::Key_G; break;
        case Qt::Key_H:     itsSelectedKey = Qt::Key_H; break;
        case Qt::Key_I:     itsSelectedKey = Qt::Key_I; break;
        case Qt::Key_J:     itsSelectedKey = Qt::Key_J; break;
        case Qt::Key_K:     itsSelectedKey = Qt::Key_K; break;
        case Qt::Key_L:     itsSelectedKey = Qt::Key_L; break;
        case Qt::Key_M:     itsSelectedKey = Qt::Key_M; break;
        case Qt::Key_N:     itsSelectedKey = Qt::Key_N; break;
        case Qt::Key_O:     itsSelectedKey = Qt::Key_O; break;
        case Qt::Key_Q:     itsSelectedKey = Qt::Key_Q; break;
        case Qt::Key_R:     itsSelectedKey = Qt::Key_R; break;
        case Qt::Key_S:     itsSelectedKey = Qt::Key_S; break;
        case Qt::Key_T:     itsSelectedKey = Qt::Key_T; break;
        case Qt::Key_U:     itsSelectedKey = Qt::Key_U; break;
        case Qt::Key_V:     itsSelectedKey = Qt::Key_V; break;
        case Qt::Key_W:     itsSelectedKey = Qt::Key_W; break;
        case Qt::Key_X:     itsSelectedKey = Qt::Key_X; break;
        case Qt::Key_Y:     itsSelectedKey = Qt::Key_Y; break;
        case Qt::Key_Z:     itsSelectedKey = Qt::Key_Z; break;
        case Qt::Key_Space: itsSelectedKey = Qt::Key_Space; break;
        case Qt::Key_0:     itsSelectedKey = Qt::Key_0; break;
        case Qt::Key_1:     itsSelectedKey = Qt::Key_1; break;
        case Qt::Key_2:     itsSelectedKey = Qt::Key_2; break;
        case Qt::Key_3:     itsSelectedKey = Qt::Key_3; break;
        case Qt::Key_4:     itsSelectedKey = Qt::Key_4; break;
        case Qt::Key_5:     itsSelectedKey = Qt::Key_5; break;
        case Qt::Key_6:     itsSelectedKey = Qt::Key_6; break;
        case Qt::Key_7:     itsSelectedKey = Qt::Key_7; break;
        case Qt::Key_8:     itsSelectedKey = Qt::Key_8; break;
        case Qt::Key_9:     itsSelectedKey = Qt::Key_9; break;
        case Qt::Key_Right: itsSelectedKey = Qt::Key_Right; break;
        case Qt::Key_Left:  itsSelectedKey = Qt::Key_Left; break;
        case Qt::Key_Up:    itsSelectedKey = Qt::Key_Up; break;
        case Qt::Key_Down:  itsSelectedKey = Qt::Key_Down; break;
        case Qt::Key_Alt:   itsSelectedKey = Qt::Key_Alt; break;
        case Qt::Key_AltGr: itsSelectedKey = Qt::Key_AltGr; break;
        case Qt::Key_Shift: itsSelectedKey = Qt::Key_Shift; break;
        case Qt::Key_Tab:   itsSelectedKey = Qt::Key_Tab; break;
        }

        bool keyIsAlreadyUse = false;
        for(QGraphicsTextItem *item : itsControlsTextForIICollection){
            if(item->toPlainText() == itsKeyManager->keyToQString(itsSelectedKey))
                keyIsAlreadyUse = true;
        }


        if(linkedControlTextForII != nullptr && !keyIsAlreadyUse){
            // Update key text with new key
            linkedControlTextForII->setPlainText(itsKeyManager->keyToQString(itsSelectedKey));

            // Change II size accordingly with key size
            linkedControlII->setPixmap(linkedControlII->pixmap().scaled(QSize(itsKeyManager->keyToQString(itsSelectedKey).size()*60,50)));


            // Change control key text position (responsive)
            linkedControlTextForII->setPos(linkedControlII->pos()
                                           + QPointF(linkedControlII->boundingRect().width()/2,
                                                     (linkedControlII->boundingRect().height()/2))
                                           - QPointF(linkedControlTextForII->boundingRect().width()/2,
                                                     (linkedControlTextForII->boundingRect().height()/2)));

            // Change control indication text position (responsive)
            linkedControlIndicationText->setPos(linkedControlII->pos()
                                                + QPointF(linkedControlII->boundingRect().width()/2,
                                                          (linkedControlII->boundingRect().height()/2))
                                                - QPointF(0,(linkedControlIndicationText->boundingRect().height()/2))
                                                + QPointF(linkedControlTextForII->boundingRect().width(),0));
        }
    }
}


/* Setters */
void MenuScene::setGameIsStarted(bool newGameIsStarted) { gameIsStarted = newGameIsStarted; }


/* Getters */
BackgroundState MenuScene::getItsSelectedMap() const { return itsSelectedMap; }
