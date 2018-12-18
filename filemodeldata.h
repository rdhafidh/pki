#ifndef FILEMODELDATA_H
#define FILEMODELDATA_H
#include <QAbstractListModel>
#include <filesystemabs.h>
#include <QString>
#include <QHash>

class FileModelData : public QAbstractListModel
{
    Q_OBJECT
    
    bool isfile(const std::string &path);
public: 
    enum FileModelEnum{
        ObjectRole=Qt::UserRole+1,
        NameRole,
        SizeRole,
        LastModifTimeRole
    };
    QHash<int,QByteArray> roleNames () const override;
    
    FileModelData(QObject *parent=nullptr);
    int rowCount (const QModelIndex &) const override;
    QVariant data (const QModelIndex &index, int role) const override;
    
    ~FileModelData()=default; 
    Q_INVOKABLE bool setPath(const QString &path);
    Q_INVOKABLE bool cd(const QString &path);
    Q_INVOKABLE void refresh(); 
    Q_INVOKABLE void makeClear(); 
    Q_INVOKABLE QString path(); 
    Q_INVOKABLE QString documentPath();
    Q_INVOKABLE bool mkdir(const QString&fname);
    Q_INVOKABLE void setNameFilterKey();
    Q_INVOKABLE void clearNameFilter();
};

#endif // FILEMODELDATA_H
