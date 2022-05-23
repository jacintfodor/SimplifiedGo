#include "gomodel.h"
#include "godataaccess.h"

GoModel::GoModel(GoDataAccess* d) :  noOfMaxTurns_(18), dataAccess_(*d)
{

}

void GoModel::newGame(const int n)
{
    if (n <= 3)
    {
        throw new std::invalid_argument("Game side size less than 4");
    }
    stepNumber_ = 0;
    scorePlayer1_ = 0;
    scorePlayer2_ = 0;
    gameTable_.clear();
    gameTable_.fill(Player::noP,n*n);

    fieldChanged(scorePlayer1_, scorePlayer2_, noOfMaxTurns_- stepNumber_/2);

}

void GoModel::stepGame(const int x, const int y)
{
    if(isGameOver()) return;
    if (gameTable_[index(x,y)] != Player::noP) return;

    gameTable_[index(x,y)] = currentPlayer();
    ++stepNumber_;

    checkGame(x,y);
    fieldChanged(scorePlayer1_, scorePlayer2_, noOfMaxTurns_- stepNumber_/2);
    if(isGameOver())
    {
        if (scorePlayer1_ == scorePlayer2_)
        {
            gameOver(Player::noP);
        }
        else if (scorePlayer1_ > scorePlayer2_)
            gameOver(Player::P1);
        else
        {
            gameOver(Player::P2);
        }
    }
}

void GoModel::loadGame(const QString &path)
{
    QVector<int> saveGameData{};

    if (!dataAccess_.loadGame(path, saveGameData))
    {
        fileIssue();
        return;
    }

    gameTable_.clear();
    noOfMaxTurns_ = saveGameData[0];
    stepNumber_ = saveGameData[1];
    scorePlayer1_ = saveGameData[2];
    scorePlayer2_ = saveGameData[3];
    for (int i = 4; i < saveGameData.size(); ++i)
    {
        Player p = static_cast<Player>(saveGameData[i]);
        gameTable_.push_back(p);
    }

    fieldChanged(scorePlayer1_, scorePlayer2_, noOfMaxTurns_- stepNumber_/2);
}

void GoModel::saveGame(const QString &path)
{
    if (isGameOver())
        return;

    QVector<int> saveGameData{};

    saveGameData.push_back(noOfMaxTurns_);
    saveGameData.push_back(stepNumber_);
    saveGameData.push_back(scorePlayer1_);
    saveGameData.push_back(scorePlayer2_);
    for (int i = 0; i < gameTable_.size(); ++i)
    {
        int raw = static_cast<int>(gameTable_[i]);
        saveGameData.push_back(raw);
    }

    bool l = dataAccess_.saveGame(path, saveGameData);
    if (l)
        fileIssue();
}

int GoModel::index(const int x, const int y) const
{
    if (!valid(x,y))
        throw std::invalid_argument("wrong indexing");

    return x + y * sideSize();
}

int GoModel::index(const std::pair<int, int> &p) const
{
    return index(p.first, p.second);
}

bool GoModel::valid(int x, int y) const
{
    return x >= 0 && y >= 0 && x < sideSize() && y < sideSize();
}

void GoModel::removeTrappedSoilders(std::pair<int, int> &p)
{
    std::queue<std::pair<int,int>> vertex_queue;
    std::set<int> visited_vertices;
    vertex_queue.push(p);
    bool trapped = true;

    Player soilder = gameTable_[index(p)];
    if (soilder == Player::noP)
        return;

    while(!vertex_queue.empty() && trapped) {
        std::pair<int,int> current_vertex = vertex_queue.front();
        vertex_queue.pop();

        visited_vertices.insert(index(current_vertex));

        std::pair<int, int> leftVertex = std::make_pair(current_vertex.first-1, current_vertex.second);
        std::pair<int, int> rightVertex = std::make_pair(current_vertex.first+1, current_vertex.second);
        std::pair<int, int> topVertex = std::make_pair(current_vertex.first, current_vertex.second-1);
        std::pair<int, int> bottomVertex = std::make_pair(current_vertex.first, current_vertex.second+1);

        //left
        if (valid(leftVertex))
        {
            if (gameTable_[index(leftVertex)] == Player::noP)
                trapped = false;

            else if (gameTable_[index(leftVertex)] == soilder && !(visited_vertices.find(index(leftVertex)) != visited_vertices.end()))
            {
                vertex_queue.push(leftVertex);
            }
        }

        //right
        if (valid(rightVertex))
        {
            if (gameTable_[index(rightVertex)] == Player::noP)
                trapped = false;

            else if (gameTable_[index(rightVertex)] == soilder && !(visited_vertices.find(index(rightVertex)) != visited_vertices.end()))
            {
                vertex_queue.push(rightVertex);
            }
        }

        //top
        if (valid(topVertex))
        {
            if (gameTable_[index(topVertex)] == Player::noP)
                trapped = false;

            else if (gameTable_[index(topVertex)] == soilder && !(visited_vertices.find(index(topVertex)) != visited_vertices.end()))
            {
                vertex_queue.push(topVertex);
            }
        }

        //bottom
        if (valid(bottomVertex))
        {
            if (gameTable_[index(bottomVertex)] == Player::noP)
                trapped = false;

            else if (gameTable_[index(bottomVertex)] == soilder && !(visited_vertices.find(index(bottomVertex)) != visited_vertices.end()))
            {
                vertex_queue.push(bottomVertex);
            }
        }
    }
    if (trapped)
    {
        scorePlayer1_ += (soilder == Player::P2) ? visited_vertices.size() : 0;
        scorePlayer2_ += (soilder == Player::P1) ? visited_vertices.size() : 0;
        for (auto it = visited_vertices.begin(); it != visited_vertices.end(); ++it)
            gameTable_[*it] = Player::noP;
    }

}

void GoModel::checkGame(int x, int y)
{
    std::pair<int,int> current_vertex = std::make_pair(x,y);
    std::pair<int, int> leftVertex = std::make_pair(current_vertex.first-1, current_vertex.second);
    std::pair<int, int> rightVertex = std::make_pair(current_vertex.first+1, current_vertex.second);
    std::pair<int, int> topVertex = std::make_pair(current_vertex.first, current_vertex.second-1);
    std::pair<int, int> bottomVertex = std::make_pair(current_vertex.first, current_vertex.second+1);

    if(valid(leftVertex) && gameTable_[index(leftVertex)] != gameTable_[index(current_vertex)])
        removeTrappedSoilders(leftVertex);
    if(valid(rightVertex) && gameTable_[index(rightVertex)] != gameTable_[index(current_vertex)])
        removeTrappedSoilders(rightVertex);
    if(valid(topVertex) && gameTable_[index(topVertex)] != gameTable_[index(current_vertex)])
        removeTrappedSoilders(topVertex);
    if(valid(bottomVertex) && gameTable_[index(bottomVertex)] != gameTable_[index(current_vertex)])
        removeTrappedSoilders(bottomVertex);

    removeTrappedSoilders(current_vertex);
    //fieldChanged();

}

bool GoModel::valid(std::pair<int, int>& p) const
{
    return valid(p.first, p.second);
}

int GoModel::sideSize() const
{
    double squareRootOfgameTable = sqrt(gameTable_.size());

    double intpart;
    double fract = modf(squareRootOfgameTable, &intpart);
    //        std::cerr << fract;
    if (fract != 0.0)
    {
        throw std::runtime_error("ill sized gameTable");
    }

    return static_cast<int>(intpart);
}
