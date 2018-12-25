#include "openfolderhandler.h"
#include <unordered_map>
#include <QDebug>

OpenFolderHandler::OpenFolderHandler(QObject *parent) : QObject(parent) {}

QStringList OpenFolderHandler::getMountRootPaths() {
  std::unordered_map<QString, QString> drives =
      MountStorageHandler::getAllRootMountedFS();
  QStringList drv;
  for (const auto dr : drives) {
    drv << dr.second;
  }
  return drv;
}

QStringList OpenFolderHandler::getHumanLabelRootPaths() {
  std::unordered_map<QString, QString> drives =
      MountStorageHandler::getAllRootMountedFS();
  QStringList drv;
  for (const auto dr : drives) {
    drv << dr.first;
  }
  qDebug()<<Q_FUNC_INFO<<" "<<drv;
  return drv;
}

QString OpenFolderHandler::findMountRootPathsValueByHumanLabel(
    const QString &label) {
  std::unordered_map<QString, QString> drives =
      MountStorageHandler::getAllRootMountedFS();
  auto fnd = drives.find(label);
  if (fnd != drives.end()) {
    return fnd->second;
  }
  return "";
}
