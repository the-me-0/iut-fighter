#include <QtTest>

// add necessary includes here
#include "../The_IUT_FIGHTER/properties.h"
#include "../The_IUT_FIGHTER/fighter.h"
#include "../The_IUT_FIGHTER/anthonyfighter.h"
#include "../The_IUT_FIGHTER/arthurfighter.h"
#include "../The_IUT_FIGHTER/background.h"
#include "../The_IUT_FIGHTER/cyrilfighter.h"
#include "../The_IUT_FIGHTER/fightscene.h"
#include "../The_IUT_FIGHTER/interactiveimage.h"
#include "../The_IUT_FIGHTER/key.h"
#include "../The_IUT_FIGHTER/maxfighter.h"
#include "../The_IUT_FIGHTER/menuscene.h"
#include "../The_IUT_FIGHTER/nilsfighter.h"
#include "../The_IUT_FIGHTER/noahfighter.h"
#include "../The_IUT_FIGHTER/widget.h"
#include "../The_IUT_FIGHTER/rock.h"

class test : public QObject
{
    Q_OBJECT

public:
    test();
    ~test();
    Fighter * firstFighter;
    Fighter * secondFighter;

    QProgressBar *ffLBar;
    QProgressBar *sfLBar;

    std::vector<QProgressBar *> itsProgressBar;

    Key *keyManager;

    QGraphicsView *itsView;

    FightScene *itsFightScene;


private slots:
    // call before each test
    void init();
    void cleanup();
    // call before the first test fonction
    void initTestCase();
    void cleanupTestCase();

    //tests
    void testFighterLostLife();
    void testFighterdoNotLoseMoreLifeThanExpected();
    void testInstantDeath();
    void testPunchAttack();
    void testKickAttack();
    void testSpecialAttackDouble();
    void testAddPower();
    void cannotAttackWhileAttack();
    void testEndOfRoundUpdatePoint();
};

test::test()
{

}

test::~test()
{

}

void test::init()
{
    firstFighter = new NoahFighter();
    secondFighter = new CyrilFighter();
    keyManager = new Key();

    ffLBar = new QProgressBar();
    sfLBar = new QProgressBar();

    itsProgressBar = {new QProgressBar(),new QProgressBar(),new QProgressBar(),new QProgressBar(),new QProgressBar(),new QProgressBar()};

    itsView = new QGraphicsView();

    itsFightScene = new FightScene(itsView,ffLBar,sfLBar,keyManager,itsProgressBar,this);
    itsView->setScene(itsFightScene);

    // itsFightScene->init..... = debut de la boucle de jeu
}

void test::cleanup()
{
    delete firstFighter;
    delete secondFighter;
    delete keyManager;
    delete ffLBar;
    delete sfLBar;
    delete itsFightScene;
    delete itsView;
}

void test::initTestCase()
{

}

void test::cleanupTestCase()
{

}

void test::testFighterLostLife()
{
    firstFighter->tank(20);
    QCOMPARE(firstFighter->getItsHP(),100);
}

void test::testFighterdoNotLoseMoreLifeThanExpected()
{
    firstFighter->tank(500);
    QCOMPARE(firstFighter->getItsHP(),0);
}

void test::testInstantDeath()
{
    itsFightScene->initializeFightScene(firstFighter,secondFighter,MAPMAX);
    itsFightScene->setItsActualEvent(Event::INSTANTDEATH);
    QCOMPARE(1,firstFighter->getItsHP());
    QCOMPARE(1,secondFighter->getItsHP());
}

void test::testPunchAttack()
{
    secondFighter->tank(firstFighter->getItsPunchAttackCP());
    QCOMPARE(103,secondFighter->getItsHP());
}

void test::testKickAttack()
{
    secondFighter->tank(firstFighter->getItsKickAttackCP());
    QCOMPARE(99,secondFighter->getItsHP());
}

void test::testSpecialAttackDouble()
{
    firstFighter->addPower(50);
    firstFighter->specialAttack();
    secondFighter->tank(firstFighter->getItsKickAttackCP());
    QCOMPARE(102,secondFighter->getItsHP());
}

void test::testAddPower()
{
    firstFighter->addPower(98752);
    QCOMPARE(60,firstFighter->getItsPower());
}

void test::cannotAttackWhileAttack()
{

    firstFighter->punchAttack();
    QCOMPARE(false,firstFighter->getCanPunchAttack());
}

void test::testEndOfRoundUpdatePoint()
{
    itsFightScene->initializeFightScene(firstFighter,secondFighter,MAPMAX);
    firstFighter->tank(1110);
    QCOMPARE(1,secondFighter->getItsRoundWon());
}


QTEST_MAIN(test)

#include "tst_test.moc"
