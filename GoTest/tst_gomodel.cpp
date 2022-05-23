#include <QtTest>
#include "godataaccessmock.h"
#include "godataaccess.h"
#include "gomodel.h"
#include "player.h"
// add necessary includes here

class GoModelTest : public QObject
{
    Q_OBJECT

public:
    GoModelTest();
    ~GoModelTest();
private:
    GoModel* _model;
    GoDataAccess *_dataAccess;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testNewGame();
    void testStepGame();

    void testLoadGame();
    void testSaveGame();
};

GoModelTest::GoModelTest()
{

}

GoModelTest::~GoModelTest()
{

}

void GoModelTest::initTestCase()
{
    _dataAccess = new GoDataAccessMock();
    _model = new GoModel(_dataAccess);
}

// tesztkörnyezet megsemmisítése
void GoModelTest::cleanupTestCase()
{
    delete _dataAccess;
    delete _model;
}

void GoModelTest::testNewGame()
{
    _model->newGame(9);

    QCOMPARE(_model->isGameOver(), false);
    QCOMPARE(_model->sideSize(), 9);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            QCOMPARE(_model->getField(i, j), Player::noP);
}
void GoModelTest::testStepGame()
{
    _model->newGame(9);
    _model->stepGame(0, 0);

    QCOMPARE(_model->currentPlayer(), Player::P2);
    QCOMPARE(_model->getField(0, 0), Player::P1);

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            QVERIFY((i == 0 && j == 0) || (_model->getField(i, j) == Player::noP));

    _model->stepGame(0, 1);
    QCOMPARE(_model->currentPlayer(), Player::P1);
    QCOMPARE(_model->getField(0, 1), Player::P2);

    _model->stepGame(0, 2);
    QCOMPARE(_model->currentPlayer(), Player::P2);
    QCOMPARE(_model->getField(0, 2), Player::P1);
}

void GoModelTest::testLoadGame()
{
    _model->newGame(6);

    _model->stepGame(1, 0);
    _model->stepGame(0, 1);
    _model->stepGame(1, 1);
    _model->stepGame(2, 1);

    _model->loadGame("path.sav");

    // ellenőrizzük, hogy a lépésszám 0, még senki sem lépett, és az X jön
    QCOMPARE(_model->isGameOver(), false);
    QCOMPARE(_model->sideSize(), 5);
    QCOMPARE(_model->currentPlayer(), Player::P1);

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            QVERIFY((_model->getField(i, j) == Player::noP));
}
void GoModelTest::testSaveGame()
{
    _model->newGame(6);

        _model->stepGame(1, 0);
        _model->stepGame(0, 1);
        _model->stepGame(1, 1);
        _model->stepGame(2, 1);

        _model->saveGame("path.sav");
}


QTEST_APPLESS_MAIN(GoModelTest)

#include "tst_gomodel.moc"
