#include "gameinfobox.hh"

GameInfoBox::GameInfoBox(QString gamePhase)
{
    _gamePhaseLabel = new QLabel(gamePhase);
    _layout = new QGridLayout();
    _layout->addWidget(_gamePhaseLabel);
    this->setLayout(_layout);
}

void GameInfoBox::setGamePhaseDisplay(QString phase){
    _gamePhaseLabel->setText(phase);
}
