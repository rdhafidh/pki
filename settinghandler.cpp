#include "settinghandler.h"
#include <QSettings>
#ifdef Q_OS_ANDROID
#include <QStandardPaths>
#endif

SettingHandler::SettingHandler(const QString &fname, QObject *parent)
    : QObject(parent), msettingfile(fname) {}

bool SettingHandler::setValue(const QString &key, const QVariant &value) {
  if (msettingfile.isEmpty()) return false;
#ifdef Q_OS_ANDROID
  auto pth = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings file(pth + "/" + msettingfile, QSettings::IniFormat, this);
#else
  QSettings file(msettingfile, QSettings::IniFormat, this);
#endif
  file.setValue(key, value);
  return file.isWritable();
}

QVariant SettingHandler::getValue(const QString &key,
                                  const QVariant &defaultval) {
  if (msettingfile.isEmpty()) return QVariant();
#ifdef Q_OS_ANDROID
  auto pth = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QSettings file(pth + "/" + msettingfile, QSettings::IniFormat, this);
#else
  QSettings file(msettingfile, QSettings::IniFormat, this);
#endif
  return file.value(key, defaultval);
}
