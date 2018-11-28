#ifndef GAMEINFOBOX_HH
#define GAMEINFOBOX_HH

#include "gamestate.hh"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
namespace Student {

class GameInfoBox : public QGroupBox
{
public:
    GameInfoBox(std::shared_ptr<GameState> gameState);

    void updateGameState();

private:
    QLabel* _gamePhaseLabel;
    QLabel* _playerTurnLabel;
    QGridLayout* _layout;
    std::shared_ptr<GameState> _gameState;
};

}
#endif // GAMEINFOBOX_HH
