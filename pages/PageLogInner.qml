import QtQuick 2.12
import QtQuick.Controls 2.8
import QtQuick.Layouts 1.12

Item {
    id: pw
    GroupBox {
        id: box
        anchors.fill: pw
        ColumnLayout {
            spacing: 10
            RowLayout {
                spacing: 10
                Label {
                    text: "Data Log"
                }
                Button {
                    text: "Reset"
                    onClicked: {
                        dialogKonfirmReset.open()
                    }
                }
            }
            TextAreaScroll {
                id: tlog
                readOnly: true
                width: window.width
                height: window.height
                clip: true
            }
        }
    }

    Dialog {
        id: dialogKonfirmReset

        modal: true
        focus: true
        title: "Konfirmasi"
        x: ((window.width - width) / 2) - 10
        y: window.height / 6
        contentWidth: notifLogLayout.width
        contentHeight: notifLogLayout.height
        ColumnLayout {
            id: notifLogLayout
            spacing: 3
            Label {
                text: "Apakah yakin anda ingin\nmelakukan reset informasi log sistem?"
            }
            RowLayout {
                spacing: 3
                Button {
                    text: "Ok"
                    onClicked: {
                        if (folderModel.resetLog()) {
                            tlog.text = ""
                        }
                        dialogKonfirmReset.close()
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: {
                        dialogKonfirmReset.close()
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        tlog.text = folderModel.readLog()
    }
}
