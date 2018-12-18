import QtQuick 2.12
import QtQuick.Controls 2.8
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height
    signal pilihFolder(string path)
    signal cancelPilihFolder()
    signal pilihFile(string file)
    property alias folderurl: folderpath.text
    property bool isModeFolder: true
    Flickable {
		x:20
        id: flickable
        width: 250;height: window.height
        contentWidth: layoutfolder.width;contentHeight: layoutfolder.height

        ColumnLayout {
			id: layoutfolder
            width: window.width
            height: window.height
            Label {
                id: headlist
                font.pixelSize: 15
                text: "Folder List"
            }
            Row {
                spacing: 20
                Label {
                    id: folderpath
                    text: folderModel.path()
                    width: 150
                    elide: Label.ElideRight
                }
                Image {
                    y: -10
                    id: btnaction
                    sourceSize.width: 30
                    sourceSize.height: 30
                    source: "qrc:/images/ic_list_black_48dp.png"
                    MouseArea {
                        anchors.fill: btnaction
                        onClicked: {
                            aksiFolder.open()
                        }
                    }
                }
                Image {
                    id: kotak
                    y: -10
                    source: "qrc:/images/ic_subdirectory_arrow_left_black_48dp.png"
                    sourceSize.width: 30
                    sourceSize.height: 30
                    MouseArea {
                        anchors.fill: kotak
                        onClicked: {
                            docdFolder("..")
                        }
                    }
                }
            }
            ListView {
                id: list
                model: folderModel
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                delegate: folderDelegate
                focus: true
                highlightRangeMode: ListView.NoHighlightRange
                snapMode: ListView.SnapToItem
            }
        } 
		
		ScrollBar.vertical: ScrollBar {}
		ScrollBar.horizontal: ScrollBar {}
	}

    Component {
        id: folderDelegate
        Item {
            width: parent.width
            height: 40
            Row {
                spacing: 6
                Image {
                    y: -5
                    source: isdirf ? "qrc:/images/ic_folder_open_black_48dp.png" : "qrc:/images/ic_insert_drive_file_black_48dp.png"
                    sourceSize.width: 30
                    sourceSize.height: 30
                }
                Text {
                    text: namef
                    width: 100
                    elide: Text.ElideRight
                }
                Text {
                    text: ltimef
                }
                Text {
                    text: sizef
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (isdirf) {
                        docdFolder(namef)
                    } else {
                        pilihFile(folderModel.path() + "/" + namef)
                    }
                }
            }
        }
    }

    function setInitPath() {
        if (folderModel.setPath(folderModel.documentPath())) {
            folderpath.text = folderModel.path()
        }
    }

    function setTitleOpenFolder(msg) {
        headlist.text = msg;
    }

    Dialog {
        id: aksiFolder
        modal: true
        focus: true
        title: "Aksi Folder"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20
            Button {
                width: aksiFolder.availableWidth
                text: "Refresh folder"
                onClicked: {
                    folderModel.refresh()
                    aksiFolder.close()
                }
            }
            Button {
                width: aksiFolder.availableWidth
                text: "Buat folder baru"
                onClicked: {
                    aksiFolder.close()
                    mkdirDialog.open()
                }
            }
            Button {
                width: aksiFolder.availableWidth
                visible: isModeFolder == true
                text: "Pilih folder ini"
                onClicked: {
                    pilihFolder(folderModel.path())
                }
            }
            Button {
                width: aksiFolder.availableWidth
                text: "Cancel"
                onClicked: {
                    cancelPilihFolder()
                }
            }

        }
    }
    Dialog {
        id: mkdirDialog

        modal: true
        focus: true
        title: "Buat Folder Baru Dialog"
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: mkdirCol.height
        Column {
            id: mkdirCol
            spacing: 20
            Label {
                text: "Nama Folder"
            }
            TextField {
                id: namaFolderBaru
            }
            Button {
                text: "Buat"
                onClicked: {
                    if (!folderModel.mkdir(namaFolderBaru.text)) {
                        mkdirDialog.close()
                        notifDialog.setMsgLabel("Galat membuat folder")
                        notifDialog.open()
                        return
                    }
                    mkdirDialog.close()
                    refresh()
                }
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


    function docdFolder(path) {
        if (folderModel.cd(path)) {
            folderurl = folderModel.path()
        }
    }

    function setKeyFilter(mode) {
        if (mode) {
            folderModel.setNameFilterKey();
        } else {
            folderModel.clearNameFilter();
        }
    }

    function refresh() {
        folderModel.refresh()
    }

    function setModeFolder(mode) {
        isModeFolder = mode;
    }

}