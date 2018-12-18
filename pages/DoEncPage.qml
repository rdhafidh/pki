import QtQuick 2.12
import QtQuick.Controls 2.8
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height
    signal openFilePriv()
    signal openFilePub()

    Flickable {
        id: flickable
        width: 250;height: window.height
        contentWidth: boxutama.width;contentHeight: boxutama.height 
        GroupBox {
            id: boxutama
            title: ""
            GridLayout {
                columns: 3
                width: parent.width
                anchors.fill: parent
                Label {
                    text: "Input text:"
                }
                TextAreaScroll {
                    id: inbuf
                    height: 60
                    width: 180
                }

                Button {
                    text: "Paste Clipboard"
                    onClicked: {
                        inbuf.text = pki.pasteClipboard();
                    }
                }
                Label {
                    text: "Input Private Key:"
                }
                Rectangle {
                    id: in_priv
                    width: 150
                    height: 30
                    border.color: "black"
                    Label {
                        id: inpriv_label
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.topMargin: 8
                        text: ""
                        anchors.rightMargin: 5
                        elide: Label.ElideLeft
                    }

                    function setLabel(msg) {
                        inpriv_label.text = msg
                    }
                }
                Image {
                    id: inpriv_open
                    sourceSize.width: 30
                    sourceSize.height: 30
                    source: "qrc:/images/ic_folder_open_black_48dp.png"
                    MouseArea {
                        anchors.fill: inpriv_open
                        onClicked: {
                            openFilePriv()
                        }
                    }
                }
                Label {
                    text: "Input Public Key:"
                }
                Rectangle {
                    id: in_pub
                    width: 150
                    height: 30
                    border.color: "black"
                    Label {
                        id: inpub_label
                        anchors.fill: in_pub
                        anchors.topMargin: 8
                        text: ""
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        elide: Label.ElideLeft
                    }

                    function setLabel(msg) {
                        inpub_label.text = msg
                    }
                }
                Image {
                    id: inpub_open
                    sourceSize.width: 30
                    sourceSize.height: 30
                    source: "qrc:/images/ic_folder_open_black_48dp.png"
                    MouseArea {
                        anchors.fill: inpub_open
                        onClicked: {
                            openFilePub()
                        }
                    }
                }
                Button {
                    text: "Enkripsi"
                    enabled: inbuf.text != "" && inpub_label.text != "" && inpriv_label.text != ""
                    onClicked: {
                        if (!pki.encrypt(inbuf.text, inpub_label.text, inpriv_label.text)) {
                            notifDialog.setMsgLabel("Gagal enkripsi teks")
                        } else {
                            notifDialog.setMsgLabel("Enkripsi teks berhasil")
                        }
                        notifDialog.open()
                    }
                }
                Button {
                    text: "Reset Input"
                    enabled: inpub_label.text != "" || inpriv_label.text != "" || inbuf.text != ""
                    onClicked: {
                        inpub_label.text = "";
                        inpriv_label.text = "";
                        inbuf.text = "";
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                }
                Label {
                    text: "Output Ciphertext:"
                }
                TextAreaScroll {
                    id: out_c
                    height: 60
                    width: 180
                }
                Rectangle {
                    Layout.fillWidth: true
                }
                Button {
                    text: "Copy Clipboard"
                    enabled: out_c.text != ""
                    onClicked: {
                        pki.copyClipboard(out_c.text)
                    }
                }
                Connections {
                    target: pki
                    onEncryptSuccess: {
                        out_c.text = ct;
                    }
                }
            }
        }
    }

    function setPrivateKeyLabel(file) {
        in_priv.setLabel(file)
    }

    function setPublicKeyLabel(file) {
        in_pub.setLabel(file)
    }

    Dialog {
        id: notifDialog

        modal: true
        focus: true
        x: (window.width - width) / 2
        title: "Informasi"
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: notifCol.height
        Column {
            id: notifCol
            spacing: 20
            Label {
                id: labelNotif
                text: ""
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
            Button {
                text: "Tutup"
                onClicked: {
                    notifDialog.close()
                }
            }
        }

        function setMsgLabel(labelmsg) {
            labelNotif.text = labelmsg;
        }
    }

}