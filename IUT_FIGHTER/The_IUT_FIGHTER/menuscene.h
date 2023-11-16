#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "background.h"
#include "interactiveimage.h"
#include "fighter.h"
#include "key.h"

#include "maxfighter.h"
#include "noahfighter.h"
#include "cyrilfighter.h"
#include "anthonyfighter.h"
#include "nilsfighter.h"
#include "arthurfighter.h"

class MenuScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit MenuScene(Key * keyManager, QObject *parent = nullptr);
    ~MenuScene();

    /* Setters */
    // Set the state of the menu and update scene depending of the state
    void setItsState(MenuState newItsState);

    void setGameIsStarted(bool newGameIsStarted);

    /* Getter */
    BackgroundState getItsSelectedMap() const;


private:
    MenuState itsMenuState = STARTMENU;

    void hideGraphics();

    int itsSceneWidth = 1920;
    int itsSceneHeight = 1080;

    Background * itsBackground = new Background();


    void askForNewFight();

    /* Start menu */
    void addStartMenuGraphics();
    QGraphicsPixmapItem * itsStartMenuLogo = nullptr;
    QGraphicsPixmapItem * itsWhiteRect = nullptr;
    QGraphicsTextItem * itsStartInstructionText = nullptr;
    QTimer * itsStartMenuAnimTimer = new QTimer(this);
    double tempOpacity = 0;


    /* Game mode choice Menu : Arcade or Campaign */
    GameMode itsSelectedGameMode = GameMode::NULLV;
    void addGameModeChoiceMenu();
    QTimer * itsChoiceMenuAnimTimer = new QTimer(this);
    InteractiveImage * arcadeModeII = nullptr;
    QGraphicsTextItem * arcadeModeTextForII = nullptr;
    InteractiveImage * campaignModeII = nullptr;
    QGraphicsTextItem * campaignModeTextForII = nullptr;
    InteractiveImage * controlsII = nullptr;
    QGraphicsTextItem * controlsTextForII = nullptr;
    QFont btnFont = QFont("Upheaval TT (BRK)", 30, QFont::Bold);
    QGraphicsPixmapItem * itsStarCursor = nullptr;
    void starMovementsForGameModeSelection(QKeyEvent *event);


    /* Controls visualisation/modification page */
    Key * itsKeyManager;
    void showControls();
    InteractiveImage * itsControlII = nullptr;
    vector<InteractiveImage *> itsControlsIICollection;

    QGraphicsTextItem * itsControlTextForII = nullptr;
    vector<QGraphicsTextItem *> itsControlsTextForIICollection;

    QGraphicsTextItem * itsControlIndicationText = nullptr;
    vector<QGraphicsTextItem *> itsControlsIndicationTextCollection;

    QGraphicsTextItem * linkedControlTextForII = nullptr;
    InteractiveImage * linkedControlII = nullptr;
    QGraphicsTextItem * linkedControlIndicationText = nullptr;

    void controlsSelectionManager(QKeyEvent *event);
    Qt::Key itsSelectedKey;
    bool canBeChanged = false;
    bool controlsAreShown = false;

    InteractiveImage * itsGoBackButtonII = nullptr;
    QGraphicsTextItem * itsGoBackButtonTextForII = nullptr;

    InteractiveImage * itsSaveButtonII = nullptr;
    QGraphicsTextItem * itsSaveButtonTextForII = nullptr;

    QGraphicsTextItem * itsPlayerOneName = nullptr;
    QGraphicsTextItem * itsPlayerTwoName = nullptr;
    QGraphicsLineItem * itsPlayersSeparationLine = nullptr;




    /* End Game Choice Menu */
    void addEndGameChoiceMenu();
    EndChoice itsEndChoice;
    InteractiveImage * replayII = nullptr;
    QGraphicsTextItem * replayTextForII = nullptr;
    InteractiveImage * endGameChoiceMenuII = nullptr;
    QGraphicsTextItem * endGameChoiceMenuTextForII = nullptr;
    void starMovementsForEndGameChoiceSelection(QKeyEvent *event);

    /* Map choice Menu */
    void addMapChoiceMenu();
    QGraphicsPixmapItem * itsMapMugshot = nullptr;
    QGraphicsTextItem * itsChooseMapText = nullptr;
    vector<QGraphicsPixmapItem *> itsMapsMugShotsGPI;
    void selectorMovementsForMapSelection(QKeyEvent *event);
    BackgroundState itsFirstPlayerMapSelected = NULLV;
    BackgroundState itsSecondPlayerMapSelected = NULLV;
    BackgroundState itsSelectedMap = NULLV;

    /* Fighter choice Menu */
    void addFighterChoiceMenu();
    QGraphicsPixmapItem * itsFighterMugShot = nullptr;
    QGraphicsTextItem * itsChooseFighterText = nullptr;
    vector<QGraphicsPixmapItem *> itsFightersMugShotsGPI;
    QGraphicsPixmapItem * itsFirstPlayerSelector = nullptr;
    QGraphicsPixmapItem * itsSecondPlayerSelector = nullptr;
    void selectorMovementsForFighterSelection(QKeyEvent *event);
    Fighter * itsFirstPlayerFighterSelected = nullptr;
    Fighter * itsSecondPlayerFighterSelected = nullptr;

    bool isBeingValidated = false;
    bool gameIsStarted = false;



    // QGraphicsScene interface
protected:
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void startArcadeFight(Fighter * fFighter, Fighter * sFighter);
    void startCampaignFight(Fighter * fFighter);
    void askForReplay();

};

#endif // MENUSCENE_H
