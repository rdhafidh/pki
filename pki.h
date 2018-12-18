#ifndef PKI_H
#define PKI_H

#include <string>
#include <QByteArray>
#include <QString>

struct KeyGenPair {
  std::string alice_pub;
  std::string alice_priv;
  std::string bob_pub;
  std::string bob_priv;
};

class PKI {
 public:
  PKI() = default;
  ~PKI() = default;
  virtual KeyGenPair doKeyGen() = 0;
  virtual bool encrypt(std::string &ciphertext, const std::string &msg,
                       const std::string &bob_pub,
                       const std::string &alice_priv) = 0;
  virtual bool decrypt(std::string &decrypted, const std::string &ciphertext,
                       const std::string &alice_pub,
                       const std::string &bob_priv) = 0;
//  virtual bool encrypt(const QByteArray &msg,
//                           const QString &bob_pub_file,
//                           const QString &alice_priv_file,QByteArray &out)=0;
//  virtual bool decrypt(const QByteArray &ciphertext, const QString &alice_pub_file,
//                      const QString &bob_priv_file,QByteArray &out)=0;
  virtual bool genPairKey(const QString &folder)=0;
  virtual std::string provider() const = 0;
};

#endif  // PKI_H
