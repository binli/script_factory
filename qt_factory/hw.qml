import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.3

Rectangle {
	width: 320
	height: 240
	color: "gray"

	Text {
		anchors.centerIn: parent
		text: "hello bin"
		color: "blue"
		font.pixelSize: 32
	}

	Button {
		id: open
		text: "Open"
		anchors.top: parent.top
		anchors.topMargin: 10
		anchors.left: parent.left
		anchors.leftMargin: 10
	}
	
	Button {
		id: quit
		text: "Quit"
		anchors.top: parent.top
		anchors.topMargin: 10
		anchors.left: open.right
		anchors.leftMargin: 10
		anchors.bottom: open.bottom

		style: ButtonStyle {
			background: Rectangle {
				implicitWidth: 70
				implicitHeight: 25
				border.width: control.activeFocus ? 2 : 1
			}
		}
		z : 1
	}
}
