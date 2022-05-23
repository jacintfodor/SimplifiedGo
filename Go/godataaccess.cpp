#include "godataaccess.h"
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QDir>


bool GoDataAccess::loadGame(const QString& path, QVector<int> &saveGameData)
{
    QFile file;
    file.setFileName(path);
    if (!file.open(QFile::ReadOnly))
        return false;

    QTextStream stream(&file);

    QString currentString = stream.readLine();
    while (!currentString.isNull())
    {
        int currentInt = currentString.toInt();
        saveGameData.push_back(currentInt);

        currentString = stream.readLine();
    }

    file.close();

    return true;
}
bool GoDataAccess::saveGame(const QString& path, const QVector<int> &saveGameData)
{
    QFile file;
    file.setFileName(path);

    if (!file.open(QFile::WriteOnly))
        return false;

    QTextStream stream(&file);

    for (int i = 0; i < saveGameData.size(); i++)
        stream << saveGameData[i] << endl;

    file.close();

    return true;
}
