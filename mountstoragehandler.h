#ifndef MOUNTSTORAGEHANDLER_H
#define MOUNTSTORAGEHANDLER_H

#include <QStringList>

class MountStorageHandler
{
public:
    MountStorageHandler()=default;
    ~MountStorageHandler()=default;
    static QStringList getAllRootMountedFS();
};

#endif // MOUNTSTORAGEHANDLER_H
