#ifndef OPENFOLDERHANDLER_H
#define OPENFOLDERHANDLER_H

#include <QObject>
#include <QStringList>

class OpenFolderHandler : public QObject
{
    Q_OBJECT
    QString lastdir;
public:
    explicit OpenFolderHandler(QObject *parent = nullptr);
    ~OpenFolderHandler()=default;
    Q_INVOKABLE QStringList getMountRootPaths();
signals:
    
public slots:
};

#endif // OPENFOLDERHANDLER_H
