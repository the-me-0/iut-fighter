#ifndef PROPERTIES_H
#define PROPERTIES_H


#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsScene>
#include <QProgressBar>
#include <QKeyEvent>
#include <QMessageBox>
#include <vector>
#include <QRandomGenerator>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QFloat16>
#include <QChar>
#include <QPen>
#include <QFile>
#include <QDebug>
#include <QTextStream>

using std::vector;


enum class GameMode{
    NULLV,
    CAMPAIGN,
    ARCADE
};

enum class RockState{
    FALLING,
    CRASHED
};

enum class Event{
    NONE,
    NOTALLOWED,
    FLASH,
    INSTANTDEATH,
    ROCKFALL,
    FREEZE
};

enum class EndChoice{
    REPLAY,
    MAINMENU
};

enum MenuState{
    STARTMENU,
    CHOICEGAMEMODE,
    CHOICEMAP,
    CHOICEFIGHTER,
    CHOICEENDGAME,
    CHOICECONTROLS,
    REPLAY
};


enum BackgroundState {
    NULLV,
    MENUBACKGROUND,
    MAPVEGA,
    MAPMAX,
    MAPANTHO,
    MAPNOAH,
    MAPCYRIL,
    MAPARTHUR,
    MAPNILS
};


enum Direction{
    FACERIGHT,
    FACELEFT
};


// Comments : max nb of sprites per animation
enum AnimationStates{
    STATIC,             // 5 sprites (Validate) + ALGO DONE
    WALKING,            // 5 sprites (Validate)
    PUNCHSTANDING,      // 3 sprites (Validate) + ALGO DONE
    PUNCHCROUCH,        // 3 sprites (Validate) + ALGO DONE
    HIGHKICK,           // 3 sprites (Validate) + ALGO DONE
    UPPERCUT,           // 3 sprites (Validate)
    SWEEPING,           // 4 sprites (Validate)
    SPECIALATTACK,      // fuck (Validate)
    BLOCKINGCROUCH,     // 1 sprite (Validate)
    BLOCKINGSTANDING,   // 1 sprite (Validate)
    CROUCHHIT,          // 2 sprites (Validate)
    STANDINGHIT,        // 3 sprites (Validate)
    KNOCKOUT,           // NO
    CROUCH,             // 3 sprites (Validate) + ALGO DONE
    JUMP,               // 7 sprites (Validate) + ALGO DONE
    INTRO,              // 4 sprites (Validate)
    WIN,                // 3 sprites (Validate) + ALGO DONE
    LOST,               // 3 sprites (Validate)
    STUN                // 1 sprite (Validate)
};

#endif // PROPERTIES_H
