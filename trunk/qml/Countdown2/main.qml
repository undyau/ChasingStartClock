import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.0

Rectangle {
    id: mainRect
    color: "grey"
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Rectangle {
        id: clockRect
        color: "black"
        height: parent.height
        width:  parent.width *.7
        anchors.top: parent.top
        anchors.left: parent.left
        Text {
            id: clock
            text: "09:16:00"
            font.pixelSize: parent.width/4.5
            color: "green"
            anchors.centerIn: parent
        }
    }


    Item {
        id: form
        anchors.verticalCenter:  parent.verticalCenter
        anchors.left: parent.left
        visible: false
        width: Math.min(clockRect.width, 30)
        GridLayout {
            id: grid
            columns: 3
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            Label {
                text: "Delete starts after:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: staleText
                validator: IntValidator {bottom: -360000; top: 0;}
                focus: true
            }
            Label {
                text: "seconds"
                font.pixelSize: 20
                color: "white"
            }
            Label {
                text: "Maximum starters:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: maxStartersText
                validator: IntValidator {bottom: 5; top: 100;}
                focus: true
            }
            Label {
                text: ""
                font.pixelSize: 20
                color: "white"
            }
            Label {
                text: "Adjust start times by:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: adjustStartsHours
                validator: IntValidator {bottom: -2400; top: 2400;}
                focus: true
            }
            Label {
                text: "hours"
                font.pixelSize: 20
                color: "white"
            }
            Label {
                text: "Start file - IOF XML 3.0:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: fileName
                focus: true
            }
            ToolButton {
               id: openButton
               iconSource: "fileopen.png"
               onClicked: {
                   fileDialog.visible = true;
               }
            }
            Label {
                text: ""
                font.pixelSize: 20
                color: "white"
            }
            Button {
               text: "Make it so"
               onClicked: {
                   console.log("Clicked OK");
               }
            }
        }
    }

    Item {
        id: bottomLeft
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: 40
        ToolButton {
           id: configOnButton
           iconSource: "monkey_on_32x32.png"
           onClicked: {
               configOffButton.visible = true;
               form.visible = true;
               clock.visible = false;
               configOnButton.visible = false;
           }
           visible: true;
        }
        ToolButton {
           id: configOffButton
           iconSource: "monkey_off_32x32.png"
           onClicked: {
               clock.visible = true;
               configOnButton.visible = true;
               configOffButton.visible = false;
               form.visible = false;
           }
           visible: false;
        }
    }


    FileDialog {
        id: fileDialog
        title: "Choose the Start List file (in IOF 3.0 XML format)"
        nameFilters: [ "XML files (*.xml)", "All files (*)" ]
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            fileName.text = fileDialog.fileUrl;
        }
        onRejected: {
            console.log("Canceled")
        }
       // Component.onCompleted: visible = true
    }
    ListView {
        id: listview
        width: parent.width *.3; height: parent.height
        anchors.top: parent.top
        anchors.left: clockRect.right

        model: myModel
        delegate: Rectangle {

            //height: parent.height
            height: listview.height / Math.max(listview.count,5)
            width: parent.width
            Item {
               id: entry
               height: parent.height *.6
               width: parent.width
               anchors.top: parent.top
               anchors.left: parent.left
               Rectangle {
                   id: nameRect
                   color: index % 2 ? "MidnightBlue" : "MediumBlue"
                   width: parent.width *.8
                   height: parent.height
                   anchors.top: parent.top
                   anchors.left: parent.left
                   Text {
                       id: name
                       text: modelData.name
                       color: modelData.color
                       font.pixelSize: entry.width *.08
                       verticalAlignment: Text.AlignTop
                   }
               }
               Rectangle {
                   width: parent.width *.2
                   height: parent.height
                   anchors.top: parent.top
                   anchors.right: parent.right
                   anchors.left: nameRect.right
                   color: index % 2 ? "MidnightBlue" : "MediumBlue"
                   Text {
                       id: timeleft
                       text:  modelData.timeleft
                       color: modelData.color
                       font.pixelSize: entry.width *.08
                       verticalAlignment: Text.AlignTop
                   }
               }
            }
            Rectangle {
               color: index % 2 ? "MidnightBlue" : "MediumBlue"
               height: parent.height *.4
               width: parent.width
               anchors.bottom: parent.bottom
               anchors.left: parent.left
               Text {
                   id: detail
                   text:  modelData.oclass + ' ' + modelData.start
                   color: modelData.color
                   font.pixelSize: parent.width *.06
                   verticalAlignment: Text.AlignTop
               }
            }
        }
      }

    Timer {
        interval: 20
        repeat: true
        running: true
        onTriggered: {
            var now = new Date();
            clock.text = Qt.formatTime(now,"hh:mm:ss");
        }
    }
}
