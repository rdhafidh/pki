#include "pkisodium.h"
#include <sodium.h>
#include <util.h>
#include <z85.h>
#include <QFile>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

KeyGenPair PKISodium::doKeyGen() {
  KeyGenPair keypair;
  keypair.alice_priv.resize(crypto_box_SECRETKEYBYTES);
  keypair.alice_pub.resize(crypto_box_PUBLICKEYBYTES);
  keypair.bob_priv.resize(crypto_box_SECRETKEYBYTES);
  keypair.bob_pub.resize(crypto_box_PUBLICKEYBYTES);
  crypto_box_keypair((unsigned char *)keypair.alice_pub.data(),
                     (unsigned char *)keypair.alice_priv.data());
  crypto_box_keypair((unsigned char *)keypair.bob_pub.data(),
                     (unsigned char *)keypair.bob_priv.data());
  return keypair;
}

bool PKISodium::keygenTests() {
  std::unique_ptr<PKISodium> sod = std::make_unique<PKISodium>();
  auto ret = sod->doKeyGen();

  remove("alice.priv");
  remove("alice.pub");
  remove("bob.pub");
  remove("bob.priv");
  if (!Util::saveFile("alice.priv", ret.alice_priv)) {
    return false;
  }
  if (!Util::saveFile("alice.pub", ret.alice_pub)) {
    return false;
  }
  if (!Util::saveFile("bob.pub", ret.bob_pub)) {
    return false;
  }
  if (!Util::saveFile("bob.priv", ret.bob_priv)) {
    return false;
  }

  std::string o_a_priv = "";
  if (!Util::readFile("alice.priv", o_a_priv)) {
    return false;
  }
  if (o_a_priv != ret.alice_priv) {
    Util::logging(" read test alice priv galat");
    return false;
  }
  std::string o_a_pub = "";
  if (!Util::readFile("alice.pub", o_a_pub)) {
    return false;
  }
  if (o_a_pub != ret.alice_pub) {
    Util::logging("read test alice pub galat");
    return false;
  }
  std::string o_b_priv = "";
  if (!Util::readFile("bob.priv", o_b_priv)) {
    return false;
  }
  if (o_b_priv != ret.bob_priv) {
    Util::logging("read test bob priv galat");
    return false;
  }
  std::string o_b_pub = "";
  if (!Util::readFile("bob.pub", o_b_pub)) {
    return false;
  }
  if (o_b_pub != ret.bob_pub) {
    Util::logging(" read test bob pub galat");
    return false;
  }
  return true;
}

bool PKISodium::encrypt(std::string &ciphertext, const std::string &msg,
                        const std::string &bob_pub,
                        const std::string &alice_priv) {
  ciphertext.clear();
  ciphertext.resize(crypto_box_MACBYTES + msg.size());
  std::string nonce;
  nonce.resize(crypto_box_NONCEBYTES);
  randombytes_buf((void *)nonce.data(), nonce.size());
  if (crypto_box_easy((unsigned char *)ciphertext.data(),
                      (const unsigned char *)msg.c_str(),
                      (unsigned long long)msg.size(),
                      (const unsigned char *)nonce.c_str(),
                      (const unsigned char *)bob_pub.c_str(),
                      (const unsigned char *)alice_priv.c_str()) != 0) {
    return false;
  }
  ciphertext += nonce;
  return true;
}

bool PKISodium::decrypt(std::string &decrypted, const std::string &ciphertext,
                        const std::string &alice_pub,
                        const std::string &bob_priv) {
  if (ciphertext.size() == 0 || alice_pub.size() == 0 || bob_priv.size() == 0)
    return false;

  std::string c =
      ciphertext.substr(0, ciphertext.size() - crypto_box_NONCEBYTES);
  std::string n = ciphertext.substr(ciphertext.size() - crypto_box_NONCEBYTES,
                                    ciphertext.size());
  decrypted.clear();
  decrypted.resize(c.size() - crypto_box_MACBYTES);
  if (crypto_box_open_easy(
          (unsigned char *)decrypted.data(), (const unsigned char *)c.data(),
          (unsigned long long)c.size(), (const unsigned char *)n.c_str(),
          (const unsigned char *)alice_pub.c_str(),
          (const unsigned char *)bob_priv.c_str()) != 0) {
    return false;
  }
  return true;
}

bool PKISodium::encryptqt(const QByteArray &msg, const QString &bob_pub_file,
                          const QString &alice_priv_file, QByteArray &out) {
  std::string pub = "";
  std::string priv = "";
  if (!Util::readFile(bob_pub_file, pub) || pub.empty()) {
    return false;
  }
  if (!Util::readFile(alice_priv_file, priv) || priv.empty()) {
    return false;
  }
  std::string ct = "";
  if (!encrypt(ct, msg.toStdString(), pub, priv)) {
    return false;
  }
  out = QByteArray::fromStdString(Util::encode85(ct));
  return true;
}

bool PKISodium::decryptqt(const QByteArray &ciphertext,
                          const QString &alice_pub_file,
                          const QString &bob_priv_file, QByteArray &out) {
  std::string pub = "";
  std::string priv = "";
  if (!Util::readFile(alice_pub_file, pub) || pub.empty()) {
    return false;
  }
  if (!Util::readFile(bob_priv_file, priv) || priv.empty()) {
    return false;
  }
  std::string dt = "";
  std::string decode = Util::decode85(ciphertext.toStdString());
  if (decode.size() == 0) {
    return false;
  }
  if (!decrypt(dt, decode, pub, priv)) {
    Util::logging("dec sodium failed");
    return false;
  }
  out = QByteArray::fromStdString(dt);
  return true;
}

bool PKISodium::genPairKey(const QString &folder) {
  KeyGenPair pair = doKeyGen();
  if (!Util::saveFile(folder + "/" + "alice.priv", pair.alice_priv)) {
    return false;
  }
  if (!Util::saveFile(folder + "/" + "alice.pub", pair.alice_pub)) {
    return false;
  }
  if (!Util::saveFile(folder + "/" + "bob.priv", pair.bob_priv)) {
    return false;
  }
  if (!Util::saveFile(folder + "/" + "bob.pub", pair.bob_pub)) {
    return false;
  }
  return true;
}

std::string PKISodium::provider() const { return "sodium"; }
