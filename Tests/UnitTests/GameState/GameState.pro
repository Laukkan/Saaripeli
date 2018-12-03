QT += testlib gui


TARGET = tst_gamestatetest
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

DESTDIR = bin

TEMPLATE = app

SOURCES +=  tst_gamestatetest.cpp \
    ../../../UI/gamestate.cpp \
    ../../../UI/helpers.cpp \

HEADERS += ../../../UI/gamestate.hh \
    ../../../UI/helpers.hh \
    ../../../UI/constants.hh \
    ../../../GameLogic/Engine/igamestate.hh \


INCLUDEPATH += ../../../UI \
    ../../../GameLogic/Engine/

DEPENDPATH  += ../../../UI \
    ../../../GameLogic/Engine/
