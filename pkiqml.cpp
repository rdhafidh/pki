#include "pkiqml.h"
#include <boringsslpki.h>
#include <settinghandler.h>
#include <util.h>
#include <QClipboard>
#include <QFile>
#include <QGuiApplication>
#include <iostream>

PKIQml::PKIQml(QObject *parent) : QObject(parent) {
  pki = std::make_unique<PKISodium>();
}

bool PKIQml::genPairKey(const QString &folder) {
  return pki->genPairKey(folder);
}

bool PKIQml::encrypt(const QByteArray &msg, const QString &bob_pub_file,
                     const QString &alice_priv_file) {
  QByteArray out = "";
  if (!pki->encryptqt(msg, bob_pub_file, alice_priv_file, out)) {
    return false;
  }
  this->encryptSuccess(out);
  return true;
}

bool PKIQml::decrypt(const QByteArray &ciphertext,
                     const QString &alice_pub_file,
                     const QString &bob_priv_file) {
  QByteArray out = "";
  if (!pki->decryptqt(ciphertext, alice_pub_file, bob_priv_file, out)) {
    return false;
  }
  this->decryptSuccess(out);
  return true;
}

void PKIQml::resetEnkripsiBasedSetting() {
  SettingHandler set("konfig.ini");
  QVariant ret = set.getValue("backend", 0);
  if (ret.toInt() == 1) {
    ret = set.getValue("algo", 1);
    pki.reset(new BoringsslPKI(ret.toInt()));
  } else {
    pki.reset(new PKISodium);
  }
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
