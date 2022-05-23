#ifndef GODATAACCESSMOCK_H
#define GODATAACCESSMOCK_H

#include <QtDebug>
#include "godataaccess.h"

class GoDataAccessMock : public GoDataAccess // mock object, csak teszteléshez
{
public:
    bool isAvailable() const { return true; } // rendelkezésre állás lekérdezése

    bool loadGame(const QString& path, QVector<int> &saveGameData) override // játék betöltése
    {
        saveGameData.fill(0, 25+4);
        saveGameData[0] = 20;

        qDebug() << "game loaded to (" << path << ") with values: ";
        for (int i = 0; i < saveGameData.size(); i++)
            qDebug() << saveGameData[i] << " ";
        qDebug() << endl;

        return true;
    }

    bool saveGame(const QString& path, const QVector<int> &saveGameData) override // játék mentése
    {
        qDebug() << "game saved to (" << path << ") with values: ";
        for (int i = 0; i < saveGameData.size(); i++)
            qDebug() << saveGameData[i] << " ";
        qDebug() << endl;

        return true;
    }
};

#endif // GODATAACCESSMOCK_H
