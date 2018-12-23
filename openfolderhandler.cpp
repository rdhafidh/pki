#include "openfolderhandler.h"
#include <mountstoragehandler.h>

OpenFolderHandler::OpenFolderHandler(QObject *parent) : QObject(parent) {}

QStringList OpenFolderHandler::getMountRootPaths() {
    return MountStorageHandler::getAllRootMountedFS ();
}
