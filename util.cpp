#include "util.h"
#include <QFile>
#include <z85.h>
#include <iostream>

bool Util::readFile(const QString &name, std::string &buffer)
{
    QFile fn(name);
    if (!fn.open(QIODevice::ReadOnly)) {
      return false;
    }
    buffer.clear();
    buffer=fn.readAll ().toStdString ();
    fn.close();
    return true;
}

bool Util::saveFile(const QString &fname, const std::string buffer)
{
    QFile fn(fname);
    if(!fn.open (QIODevice::WriteOnly)){
        return false;
    }
    fn.write (buffer.c_str (),buffer.size ());
    fn.close ();
    return true;
}

std::string Util::encode85(const std::string &buffer) {
  std::string ret;
  size_t len=Z85_encode_with_padding_bound(buffer.size());
  if(len==0) return "";
  ret.resize(len);
  len =
      Z85_encode_with_padding(buffer.c_str(), ret.data(), buffer.size());
  if (len == 0) {
    std::cout << "\nencode problem len 0\n";
    ret = "";
    return ret;
  }
  return ret;
}
std::string Util::decode85(const std::string &buffer) {
  std::string ret;
  size_t len=Z85_decode_with_padding_bound(buffer.c_str(), buffer.size());
  if(len==0){
	std::cout<<"\nZ85_decode_with_padding_bound: "<<len<<"\n";
	return "";
  }
  
  ret.resize(len); 
  len =
      Z85_decode_with_padding(buffer.c_str(), ret.data(), buffer.size());
  if (len == 0) {
    std::cout << "\ndecode problem len 0\n";
    ret = "";
    return ret;
  }
  return ret;
}
