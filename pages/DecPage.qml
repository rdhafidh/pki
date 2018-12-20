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
            DoDecPage {
                anchors.fill: parent
                id: dodec
            }
            Connections {
                target: dodec
                onOpenFilePriv: {
                    openpriv = true
                    dodec.visible = false;
                    folderModel.setNameFilterKey();
                    ofolder.setTitleOpenFolder("Pilih file berekstensi *.priv atau *.pub");
                    ofolder.setInitPath()
                    swipe.currentIndex = 1
                }
                onOpenFilePub: {
                    openpriv = false;
                    dodec.visible = false;
                    ofolder.setTitleOpenFolder("Pilih file berekstensi *.priv atau *.pub");
                    folderModel.setNameFilterKey();
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
                        dodec.setPrivateKeyLabel(file)
                    } else {
                        dodec.setPublicKeyLabel(file)
                    }
                    swipe.currentIndex = 0
                    dodec.visible = true;
                }
                onCancelPilihFolder: {
                    swipe.currentIndex = 0
                    dodec.visible = true;
                }
            }
        }

    }
}