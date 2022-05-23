#ifndef GOMODEL_H
#define GOMODEL_H

#include <QObject>
#include <QVector>
//#include <QInputDialog>

#include "player.h"
#include "godataaccess.h"

#include <exception>
#include <iostream>
#include <queue>
#include <set>

class GoModel : public QObject
{
    Q_OBJECT
public:
    explicit GoModel(GoDataAccess* d);

    Player currentPlayer() const { return (stepNumber_ % 2 == 0) ? Player::P1 : Player::P2; }
    Player getField(const int x, const int y) const {return gameTable_[index(x,y)]; }
    int sideSize() const;

    void setTurns(const int t){ noOfMaxTurns_ = t; }
    bool isGameOver() const { return stepNumber_ >= noOfMaxTurns_ * 2; }

    void newGame(const int n);
    void stepGame(const int x, const int y);

    void loadGame(const QString& path);
    void saveGame(const QString& path);

signals:
    void gameOver(Player player);
    void fieldChanged(const int score1, const int score2, const int turnsLeft);
    void fileIssue();

private:
    int index(const int x, const int y) const;
    int index(const std::pair<int,int>& p) const;
    bool valid(int x, int y) const;
    bool valid(std::pair<int,int>& p) const;

    void removeTrappedSoilders(std::pair<int,int> &p);
    void checkGame(int x,int y);

    int noOfMaxTurns_;
    int stepNumber_;
    int scorePlayer1_;
    int scorePlayer2_;
    QVector<Player> gameTable_;



    GoDataAccess& dataAccess_; // adatelérés
};

#endif // GOMODEL_H
