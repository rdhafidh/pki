#ifndef OPENFOLDERHANDLER_H
#define OPENFOLDERHANDLER_H

#include <QObject>
#include <QStringList>
#include <mountstoragehandler.h>

class OpenFolderHandler : public QObject
{
    Q_OBJECT 
public:
    explicit OpenFolderHandler(QObject *parent = nullptr);
    ~OpenFolderHandler()=default;
    Q_INVOKABLE QStringList getMountRootPaths();
    Q_INVOKABLE QStringList getHumanLabelRootPaths();
    Q_INVOKABLE QString findMountRootPathsValueByHumanLabel(const QString &label);
signals:
    
public slots:
};

#endif // OPENFOLDERHANDLER_H
