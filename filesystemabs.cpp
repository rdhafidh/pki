#include "filesystemabs.h"
#include <mountstoragehandler.h>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <algorithm>
#include <sstream>

FileObj::~FileObj() {
  //  std::cout <<"\n~FileObj()\n";
}

std::shared_ptr<FileObj> FileObj::makeFileObj(const QString &bname, qint64 size,
                                              bool isdir, bool issym,
                                              bool exists,
                                              const QString &ltimef) {
  std::shared_ptr<FileObj> obj = std::make_shared<FileObj>();
  obj->basename = bname;
  obj->isdir = isdir;
  obj->exists = exists;
  obj->issym = issym;
  obj->size = computeSize(size);
  obj->ltimef = ltimef;
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

  QDir dir(path);
  QFileInfoList mret = dir.entryInfoList(
      QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Readable,
      QDir::DirsFirst | QDir::Time);
  for (QFileInfo file : mret) {
    list.emplace_back(FileObj::makeFileObj(
        file.fileName(), file.size(), file.isDir(), file.isSymLink(),
        file.exists(), file.lastModified().toString("d/M/yyyy hh:m:s")));
  }
  mret.clear();

  if (nameFilters.size() > 0) {
    size_t idx = 0;
    std::vector<size_t> remove;
    for (auto mfile : list) {
      if (mfile && !mfile->isdir) {
        if (std::none_of(nameFilters.begin(), nameFilters.end(),
                         [mfile](const QString &filter) {
                           if (mfile && mfile->basename.endsWith(filter)) {
                             return true;
                           }
                           return false;
                         })) {
          remove.push_back(idx);
        }
      }
      ++idx;
    }
    for (size_t rm : remove) {
      list[rm] = nullptr;
      list.erase(list.begin() + rm);
    }
  }

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

  path = QDir::cleanPath(pth);
  refresh();
  return true;
}

void FileSystemAbs::refresh() {
  for (auto file : files) {
    if (file) file = nullptr;
  }
  files.clear();
  files = getListDir();
}

void FileSystemAbs::makeClear() {
  for (auto file : files) {
    if (file) file = nullptr;
  }
  files.clear();
  path = "";
}

bool FileSystemAbs::cd(const QString &pth) {
  if (!isValidPath(path)) {
    return false;
  }
  QStringList roots = MountStorageHandler::getAllRootMountedFS();
  if (std::any_of(
          roots.begin(), roots.end(),
          [this](const QString &rootdir) { return this->path == rootdir; }) &&
      pth == "..") {
    std::cout << "\nreached root dir can't simply cdUp again\n";
    return false;
  }
  auto mpath = QDir::cleanPath(path + "/" + pth);
  if (!isValidPath(mpath)) {
    return false;
  }
  path = mpath;
  refresh();
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
  os << "\nfolder list: \n";
  for (size_t x = 0; x < list.getFiles().size(); x += 1) {
    if (list.getFiles().at((x))) {
      os << list.getFiles().at(x)->basename.toStdString()
         << " isdir: " << list.getFiles().at(x)->isdir << " exists "
         << list.getFiles().at(x)->exists << " size "
         << list.getFiles().at(x)->size.toStdString() << " last modified time "
         << list.getFiles().at(x)->ltimef.toStdString() << " sym "
         << list.getFiles().at(x)->issym << "\n";
    }
  }
  os << "\n";
  return os;
}
