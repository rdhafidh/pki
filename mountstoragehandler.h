#ifndef MOUNTSTORAGEHANDLER_H
#define MOUNTSTORAGEHANDLER_H

#include <QStringList>
#include <unordered_map>
#include <qhash.h>

namespace std
{
    template<> struct hash<QString>
    {
        std::size_t operator()(const QString& s) const noexcept
        {
            return qHash (s);
        }
    };
}

class MountStorageHandler
{
public:
    MountStorageHandler()=default;
    ~MountStorageHandler()=default;
    static std::unordered_map<QString,QString> getAllRootMountedFS();
};

#endif // MOUNTSTORAGEHANDLER_H
