#include "boringsslpki.h"
#include <openssl/curve25519.h>
#include <openssl/digest.h>
#include <openssl/hkdf.h>
#include <openssl/rand.h>
#include <settinghandler.h>
#include <util.h>
#include <iostream>

#define DEFAULT_AD_LENGTH 16
#define DEFAULT_KDF_SALT_LENGTH 32
#define DEFAULT_KDF_INFO_LENGTH 16

class RandWrapper {
 public:
  RandWrapper() = default;
  ~RandWrapper() { RAND_cleanup(); }
  static RandWrapper *instance() {
    static RandWrapper rande;
    return &rande;
  }
  bool makeRandom(uint8_t *buf, size_t len) {
    return RAND_bytes(buf, len) == 1;
  }
};

BoringsslPKI::BoringsslPKI(int idx) {
  currentAeadIndex = idx;
  if (currentAeadIndex < 0 || currentAeadIndex >= 3) {
    currentAeadIndex = 1;
  }
  ciphers.push_back(AEADCipher(EVP_aead_aes_256_gcm(), "aes_256_gcm"));
  ciphers.push_back(
      AEADCipher(EVP_aead_xchacha20_poly1305(), "xchacha20_poly1305"));
  ciphers.push_back(AEADCipher(EVP_aead_aes_256_gcm_siv(), "aes_256_gcm_siv"));
}

std::vector<AEADCipher> BoringsslPKI::getCiphers() const { return ciphers; }

bool BoringsslPKI::encryptqt(const QByteArray &msg, const QString &bob_pub_file,
                             const QString &alice_priv_file, QByteArray &out) {
  std::string pub;
  std::string priv;
  if (!Util::readFile(bob_pub_file, pub) || pub.empty()) {
    return false;
  }
  if (!Util::readFile(alice_priv_file, priv) || priv.empty()) {
    return false;
  }
  std::string shared;
  shared.resize(32);
  int ret = X25519((uint8_t *)shared.data(), (const uint8_t *)priv.c_str(),
                   (const uint8_t *)pub.c_str());
  if (ret == 0) {
    return false;
  }
  if (shared.size() != EVP_AEAD_key_length(ciphers.at(currentAeadIndex).aead)) {
    std::cout << "\nfound ciphers: " << ciphers.at(currentAeadIndex).name
              << " not 32 bytes key length properties: "
              << EVP_AEAD_key_length(ciphers.at(currentAeadIndex).aead) << "\n";
    return false;
  }
  std::string random;
  random.resize(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead) +
                DEFAULT_AD_LENGTH + DEFAULT_KDF_SALT_LENGTH +
                DEFAULT_KDF_INFO_LENGTH);
  if (!RandWrapper::instance()->makeRandom((uint8_t *)random.data(),
                                           random.size())) {
    std::cout << "\nmake random key failed ..\n";
    return false;
  }
  std::string key;
  key.resize(EVP_AEAD_key_length(ciphers.at(currentAeadIndex).aead));
  ret = HKDF(
      (uint8_t *)key.data(), key.size(), EVP_sha256(),
      (const uint8_t *)shared.c_str(), shared.size(),
      (const uint8_t *)random
          .substr(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead) +
                      DEFAULT_AD_LENGTH,
                  DEFAULT_KDF_SALT_LENGTH)
          .c_str(),
      DEFAULT_KDF_SALT_LENGTH,
      (const uint8_t *)random
          .substr(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead) +
                      DEFAULT_AD_LENGTH + DEFAULT_KDF_SALT_LENGTH,
                  DEFAULT_KDF_INFO_LENGTH)
          .c_str(),
      DEFAULT_KDF_INFO_LENGTH);
  if (ret == 0) {
    std::cout << "\nhkdf failed";
    return false;
  }

  bssl::ScopedEVP_AEAD_CTX ctx;

  ret = EVP_AEAD_CTX_init(ctx.get(), ciphers.at(currentAeadIndex).aead,
                          (const uint8_t *)key.c_str(), key.size(), 0, nullptr);
  if (ret == 0) {
    std::cout << "\nEVP_AEAD_CTX_init failed";
    return false;
  }
  std::string ct;
  ct.resize(msg.size() +
            EVP_AEAD_max_overhead(ciphers.at(currentAeadIndex).aead));
  size_t ow = 0;
  ret = EVP_AEAD_CTX_seal(
      ctx.get(), (uint8_t *)ct.data(), &ow, ct.size(),
      (const uint8_t *)random
          .substr(0, EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead))
          .c_str(),
      EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead),
      (const uint8_t *)msg.toStdString().c_str(), msg.size(),
      (const uint8_t *)random
          .substr(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead),
                  DEFAULT_AD_LENGTH)
          .c_str(),
      DEFAULT_AD_LENGTH);
  if (ret == 0) {
    std::cout << "\nseal failed\n";
    return false;
  }
  ct += random;
  out = QByteArray::fromStdString(Util::encode85(ct));
  return true;
}

