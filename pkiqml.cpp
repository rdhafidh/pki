#include "pkiqml.h"
#include <QClipboard>
#include <QFile>
#include <QGuiApplication> 
#include <iostream>


PKIQml::PKIQml(QObject *parent) : QObject(parent) {
  pki = std::make_unique<PKISodium>();
}

bool PKIQml::genPairKey(const QString &folder) {
  KeyGenPair pair = pki->doKeyGen();
  if (!PKISodium::saveFile(folder + "/" + "alice.priv", pair.alice_priv)) {
    Q_EMIT failWritePrivFile(tr("Galat menulis alice private key"));
    return false;
  }
  if (!PKISodium::saveFile(folder + "/" + "alice.pub", pair.alice_pub)) {
    Q_EMIT failWritePubFile(tr("Galat menulis alice public key"));
    return false;
  }
  if (!PKISodium::saveFile(folder + "/" + "bob.priv", pair.bob_priv)) {
    Q_EMIT failWritePrivFile(tr("Galat menulis bob private key"));
    return false;
  }
  if (!PKISodium::saveFile(folder + "/" + "bob.pub", pair.bob_pub)) {
    Q_EMIT failWritePubFile(tr("Galat menulis bob public key"));
    return false;
  }
  return true;
}

bool PKIQml::encrypt(const QByteArray &msg, const QString &bob_pub_file,
                     const QString &alice_priv_file) {
 
  QByteArray out="";
  if(!pki->encryptqt(msg,bob_pub_file,alice_priv_file,out)){
	return false;
  } 
  this->encryptSuccess(out);
  return true;
}

bool PKIQml::decrypt(const QByteArray &ciphertext,
                     const QString &alice_pub_file,
                     const QString &bob_priv_file) {
  
  QByteArray out="";
  if(!pki->decryptqt(ciphertext,alice_pub_file,bob_priv_file,out)){
		return false;
  }
  this->decryptSuccess(out);
  return true;
}

void PKIQml::copyClipboard(const QString &text) {
  QClipboard *clip = qApp->clipboard();
  if (clip) {
    clip->setText(text);
  }
}

QString PKIQml::pasteClipboard() {
  QClipboard *clip = qApp->clipboard();
  if (clip) {
    return clip->text();
  }
  return "";
}
