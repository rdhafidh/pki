#ifndef PKIQML_H
#define PKIQML_H

#include <QObject>
#include <QString>
#include <memory>
#include <pkisodium.h>

class PKIQml : public QObject
{
    Q_OBJECT
public:
    explicit PKIQml(QObject *parent = nullptr);
    ~PKIQml(){}
    Q_INVOKABLE bool genPairKey(const QString &folder);
    Q_INVOKABLE bool encrypt(const QByteArray &msg,
                             const QString &bob_pub_file,
                             const QString &alice_priv_file);
    Q_INVOKABLE bool decrypt(const QByteArray &ciphertext, const QString &alice_pub_file,
                        const QString &bob_priv_file);
    Q_INVOKABLE void resetEnkripsiBasedSetting();
    Q_INVOKABLE void copyClipboard(const QString &text);
    Q_INVOKABLE QString pasteClipboard(); 
signals:
    void failWritePrivFile(const QString &msg);
    void failWritePubFile(const QString &msg);
    void encryptSuccess(const QByteArray &ct);
    void encryptReadPubFailed();
    void encryptReadPrivFailed();
    void decryptSuccess(const QByteArray &msg);
    void decryptReadPubFailed();
    void decryptReadPrivFailed();
    
public slots:
    
private:
    std::unique_ptr<PKI> pki;
};

#endif // PKIQML_H