bool BoringsslPKI::decryptqt(const QByteArray &ciphertext,
                             const QString &alice_pub_file,
                             const QString &bob_priv_file, QByteArray &out) {
  if (ciphertext.isEmpty()) {
    return false;
  }
  std::string pub;
  std::string priv;
  if (!Util::readFile(alice_pub_file, pub) || pub.empty()) {
    return false;
  }
  if (!Util::readFile(bob_priv_file, priv) || priv.empty()) {
    return false;
  }
  std::string shared;
  shared.resize(32);
  int ret = X25519((uint8_t *)shared.data(), (const uint8_t *)priv.c_str(),
                   (const uint8_t *)pub.c_str());
  if (ret == 0) {
    return false;
  }
  std::string plain = Util::decode85(ciphertext.toStdString());
  if (plain.size() == 0) {
    std::cout << "\ndecode85 failed";
    return false;
  }
  std::string random = plain.substr(
      plain.size() - (EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead) +
                      DEFAULT_AD_LENGTH + DEFAULT_KDF_SALT_LENGTH +
                      DEFAULT_KDF_INFO_LENGTH),
      plain.size());
  if (random.empty()) {
    return false;
  }

  std::string key;
  key.resize(EVP_AEAD_key_length(ciphers.at(currentAeadIndex).aead));
  ret = HKDF(
      (uint8_t *)key.data(), key.size(), EVP_sha256(),
      (const uint8_t *)shared.c_str(), shared.size(),
      (const uint8_t *)random
          .substr(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead) +
                      DEFAULT_AD_LENGTH,
                  DEFAULT_KDF_SALT_LENGTH)
          .c_str(),
      DEFAULT_KDF_SALT_LENGTH,
      (const uint8_t *)random
          .substr(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead) +
                      DEFAULT_AD_LENGTH + DEFAULT_KDF_SALT_LENGTH,
                  DEFAULT_KDF_INFO_LENGTH)
          .c_str(),
      DEFAULT_KDF_INFO_LENGTH);
  if (ret == 0) {
    std::cout << "\nhkdf failed";
    return false;
  }

  bssl::ScopedEVP_AEAD_CTX ctx;
  ret = EVP_AEAD_CTX_init(ctx.get(), ciphers.at(currentAeadIndex).aead,
                          (const uint8_t *)key.c_str(), key.size(), 0, nullptr);
  if (ret == 0) {
    std::cout << "\nEVP_AEAD_CTX_init failed";
    return false;
  }
  std::string dt;
  dt.resize(plain.size() - random.size() -
            EVP_AEAD_max_overhead(ciphers.at(currentAeadIndex).aead));
  if (dt.size() <= 0) {
    return false;
  }
  size_t ow = 0;
  ret = EVP_AEAD_CTX_open(
      ctx.get(), (uint8_t *)dt.data(), &ow, dt.size(),
      (const uint8_t *)random
          .substr(0, EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead))
          .c_str(),
      EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead),
      (const uint8_t *)plain
          .substr(0, dt.size() + EVP_AEAD_max_overhead(
                                     ciphers.at(currentAeadIndex).aead))
          .c_str(),
      dt.size() + EVP_AEAD_max_overhead(ciphers.at(currentAeadIndex).aead),
      (const uint8_t *)random
          .substr(EVP_AEAD_nonce_length(ciphers.at(currentAeadIndex).aead),
                  DEFAULT_AD_LENGTH)
          .c_str(),
      DEFAULT_AD_LENGTH);
  if (ret == 0) {
    std::cout << "\nopen failed\n";
    return false;
  }
  out = QByteArray::fromStdString(dt);
  return true;
}

bool BoringsslPKI::genPairKey(const QString &folder) {
  std::string alice_pv;
  alice_pv.resize(32);
  std::string alice_pb;
  alice_pb.resize(32);
  std::string bob_pv;
  bob_pv.resize(32);
  std::string bob_pb;
  bob_pb.resize(32);
  X25519_keypair((uint8_t *)alice_pb.data(), (uint8_t *)alice_pv.data());

  X25519_keypair((uint8_t *)bob_pb.data(), (uint8_t *)bob_pv.data());
  if (!Util::saveFile(folder + "/" + "alice.pub", alice_pb)) {
    return false;
  }
  if (!Util::saveFile(folder + "/" + "alice.priv", alice_pv)) {
    return false;
  }
  if (!Util::saveFile(folder + "/" + "bob.pub", bob_pb)) {
    return false;
  }
  if (!Util::saveFile(folder + "/" + "bob.priv", bob_pv)) {
    return false;
  }
  return true;
}

std::string BoringsslPKI::provider() const { return "boringssl"; }
