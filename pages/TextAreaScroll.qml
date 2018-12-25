import QtQuick 2.7
import QtQuick.Controls 2.2


Flickable {
    id: flickable
	clip:true
    property alias text: textArea.text 
    property alias readOnly: textArea.readOnly
	function setReadOnly(mode){
		textArea.readOnly=mode;
	}
    contentWidth: textArea.width
    contentHeight: textArea.height
    TextArea.flickable: TextArea {
        id: textArea
        wrapMode: TextArea.Wrap
        persistentSelection: true
        // Different styles have different padding and background
        // decorations, but since this editor is almost taking up the
        // entire window, we don't need them.
        leftPadding: 6
        rightPadding: 6
        topPadding: 3
        bottomPadding: 3 
    } 
    ScrollBar.vertical: ScrollBar {}
	ScrollBar.horizontal: ScrollBar {}
}
