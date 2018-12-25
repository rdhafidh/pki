QT += quick
CONFIG += c++17

HEADERS += filesystemabs.h \
    filemodeldata.h \
    pki.h \
    pkisodium.h \
    pkiqml.h \
    util.h boringsslpki.h settinghandler.h \
    openfolderhandler.h mountstoragehandler.h

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SODIUMDIR=
win32-g++{
SODIUMDIR= D:/masteraplikasi/transferh11nov/sodium/mingw
}
android{
SODIUMDIR= D:/masteraplikasi/transferh11nov/sodium/android-armv7-a 
}
INCLUDEPATH += $$SODIUMDIR/include 
LIBS += $$SODIUMDIR/lib/libsodium.a $$SODIUMDIR/lib/libcrypto.a
 
SOURCES += \
        main.cpp  filesystemabs.cpp \
    filemodeldata.cpp \
    pki.cpp \
    pkisodium.cpp \
    pkiqml.cpp z85.c \
    util.cpp boringsslpki.cpp settinghandler.cpp \
    openfolderhandler.cpp  mountstoragehandler.cpp


RESOURCES += pages/DecPage.qml pages/DoDecPage.qml pages/DoEncPage.qml pages/EncPage.qml \
                pages/GroupBoxScroll.qml pages/KeygenGenerate.qml pages/KeygenPage.qml pages/OpenFolder.qml \
                pages/PageLog.qml pages/PageLogInner.qml pages/ScrollablePage.qml pages/TextAreaScroll.qml

RESOURCES += main.qml \
                 $$files(images/*.png)

        
VERSION = 1.0
RC_ICONS =  key.ico 

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \ 
    filesystemabs.h qmlappengine.h filemodeldata.h pkiqml.h z85.h openfolderhandler.h
 

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
