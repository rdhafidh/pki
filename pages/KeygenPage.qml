import QtQuick 2.12
import QtQuick.Controls 2.8
import QtQuick.Layouts 1.12

ScrollablePage {
    id: page 
    SwipeView {
        id: swipe
        currentIndex: 0
        anchors.fill: parent
        interactive: false
        Pane {
			KeygenGenerate{
				anchors.fill:parent
				id: genpage
			}
			Connections{
				target: genpage
				onPrepareFolderOpen: {
                    swipe.currentIndex = 1
					ofolder.setInitPath()
				}
			}
		}
        Pane {
            id: page2
            OpenFolder {
                anchors.fill: parent
                id: ofolder
            }
            Connections {
                target: ofolder
                onPilihFolder: {
                    swipe.currentIndex = 0
                    genpage.setOutputPath(path)
                }
                onCancelPilihFolder: {
                    swipe.currentIndex = 0
                }
            } 
        }

		Component.onCompleted:{
			pki.resetEnkripsiBasedSetting();
		}
    }
}