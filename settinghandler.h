#ifndef SETTINGHANDLER_H
#define SETTINGHANDLER_H

#include <QString>
#include <QVariant>
#include <QObject>

class SettingHandler :public QObject
{
    Q_OBJECT
    QString msettingfile;
public:
    SettingHandler(const QString &fname,QObject *parent=nullptr);
    ~SettingHandler()=default;
    Q_INVOKABLE bool setValue(const QString &key,const QVariant &value);
    Q_INVOKABLE QVariant getValue(const QString &key,const QVariant &defaultval);
};

#endif // SETTINGHANDLER_H
