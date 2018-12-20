import QtQuick 2.12
import QtQuick.Controls 2.8
import QtQuick.Layouts 1.12

ScrollablePage {
    id: page
    property bool openpriv: false
    SwipeView {
        id: swipe
        currentIndex: 0
        anchors.fill: parent
        interactive: false
        Pane {
            DoEncPage {
                anchors.fill: parent
                id: doenc
            }
            Connections {
                target: doenc
                onOpenFilePriv: {
                    openpriv = true
                    doenc.visible = false
                    folderModel.setNameFilterKey();
                    ofolder.setTitleOpenFolder("Pilih file berekstensi *.priv atau *.pub");
                    ofolder.setInitPath()
                    swipe.currentIndex = 1
                }
                onOpenFilePub: {
                    openpriv = false;
                    doenc.visible = false
                    folderModel.setNameFilterKey();
                    ofolder.setTitleOpenFolder("Pilih file berekstensi *.priv atau *.pub");
                    ofolder.setInitPath()
                    swipe.currentIndex = 1
                }
            }
        }
        Pane {
            OpenFolder {
                anchors.fill: parent
                id: ofolder
            }
            Component.onCompleted: {
                ofolder.setModeFolder(false)
            }
            Connections {
                target: ofolder
                onPilihFile: {
                    if (openpriv) {
                        doenc.setPrivateKeyLabel(file)
                    } else {
                        doenc.setPublicKeyLabel(file)
                    }
                    doenc.visible = true
                    swipe.currentIndex = 0
                }
                onCancelPilihFolder: {
                    doenc.visible = true
                    swipe.currentIndex = 0
                }
            }
        }

    }
}