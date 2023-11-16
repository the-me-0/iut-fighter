#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "fightscene.h"
#include "properties.h"
#include "menuscene.h"
#include "key.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    MenuScene * itsMenuScene = nullptr;
    FightScene * itsFightScene = nullptr;

    bool isCurrentlyCreatingFight = false;

    Key * itsKeyManager = nullptr;

    void keyPressEvent(QKeyEvent *event);

private slots:
    void createArcadeFight(Fighter * fFighter, Fighter * sFighter);
    void createCampaignFight(Fighter * fFighter);
    void showBackMenu();
    void replayDelegate();
};

#endif // WIDGET_H
