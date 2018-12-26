import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.1
import QtQuick.Window 2.2
import "pages"
ApplicationWindow {
    id: window
    visible: true
    width: (Qt.platform.os == "android" ||
        Qt.platform.os == "ios") ? Screen.width : 600
    height: 480
    title: qsTr("Demo PKI Kriptografi")

    header: ToolBar {
        Material.foreground: "white"

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: stackView.depth > 1 ? "images/back.png" : "images/drawer.png"
                }
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                        listView.currentIndex = -1
                    } else {
                        drawer.open()
                    }
                }
            }

            Label {
                id: titleLabel
                text: listView.currentItem ? listView.currentItem.text : window.title
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Pengaturan Backend"
                        onTriggered: {
                            //adjust based on setting file
                            cipherBackendDialog.adjustCipherDialog(
                                settingApp.getValue(
                                    "backend",
                                    cipherBackendDialog.currentBackend),
                                settingApp.getValue(
                                    "algo",
                                    cipherBackendDialog.boringsslcipher))
                            cipherBackendDialog.open()
                            folderModel.log("pengaturan backend ")
                        }
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: {
                            notifDialog.setMsgLabel(
                                "Implementasi PKI Kriptografi\nDengan antar muka \nQml javascript dan logic c++.\nMetode= Berbagai metode.\nLibrary Backend= \nLibsodium git stable branch dan\nGoogle BoringSSL master branch.")
                            notifDialog.open()
                        }
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: parent.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    qmlengine.hapusCache()
                    listView.currentIndex = index
                    stackView.push(model.source)
                    drawer.close()
                }
            }

            model: ListModel {
                ListElement {
                    title: "Keygen 2 Pair PKI"
                    source: "qrc:/pages/KeygenPage.qml"
                }
                ListElement {
                    title: "Enkripsi Pesan"
                    source: "qrc:/pages/EncPage.qml"
                }
                ListElement {
                    title: "Dekripsi Pesan"
                    source: "qrc:/pages/DecPage.qml"
                }
                ListElement {
                    title: "Informasi Logging Sistem"
                    source: "qrc:/pages/PageLog.qml"
                }
            }

            ScrollIndicator.vertical: ScrollIndicator {}
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Pane {
            id: pane 
            TextAreaScroll{
                id: tex 
                clip: true
                width: window.width/2
                height: 100
                text: "Demo app enkripsi dengan pertukaran kunci X25519  dan berbagai macam metode AEAD Enkripsi & Dekripsi" 
                readOnly: true 
                x:window.width/2-(width/1.9)
            }

            Image {
                id: logo 
                anchors.top: tex.bottom 
                sourceSize.width: window.width/2 
                sourceSize.height: window.height/2
                fillMode: Image.PreserveAspectFit
                source: "images/banner.png"  
                x:window.width/2-(width/1.9)
            }  
        }
    }

    Dialog {
        id: notifDialog

        modal: true
        focus: true
        title: "Informasi"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: notifCol.height
        ColumnLayout {
            id: notifCol
            spacing: 20
            Label {
                id: labelNotif
                text: ""
                wrapMode: Label.Wrap
                font.pixelSize: 12 
                Layout.alignment: Qt.AlignHCenter
            }
            Button {
               text: "Tutup"  
                onClicked: {
                    notifDialog.close()
                }
            }
        }

        function setMsgLabel(labelmsg) {
            labelNotif.text = labelmsg
        }
    }

    Dialog {
        id: cipherBackendDialog

        modal: true
        focus: true
        title: "Cipher Backend"
        x: ((window.width - width) / 2) - 10
        y: window.height / 6
        contentWidth: notifColCipher.width
        contentHeight: notifColCipher.height
        property
        var currentBackend: 0
        property
        var boringsslcipher: 1

        function adjustCipherDialog(backend_idx, cipher_idx) {
            if (backend_idx == 1) {
                pilihBackend.currentIndex = 1
                pilihBackendBoringSSL.visible = true
                if (cipher_idx < 0 || cipher_idx >= 3) {
                    pilihBackendBoringSSL.currentIndex = 1
                } else {
                    pilihBackendBoringSSL.currentIndex = cipher_idx
                }
                cipherBackendDialog.boringsslcipher = cipher_idx
            } else {
                pilihBackendBoringSSL.visible = false
                pilihBackend.currentIndex = 0
            }
            cipherBackendDialog.currentBackend = backend_idx
        }
        ColumnLayout {
            id: notifColCipher
            spacing: 20
            ComboBox {
                id: pilihBackend
                Layout.fillWidth: true
                model: ["Libsodium(xsalsa20 poly1305)", "Boringssl"]
                onActivated: {
                    if (index == 1) {
                        pilihBackendBoringSSL.visible = true
                    } else {
                        pilihBackendBoringSSL.visible = false
                    }
                }
            }
            ComboBox {
                id: pilihBackendBoringSSL
                Layout.fillWidth: true
                visible: false
                model: ["aes 256 gcm", "xchacha20 poly1305", "aes 256 gcm siv"]
            }

            RowLayout {
                spacing: 4
                Button {
                    text: "Simpan"
                    onClicked: {
                        cipherBackendDialog.currentBackend = pilihBackend.currentIndex
                        if (cipherBackendDialog.currentBackend == 1) {
                            cipherBackendDialog.boringsslcipher = pilihBackendBoringSSL.currentIndex
                        }
                        settingApp.setValue("backend",
                            cipherBackendDialog.currentBackend)
                        settingApp.setValue("algo",
                            cipherBackendDialog.boringsslcipher)
                        if (stackView.depth == 2) {
                            stackView.pop()
                            titleLabel.text = window.title
                        }
                        cipherBackendDialog.close()
                    }
                }

                Button {
                    text: "cancel"
                    onClicked: {
                        cipherBackendDialog.close()
                    }
                }
            }
        }

        function getCurrentBackend() {
            return cipherBackendDialog.currentBackend
        }

        function getCurrentBoringsslCipher() {
            return cipherBackendDialog.boringsslcipher
        }
    }
}
