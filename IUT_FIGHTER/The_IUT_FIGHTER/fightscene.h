#ifndef FIGHTSCENE_H
#define FIGHTSCENE_H

#include "properties.h"
#include "background.h"
#include "fighter.h"
#include "key.h"
#include "rock.h"


class FightScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit FightScene(QGraphicsView *view, QProgressBar *firstPBar, QProgressBar *secondPBar, Key *keyManager, std::vector<QProgressBar *> fightersPowerBars, QObject *parent = nullptr);

    void moveFighter(Fighter * aFirstFighter,
                     Fighter * aSecondFighter);

    void fighterMovementManager(Fighter * aFirstFighter,
                                Fighter * aSecondFighter);

    void initializeFightScene(Fighter * fFighter,
                              Fighter * sFighter,
                              BackgroundState mapBg);


    void resetFightForReplayAndStartIt();
    QString strEvent(Event anEvent);

    Event getItsActualEvent() const;
    void setItsActualEvent(Event newItsActualEvent);

private:
    int itsSceneWidth = 3702;
    int itsSceneHeight = 1080;
    int itsCameraSceneWidth = 1920;

    QGraphicsView *itsView;

    QGraphicsPixmapItem * itsEventImg = nullptr;

    Background * itsBackground = new Background();

    Event itsActualEvent = Event::NONE;

    /* keys manager */
    Key * keys;

    Fighter * itsFirstFighter = nullptr;
    Fighter * itsSecondFighter = nullptr;

    QTimer * itsUpdateTimer = new QTimer(this);
    QTimer * itsEndRoundTimer = new QTimer(this);

    /* LifeB -> Life Bar (HP representation) */
    QProgressBar *itsFirstFighterLifeB;
    QProgressBar *itsSecondFighterLifeB;
    void updateLifeBars();

    /* Power bars */
    QProgressBar *itsFirstFighterPowerBar1;
    QProgressBar *itsFirstFighterPowerBar2;
    QProgressBar *itsFirstFighterPowerBar3;
    QProgressBar *itsSecondFighterPowerBar1;
    QProgressBar *itsSecondFighterPowerBar2;
    QProgressBar *itsSecondFighterPowerBar3;
    int firstFighterActualPowerProgress = 0;
    int secondFighterActualPowerProgress = 0;
    void updatePowerBars();

    /*Display name's of figther above life bar*/
    QGraphicsTextItem * itsFirstFighterNameText = new QGraphicsTextItem();;
    QGraphicsTextItem * itsSecondFighterNameText = new QGraphicsTextItem();;
    void displayFightersNames();

    /* Pause the fight */
    QGraphicsTextItem * itsBreakText = new QGraphicsTextItem();
    void gameBreak();
    void gameResume();
    bool fightIsInPause=false;

    /* Display if the fighter have win round */
    QGraphicsPixmapItem * itsRoundWonPixMapForFirstFighter ;
    QGraphicsPixmapItem * itsRoundWonPixMapForSecondFighter ;
    void displayRoundWon();

    /*Display a timer in the fightscene*/
    int seconde = 0 ;
    QTimer * itsFightTimer = new QTimer(this);
    QGraphicsTextItem * itsTimerTextForFightScene = new QGraphicsTextItem();

    /* Hit text */
    QGraphicsTextItem * itsFirstFighterHitText = nullptr;
    QGraphicsTextItem * itsSecondFighterHitText = nullptr;

    /* End of fight / round */
    QGraphicsTextItem * itsEndRoundText = nullptr;
    QGraphicsTextItem * itsRoundWinnerText = nullptr;
    QGraphicsTextItem * itsEndGameText = new QGraphicsTextItem;
    QGraphicsTextItem * itsFightWinnerText = new QGraphicsTextItem;
    QFont btnFont = QFont("Upheaval TT (BRK)", 40, QFont::Bold);
    void endOfRound();
    bool endOfRoundIsBeingTested = false;
    bool endOfRoundIsRunning = false;

    void displayEndOfRound();
    void displayWinner(Fighter* aFighter);

    /* special attack handler -verify and apply the effects of a special attack- */
    void specialAttackHandler();
    int firstFighterSARemainingTime = 0;
    int secondFighterSARemainingTime = 0;
    bool firstFighterIsSA = false;
    bool secondFighterIsSA = false;
    QGraphicsPixmapItem * itsFirstFighterSpecialAttackImg = nullptr;
    QGraphicsPixmapItem * itsSecondFighterSpecialAttackImg = nullptr;

    /* event manager main method */
    int remainingTimeBeforeEvent = 0;
    void updateEventManager();
    Event pickRandomEvent();

    bool isEventSingleShotRunning = false;
    int itsRemainingEventTime = 0;
    int itsFormerHPFirstFighter;
    int itsFormerHPSecondFighter;

    std::vector<Rock *> itsRocks;
    std::vector<QGraphicsPixmapItem *> itsMarks;
    std::vector<QGraphicsPixmapItem *> toBeDeletedMarks;
    void resetEvent();
    void createRockFall();


    void resetFightForNewRound();

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void fightLoop();
    void displayTimerOnFightScene();

signals:
    void returnBackToMenu();
};

#endif // FIGHTSCENE_H

