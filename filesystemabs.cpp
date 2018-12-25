#include "filesystemabs.h"
#include <mountstoragehandler.h>
#include <util.h>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <algorithm>
#include <sstream>
#include <utility>
 
FileObj FileObj::makeFileObj(const QString &bname, qint64 size, bool isdir,
                             bool issym, const QString &ltimef) {
  FileObj obj;
  obj.basename = bname;
  obj.isdir = isdir;
  obj.issym = issym;
  obj.size = computeSize(size);
  obj.ltimef = ltimef;
  return obj;
}

QString FileObj::computeSize(const qint64 size) {
  std::vector<QString> mb = {"bytes", "KB", "MB", "GB", "TB",
                             "PB",    "EB", "ZB", "YB"};
  int l = 0;
  qint64 n = size;
  while (n >= 1024) {
    n = n / 1024;
    ++l;
  }
  return QObject::tr("%1 %2").arg(n).arg(mb.at(l));
}

ListFiles FileSystemAbs::getListDir() {
  ListFiles list;
  if (!isValidPath(path)) return list;
  Util::logging(QObject::tr("%1 %2").arg(Q_FUNC_INFO).arg(__LINE__));

  QDir dir(path);
  QFileInfoList mret = dir.entryInfoList(
      QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Readable,
      QDir::DirsFirst | QDir::Time);
  for (QFileInfo file : mret) {
    if (nameFilters.size() > 0) {
      if (file.isFile() &&
          std::any_of(nameFilters.begin(), nameFilters.end(),
                      [&file](const QString &filter) {
                        return file.fileName().endsWith(filter);
                      })) {
        list.emplace_back(FileObj::makeFileObj(
            file.fileName(), file.size(), file.isDir(), file.isSymLink(),
            file.lastModified().toString("d/M/yyyy hh:m:s")));
      }
      if (file.isDir()) {
        list.emplace_back(FileObj::makeFileObj(
            file.fileName(), file.size(), file.isDir(), file.isSymLink(),
            file.lastModified().toString("d/M/yyyy hh:m:s")));
      }
    } else {
      list.emplace_back(FileObj::makeFileObj(
          file.fileName(), file.size(), file.isDir(), file.isSymLink(),
          file.lastModified().toString("d/M/yyyy hh:m:s")));
    }
  }
  Util::logging(QObject::tr("%1 %2").arg(Q_FUNC_INFO).arg(__LINE__));
  mret.clear();

  return list;
}

bool FileSystemAbs::isValidPath(const QString &path) {
  QFileInfo inf(path);
  return inf.exists() && inf.isReadable() && inf.isDir();
}

FileSystemAbs *FileSystemAbs::instance() {
  static FileSystemAbs files;
  return &files;
}

FileSystemAbs::~FileSystemAbs() { makeClear(); }

FileSystemAbs::FileSystemAbs(const FileSystemAbs &other) {
  setPath(other.getPath());
}

FileSystemAbs::FileSystemAbs(FileSystemAbs &&other) : FileSystemAbs(other) {
  other.makeClear();
}

bool FileSystemAbs::setPath(const QString &pth) {
  if (!isValidPath(pth)) return false;

  Util::logging(QObject::tr("%1 %2 setpath: %3")
                    .arg(Q_FUNC_INFO)
                    .arg(__LINE__)
                    .arg(pth));
  path = QDir::cleanPath(pth);
  refresh();
  Util::logging(QObject::tr("%1 %2 setpath: %3")
                    .arg(Q_FUNC_INFO)
                    .arg(__LINE__)
                    .arg(pth));
  return true;
}

void FileSystemAbs::refresh() {
  files.clear();
  files = getListDir();
}

void FileSystemAbs::makeClear() {
  files.clear();
  path = "";
}

bool FileSystemAbs::cd(const QString &pth) {
  if (!isValidPath(path)) {
    return false;
  }
  auto roots = MountStorageHandler::getAllRootMountedFS();
  if (std::any_of(roots.begin(), roots.end(),
                  [this](const std::pair<QString, QString> &rootdir) {
                    return this->path == rootdir.second;
                  }) &&
      pth == "..") {
    Util::logging(QObject::tr("%1 %2").arg(Q_FUNC_INFO).arg(__LINE__));
    return false;
  }
  Util::logging(QObject::tr("%1 %2").arg(Q_FUNC_INFO).arg(__LINE__));
  auto mpath = QDir::cleanPath(path + "/" + pth);
  if (!isValidPath(mpath)) {
    return false;
  }
  path = mpath;
  refresh();
  Util::logging(QObject::tr("%1 %2").arg(Q_FUNC_INFO).arg(__LINE__));
  return true;
}

bool FileSystemAbs::mkdir(const QString &fname) {
  QDir dir(path);
  return dir.mkdir(fname);
}

void FileSystemAbs::setNameFilters(const QStringList &filter) {
  nameFilters = filter;
}

void FileSystemAbs::clearNameFilters() { nameFilters.clear(); }

ListFiles FileSystemAbs::getFiles() const { return files; }

QString FileSystemAbs::getPath() const { return path; }

std::ostream &operator<<(std::ostream &os, const FileSystemAbs &list) {
  os << "folder list: ";
  for (size_t x = 0; x < list.getFiles().size(); x += 1) {
    os << list.getFiles().at(x).basename.toStdString()
       << " isdir: " << list.getFiles().at(x).isdir << " size "
       << list.getFiles().at(x).size.toStdString() << " last modified time "
       << list.getFiles().at(x).ltimef.toStdString() << " sym "
       << list.getFiles().at(x).issym << "";
  }
  os << "";
  return os;
}
