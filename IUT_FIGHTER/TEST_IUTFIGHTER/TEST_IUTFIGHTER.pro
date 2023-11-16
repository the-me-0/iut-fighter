QT += testlib
QT -= gui
QT += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

TEMPLATE = app

SOURCES +=  tst_test.cpp \
    ../The_IUT_FIGHTER/anthonyfighter.cpp \
    ../The_IUT_FIGHTER/arthurfighter.cpp \
    ../The_IUT_FIGHTER/background.cpp \
    ../The_IUT_FIGHTER/cyrilfighter.cpp \
    ../The_IUT_FIGHTER/fighter.cpp \
    ../The_IUT_FIGHTER/fightscene.cpp \
    ../The_IUT_FIGHTER/interactiveimage.cpp \
    ../The_IUT_FIGHTER/key.cpp \
    ../The_IUT_FIGHTER/maxfighter.cpp \
    ../The_IUT_FIGHTER/menuscene.cpp \
    ../The_IUT_FIGHTER/nilsfighter.cpp \
    ../The_IUT_FIGHTER/noahfighter.cpp \
    ../The_IUT_FIGHTER/rock.cpp \
    ../The_IUT_FIGHTER/widget.cpp

HEADERS += \
    ../The_IUT_FIGHTER/anthonyfighter.h \
    ../The_IUT_FIGHTER/arthurfighter.h \
    ../The_IUT_FIGHTER/background.h \
    ../The_IUT_FIGHTER/cyrilfighter.h \
    ../The_IUT_FIGHTER/fighter.h \
    ../The_IUT_FIGHTER/fightscene.h \
    ../The_IUT_FIGHTER/interactiveimage.h \
    ../The_IUT_FIGHTER/key.h \
    ../The_IUT_FIGHTER/maxfighter.h \
    ../The_IUT_FIGHTER/menuscene.h \
    ../The_IUT_FIGHTER/nilsfighter.h \
    ../The_IUT_FIGHTER/noahfighter.h \
    ../The_IUT_FIGHTER/properties.h \
    ../The_IUT_FIGHTER/rock.h \
    ../The_IUT_FIGHTER/widget.h

FORMS += \
    ../The_IUT_FIGHTER/widget.ui
