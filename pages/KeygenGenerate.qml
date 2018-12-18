import QtQuick 2.12
import QtQuick.Controls 2.8
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height
    signal prepareFolderOpen()
    Column {
        spacing: 30
        width: parent.width
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            text: "Key Gen Pair Public dan Private Key Output"
        }
        Flickable {
            id: flickable 
			width: 250; height: 300
			contentWidth: boxutama.width; contentHeight: boxutama.height
            GroupBox {
				id: boxutama
                title: "" 
                GridLayout {
                    columns: 3
                    columnSpacing: 10
                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        text: "Output Public dan Private Key:"
                        font.pixelSize: 10
                    }
                    Rectangle {
                        id: path1
                        width: 150
                        height: 30
                        Label {
                            id: pathlabel
                            anchors.fill: path1
                            anchors.topMargin: 8
                            anchors.rightMargin: 5
                            anchors.leftMargin: 5
                            text: ""
                            elide: Label.ElideLeft
                        }
                        border.color: "black"
                        function setText(msg) {
                            pathlabel.text = msg
                        }
                    }
                    Image {
                        id: btn1
                        source: "qrc:/images/ic_folder_open_black_48dp.png"
                        sourceSize.width: 30
                        sourceSize.height: 30
                        MouseArea {
                            anchors.fill: btn1
                            onClicked: {
                                folderModel.clearNameFilter();
                                prepareFolderOpen()
                            }
                        }
                    }
                    Button {
                        text: "Generate"
                        onClicked: {
                            if (pathlabel.text === "") return

                            if (!pki.genPairKey(pathlabel.text)) {
                                notifDialogk.setMsgLabel("Galat generate pair PKI Key")
                                notifDialogk.open()
                            } else {
                                notifDialogk.setMsgLabel("Generate pair PKI Key berhasil")
                                notifDialogk.open()
                            }
                        }
                    }
                }
            }
			ScrollBar.vertical: ScrollBar {}
			ScrollBar.horizontal: ScrollBar {}
		}

    }


    Dialog {
        id: notifDialogk

        modal: true
        focus: true
        title: "Informasi"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: notifColk.height
        Column {
            id: notifColk
            spacing: 20
            Label {
                id: labelNotifk
                text: ""
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
            Button {
                text: "Tutup"
                onClicked: {
                    notifDialogk.close()
                }
            }
        }

        function setMsgLabel(labelmsg) {
            labelNotifk.text = labelmsg;
        }
    }

    function setOutputPath(path) {
        path1.setText(path)
    }

}