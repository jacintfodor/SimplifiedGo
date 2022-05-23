#ifndef GODATAACCESS_H
#define GODATAACCESS_H

#include <QString>
#include <QVector>
#include <QFile>


class GoDataAccess
{
public:
    explicit GoDataAccess() {}

    bool loadGame(const QString& path, QVector<int> &saveGameData);
    bool saveGame(const QString& path, const QVector<int> &saveGameData);
};

#endif // GODATAACCESS_H
