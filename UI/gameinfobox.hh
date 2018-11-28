#ifndef GAMEINFOBOX_HH
#define GAMEINFOBOX_HH

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

class GameInfoBox : public QGroupBox
{
public:
    GameInfoBox(QString gamePhase);

    void setGamePhaseDisplay(QString phase);

private:
    QLabel* _gamePhaseLabel;
    QGridLayout* _layout;
};

#endif // GAMEINFOBOX_HH
