#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    /* Do not show combat life bars and power bars before fight */
    ui->firstFighterLifeBar->setVisible(false);
    ui->secondFighterLifeBar->setVisible(false);
    ui->firstFighterPowerBar1->setVisible(false);
    ui->firstFighterPowerBar2->setVisible(false);
    ui->firstFighterPowerBar3->setVisible(false);
    ui->secondFighterPowerBar1->setVisible(false);
    ui->secondFighterPowerBar2->setVisible(false);
    ui->secondFighterPowerBar3->setVisible(false);

    setWindowTitle("IUT Fighter V.1");

    itsKeyManager = new Key();
    
    itsMenuScene = new MenuScene(itsKeyManager, this);

    ui->graphicsView->setScene(itsMenuScene);

    connect(itsMenuScene, &MenuScene::startArcadeFight, this, &Widget::createArcadeFight);
    connect(itsMenuScene, &MenuScene::startCampaignFight, this, &Widget::createCampaignFight);
    connect(itsMenuScene, &MenuScene::askForReplay, this, &Widget::replayDelegate);


}


/* keyPressEvents handler (escape, ...) */
void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        qApp->exit();

    QWidget::keyPressEvent(event);
}


/* Create an arcade mode game (multiplayer),
 * initialize a new FightScene, only one fight between the two players.
 */
void Widget::createArcadeFight(Fighter *fFighter, Fighter *sFighter)
{
    if(!isCurrentlyCreatingFight){ // Security if the emit is send multiple times
        isCurrentlyCreatingFight = true;


        if(itsFightScene != nullptr){
            delete itsFightScene;
            itsFightScene = nullptr;
        }

        std::vector<QProgressBar *> fightersPowerBars = {ui->firstFighterPowerBar1, ui->firstFighterPowerBar2, ui->firstFighterPowerBar3,
                                                        ui->secondFighterPowerBar1, ui->secondFighterPowerBar2, ui->secondFighterPowerBar3};

        itsFightScene = new FightScene(ui->graphicsView,
                                       ui->firstFighterLifeBar,
                                       ui->secondFighterLifeBar,
                                       itsKeyManager,
                                       fightersPowerBars,
                                       this);

        connect (itsFightScene, &FightScene::returnBackToMenu, this, &Widget::showBackMenu);

        itsFightScene->initializeFightScene(fFighter, sFighter, itsMenuScene->getItsSelectedMap());
        ui->graphicsView->setScene(itsFightScene);

        // Security desactivation after an amount of sec
        QTimer::singleShot(1000, this, [=](){isCurrentlyCreatingFight = false;});
    }
}


/* Create a campaign mode game (singleplayer),
 * initialize a new FightScene, 6 fights (the player versus each fighters).
 */
void Widget::createCampaignFight(Fighter *fFighter)
{
    Q_UNUSED(fFighter);
    return;
}


/* Show back menu after a fight */
void Widget::showBackMenu()
{
    ui->graphicsView->setScene(itsMenuScene);

    // At the end of the fight we show the end game menu to the players
    itsMenuScene->setItsState(CHOICEENDGAME);
    itsMenuScene->setGameIsStarted(false);
}


/* Delegate (mediator) between the fightscene and the menuscene, in order to make replay possible */
void Widget::replayDelegate()
{
    ui->graphicsView->setScene(itsFightScene);
    itsFightScene->resetFightForReplayAndStartIt();
}

Widget::~Widget() {
    delete ui;
    delete itsKeyManager;
}

