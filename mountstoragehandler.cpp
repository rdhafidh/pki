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

QStringList MountStorageHandler::getAllRootMountedFS() {
  QStringList root_dir;
#ifdef Q_OS_ANDROID
  // FIXME more reliable detection usb otg
  // https://stackoverflow.com/questions/36086201/scan-otg-mount-point-in-android
  auto internal = getEnvVar("EXTERNAL_STORAGE");
  if(isValidDir(internal)){
      root_dir << QString::fromStdString(internal);
  }else{
      //give up
      
  }
  std::vector<std::string> toscan = {
      "/storage/sdcard1",     "/storage/UsbDriveA", "/storage/USBstorage1",
      "/storage/usbdisk",     "/storage/usbotg",    "/storage/UDiskA",
      "/storage/usb-storage", "/storage/usbcard",   "/storage/usb"};
  for (const auto& p : toscan) {
    if (isValidDir(p)) {
        root_dir<< QString::fromStdString(p);
    }
  }
#else
  for (const auto& mnt : QStorageInfo::mountedVolumes()) {
    if (mnt.isValid() && mnt.isReady()) {
      root_dir << mnt.rootPath();
    }
  }
#endif
  return root_dir;
}
