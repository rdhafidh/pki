#ifndef PKISODIUM_H
#define PKISODIUM_H

#include <pki.h>

class PKISodium : public PKI {
  bool encrypt(std::string &ciphertext, const std::string &msg,
               const std::string &bob_pub, const std::string &alice_priv);
  bool decrypt(std::string &decrypted, const std::string &ciphertext,
               const std::string &alice_pub, const std::string &bob_priv);

  KeyGenPair doKeyGen();

 public:
  PKISodium() = default;
  ~PKISodium() = default;

  bool encryptqt(const QByteArray &msg, const QString &bob_pub_file,
                 const QString &alice_priv_file, QByteArray &out) override;
  bool decryptqt(const QByteArray &ciphertext, const QString &alice_pub_file,
                 const QString &bob_priv_file, QByteArray &out) override;
  bool genPairKey(const QString &folder) override;
  std::string provider() const override;

  static bool keygenTests();
};

#endif  // PKISODIUM_H
