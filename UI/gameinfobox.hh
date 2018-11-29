#ifndef GAMEINFOBOX_HH
#define GAMEINFOBOX_HH

#include "gamestate.hh"

#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
namespace Student {

class GameInfoBox : public QGroupBox
{
    Q_OBJECT
public:
    GameInfoBox(std::shared_ptr<GameState> gameState);

    void updateGameState();
    void updateActor(QPixmap image, std::string moves);

signals:
   void spinButtonPressed();

public slots:
   void emitSpinButtonPressed();

private:
    QLabel* _gamePhaseLabel;
    QLabel* _playerTurnLabel;
    QPushButton* _spinButton;
    QLabel* _actorImageLabel;
    QLabel* _actorMovesLabel;

    QGridLayout* _layout;
    std::shared_ptr<GameState> _gameState;
};

}
#endif // GAMEINFOBOX_HH
