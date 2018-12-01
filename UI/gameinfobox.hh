#ifndef GAMEINFOBOX_HH
#define GAMEINFOBOX_HH

#include "gamestate.hh"
#include "igamerunner.hh"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

namespace Student {

class GameInfoBox : public QGroupBox
{
    Q_OBJECT
public:
    GameInfoBox(std::shared_ptr<GameState> gameState,
                std::shared_ptr<Common::IGameRunner> gameRunner);

    void updateGameState();
    void updateActor(QPixmap image, std::string moves);

signals:
   void spinButtonPressed();
   void stayHerePressed();
   void continueFromSpinPressed();

private:
    QLabel* _gamePhaseLabel;
    QLabel* _playerTurnLabel;
    QLabel* _playerMovesLabel;
    QPushButton* _spinButton;
    QPushButton* _stayHereButton;
    QPushButton* _continueFromSpin;
    QLabel* _actorImageLabel;
    QLabel* _actorMovesLabel;

    QGridLayout* _layout;
    std::shared_ptr<GameState> _gameState;
    std::shared_ptr<Common::IGameRunner> _gameRunner;
};

}
#endif // GAMEINFOBOX_HH
