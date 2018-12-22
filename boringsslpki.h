#ifndef BORINGSSLPKI_H
#define BORINGSSLPKI_H

#include <openssl/aead.h>
#include <string>
#include <vector>
#include <pki.h>

struct AEADCipher {
  AEADCipher(const EVP_AEAD *a, const std::string &nm) : name(nm),aead(a) {}
  ~AEADCipher()=default;
  std::string name;
  const EVP_AEAD *aead;
};

class BoringsslPKI: public PKI {
  std::vector<AEADCipher> ciphers;
  int currentAeadIndex;
 public:
  BoringsslPKI(int idx=2);
  ~BoringsslPKI() = default; 
  std::vector<AEADCipher> getCiphers() const;
  
   bool encryptqt(const QByteArray &msg, const QString &bob_pub_file,
                const QString &alice_priv_file, QByteArray &out) override;
   bool decryptqt(const QByteArray &ciphertext, const QString &alice_pub_file,
                const QString &bob_priv_file, QByteArray &out) override;
   bool genPairKey(const QString &folder) override;
   std::string provider() const override;
};

#endif  // BORINGSSLPKI_H
