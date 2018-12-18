#include "filemodeldata.h"
#include <QDebug>
#include <QFileInfo>
#include <QStandardPaths>

QHash<int, QByteArray> FileModelData::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[ObjectRole] = "isdirf";
  roles[NameRole] = "namef";
  roles[SizeRole] = "sizef";
  roles[LastModifTimeRole] = "ltimef";
  return roles;
}

FileModelData::FileModelData(QObject *parent) : QAbstractListModel(parent) {}

int FileModelData::rowCount(const QModelIndex &) const {
  return static_cast<int>(FileSystemAbs::instance()->getFiles().size());
}

QVariant FileModelData::data(const QModelIndex &index, int role) const {
  QVariant var = QVariant();
  if (!index.isValid()) return var;

  switch (role) {
    case ObjectRole:
      var = (bool)FileSystemAbs::instance()->getFiles().at(index.row())->isdir;
      break;
    case NameRole:
      var = 
          FileSystemAbs::instance()->getFiles().at(index.row())->basename;
      break;
    case SizeRole:
      var =  
          FileSystemAbs::instance()->getFiles().at(index.row())->size;
      break;
    case LastModifTimeRole:
      var = 
          FileSystemAbs::instance()->getFiles().at(index.row())->ltimef;
      break;
    default:
      break;
  }
  return var;
}

bool FileModelData::setPath(const QString &path) {
  beginResetModel();
  bool e = FileSystemAbs::instance()->setPath(path);
  endResetModel();
  return e;
}

bool FileModelData::cd(const QString &path) {
  beginResetModel();
  bool e = FileSystemAbs::instance()->cd(path);
  endResetModel();
  return e;
}

void FileModelData::refresh() { 
    beginResetModel();
    FileSystemAbs::instance()->refresh(); 
    endResetModel();
}

void FileModelData::makeClear() { FileSystemAbs::instance()->makeClear(); }

QString FileModelData::path() {
  return FileSystemAbs::instance()->getPath();
}
QString FileModelData::documentPath() {
  QStringList dir =
      QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
  if (dir.size() == 0) {
    return "";
  }
  return dir.at(0);
}

bool FileModelData::mkdir(const QString &fname)
{
    return FileSystemAbs::instance()->mkdir (fname);
}

void FileModelData::setNameFilterKey()
{
    FileSystemAbs::instance ()->setNameFilters (QStringList()<<".priv"<<".pub");
}

void FileModelData::clearNameFilter()
{
    FileSystemAbs::instance ()->clearNameFilters ();
}
