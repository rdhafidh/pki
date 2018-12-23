## Simple Demo 
Cross platform Windows, linux, Android, dan mungkin iOS/Mac demo PKI Enkripsi & Dekripsi metode beragam. 
(Backend library [libsodium](https://github.com/jedisct1/libsodium) dan [Google stock boringSSL](https://boringssl.googlesource.com/boringssl/)).

Aead Cihpher dari [boringSSL](https://github.com/rdhafidh/pki/blob/master/boringsslpki.cpp#L27):
1. aes_256_gcm
2. xchacha20_poly1305
3. aes_256_gcm_siv

### Proses Enkripsi
![](https://raw.githubusercontent.com/rdhafidh/pki/master/doc/enc.png)

### Proses Dekripsi 
![](https://raw.githubusercontent.com/rdhafidh/pki/master/doc/dec.png)

### Pemilihan Backend Enkripsi/Dekripsi
![](https://raw.githubusercontent.com/rdhafidh/pki/master/doc/pengaturan.png)

### Dialog Folder
![](https://raw.githubusercontent.com/rdhafidh/pki/master/doc/dialog.jpeg)
![](https://raw.githubusercontent.com/rdhafidh/pki/master/doc/folder.jpeg)


#### [Windows installer](https://github.com/rdhafidh/pki/releases/download/1.0.3/InstallerDemoPKISetup.zip)  Min Windows 7


#### [Android](https://github.com/rdhafidh/pki/releases/download/1.0.3/android-debug.zip) Min jelly bean arm 32bit
