#ifndef PKISODIUM_H
#define PKISODIUM_H

#include <pki.h>

class PKISodium : public PKI {
 public:
  PKISodium() = default;
  ~PKISodium(){}
  static bool readFile(const QString &name, std::string &buffer);
  static bool saveFile(const QString &name, const std::string &buffer);
  
  KeyGenPair doKeyGen() override;
  bool encrypt(std::string &ciphertext, const std::string &msg,
               const std::string &bob_pub,
               const std::string &alice_priv) override;
  bool decrypt(std::string &decrypted, const std::string &ciphertext,
               const std::string &alice_pub,
               const std::string &bob_priv) override;
  bool encryptqt(const QByteArray &msg, const QString &bob_pub_file,
               const QString &alice_priv_file, QByteArray &out) ;
  bool decryptqt(const QByteArray &ciphertext, const QString &alice_pub_file,
               const QString &bob_priv_file, QByteArray &out) ;
  bool genPairKey(const QString &folder) override;
  std::string provider() const override;
  static std::string decode85(const std::string &buffer);
  static std::string encode85(const std::string &buffer);
  
  static bool keygenTests();
};

#endif  // PKISODIUM_H
