#ifndef IDATAACCESS_H
#define IDATAACCESS_H
#include <QString>
#include <QVector>
#include <QFile>

class IDataAccess
{
public:
    explicit IDataAccess() {}
    virtual ~IDataAccess(){}

    virtual bool loadGame(const QString& path, QVector<int> &saveGameData) =0;
    virtual bool saveGame(const QString& path, const QVector<int> &saveGameData) =0;
};

#endif // IDATAACCESS_H
