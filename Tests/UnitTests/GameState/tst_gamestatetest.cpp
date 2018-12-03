#include <QtTest>

#include "gamestate.hh"
#include "constants.hh"
#include "helpers.hh"

#include <random>
#include <memory>

// add necessary includes here

using PhaseIter =
const std::map<Common::GamePhase, Common::GamePhase>::iterator;

class GameStateTest : public QObject
{
    Q_OBJECT

public:
    GameStateTest();
    virtual ~GameStateTest() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Test changePlayerTurn
    void testNextPlayerValid();
    void testNextPlayerInvalid();

    // Test changeGamePhase
    void testNextPhaseValid();
    void testNextPhaseInvalid();

private:
    std::shared_ptr<Student::GameState> _gameState;
    const int _minPlayers;
    const int _maxPlayers;
    const std::map <Common::GamePhase, Common::GamePhase> _nextPhaseMap;
};

GameStateTest::GameStateTest() :
    _gameState(nullptr),
    _minPlayers(GameConstants::MIN_PLAYERS),
    _maxPlayers(GameConstants::MAX_PLAYERS),
    _nextPhaseMap(GameConstants::NEXT_GAME_PHASE)
{}

void GameStateTest::initTestCase()
{
    _gameState = std::make_shared<Student::GameState>(_maxPlayers);
    QVERIFY(_gameState->currentGamePhase() == Common::GamePhase::MOVEMENT);
}

void GameStateTest::cleanupTestCase()
{
    _gameState.reset();
}

void GameStateTest::testNextPlayerValid()
{
    int randomValid = Helpers::randomNumber(_minPlayers - 1, _maxPlayers);
    _gameState->changePlayerTurn(randomValid);

    QVERIFY(randomValid == _gameState->currentPlayer());
}

void GameStateTest::testNextPlayerInvalid()
{
    int randomInvalid = Helpers::randomNumber(_maxPlayers + 1, 100);
    _gameState->changePlayerTurn(randomInvalid);

    QVERIFY(randomInvalid != _gameState->currentPlayer());
}

void GameStateTest::testNextPhaseValid()
{
    Common::GamePhase currentPhase = _gameState->currentGamePhase();
    Common::GamePhase nextPhase = _nextPhaseMap.at(currentPhase);
    _gameState->changeGamePhase(nextPhase);
    QVERIFY(_gameState->currentGamePhase() == nextPhase);
}


void GameStateTest::testNextPhaseInvalid()
{
    Common::GamePhase currentPhase = _gameState->currentGamePhase();
    Common::GamePhase invalidNextPhase = _nextPhaseMap.at(
                _nextPhaseMap.at(currentPhase));
    _gameState->changeGamePhase(invalidNextPhase);
    QVERIFY(_gameState->currentGamePhase() != invalidNextPhase);
}



QTEST_APPLESS_MAIN(GameStateTest)

#include "tst_gamestatetest.moc"
