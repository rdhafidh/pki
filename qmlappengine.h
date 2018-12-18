#ifndef QMLAPPENGINE_H
#define QMLAPPENGINE_H
#include <QQmlApplicationEngine>

class QMLAppEngine: public QQmlApplicationEngine
{
Q_OBJECT
public:
    QMLAppEngine(QObject *parent=nullptr):QQmlApplicationEngine(parent){}
    ~QMLAppEngine()=default;

    Q_INVOKABLE void hapusCache() {
        clearComponentCache();
    } 
}; 

#endif
