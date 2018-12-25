#include "mountstoragehandler.h"
#include <QFileInfo>
#include <QStorageInfo>
#include <cstdlib>

std::string getEnvVar(std::string const& key) {
  char const* val = std::getenv(key.c_str());
  return val == NULL ? std::string() : std::string(val);
}

#ifdef Q_OS_ANDROID
bool isValidDir(const std::string& path) {
  QFileInfo inf(path.c_str());
  if (inf.exists() && inf.isDir() && inf.isReadable()) {
    return true;
  }
  return false;
}
#endif

std::unordered_map<QString,QString> MountStorageHandler::getAllRootMountedFS() {
  std::unordered_map<QString,QString> root_dir;
#ifdef Q_OS_ANDROID
  // FIXME more reliable detection usb otg
  // https://stackoverflow.com/questions/36086201/scan-otg-mount-point-in-android
  auto internal = getEnvVar("EXTERNAL_STORAGE");
  if(isValidDir(internal)){
      root_dir["internal"]=QString::fromStdString(internal);
  } 
  if(isValidDir ( "/storage/sdcard1")){
      root_dir["eksternal"]="/storage/sdcard1";
  }
  std::vector<std::string> otg = {
          "/storage/UsbDriveA", "/storage/USBstorage1",
      "/storage/usbdisk",     "/storage/usbotg",    "/storage/UDiskA",
      "/storage/usb-storage", "/storage/usbcard",   "/storage/usb"};
  int idx=1;
  for (const auto& p : otg) {
    if (isValidDir(p)) {
        root_dir[QObject::tr ("usb_%1").arg (idx)]=QString::fromStdString(p); 
        ++idx;
    }
  }
#else 
  for (const auto& mnt : QStorageInfo::mountedVolumes()) {
    if (mnt.isValid() && mnt.isReady()) {
      root_dir[QObject::tr ("DRIVE_%1").arg (mnt.rootPath().at(0))]=mnt.rootPath();
    } 
  }
#endif
  return root_dir;
}
