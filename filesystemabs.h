#ifndef FILESYSTEMABS_H
#define FILESYSTEMABS_H

#include <cstdint>
#include <memory> 
#include <vector>
#include <iostream>
#include <QtGlobal>
#include <QStringList>

struct FileObj {
  ~FileObj();
  QString basename;
  bool isdir;    
  bool issym;
  QString size;
  QString ltimef;
  static std::shared_ptr<FileObj> makeFileObj(const QString &bname,
                                       qint64 size, bool isdir, bool issym, bool exists, const QString &ltimef);
  bool exists; 
  static QString computeSize(const qint64 size) ;
};

typedef std::vector<std::shared_ptr<FileObj> > ListFiles;

class FileSystemAbs {
 QString path; 
 QStringList nameFilters;
 ListFiles files;
 ListFiles getListDir(); 
 bool isValidPath(const QString &path);
 public:
  FileSystemAbs() = default;
  static FileSystemAbs *instance();
  ~FileSystemAbs();
  FileSystemAbs(const FileSystemAbs&other);
  FileSystemAbs&operator =(const FileSystemAbs&other)=default; 
  FileSystemAbs(FileSystemAbs &&other);
  bool setPath(const QString &pth);
  void refresh();
  void makeClear();
  bool cd(const QString &pth);
  bool mkdir(const QString &fname);
  void setNameFilters(const QStringList &filter);
  void clearNameFilters();
  
  ListFiles getFiles() const;
  QString getPath()const;
};

std::ostream &operator<<(std::ostream &os, const FileSystemAbs&list);

#endif  // FILESYSTEMABS_H
