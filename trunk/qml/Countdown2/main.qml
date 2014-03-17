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
                text: myConfig.stale
            }
            Label {
                text: "seconds"
                font.pixelSize: 20
                color: "white"
            }
            Label {
                text: "Maximum starters shown:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: maxStartersText
                validator: IntValidator {bottom: 5; top: 100;}
                focus: true
                text: myConfig.maxDisplay
            }
            Label {
                text: ""
                font.pixelSize: 20
                color: "white"
            }
            Label {
                text: "Look ahead by:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: lookAheadText
                validator: IntValidator {bottom: 5; top: 10000;}
                focus: true
                text: myConfig.lookAhead
            }
            Label {
                text: "minutes"
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
                text: myConfig.file
            }
            ToolButton {
               id: openButton
               iconSource: "qrc:/icons/fileopen.png"
               onClicked: {
                   fileDialog.visible = true;
               }
            }
            Label {
                text: "(Enter \"FAKE\" as file name to simulate start list)"
                font.pixelSize: 12
                color: "white"
            }
            Label {
                text: ""
                font.pixelSize: 12
                color: "white"
            }
            Label {
                text: ""
                font.pixelSize: 12
                color: "white"
            }
            Label {
                text: "Sound file to play at zero:"
                font.pixelSize: 20
                color: "white"
            }
            TextField {
                id: startSoundFileName
                focus: true
                text: myConfig.startSoundFile
            }
            ToolButton {
               id: openStartSoundFileButton
               iconSource: "qrc:/icons/fileopen.png"
               onClicked: {
                   startSoundFileDialog.visible = true;
               }
            }
            Label {
                text: "(Leave blank for no sound at zero)"
                font.pixelSize: 12
                color: "white"
            }
            Button {
               text: "Apply"
               onClicked: {                   
                   myConfig.file = fileName.text;
                   myConfig.maxDisplay = maxStartersText.text;
                   myConfig.stale = staleText.text;
                   myConfig.lookAhead = lookAheadText.text;
                   myConfig.startSoundFile = startSoundFileName.text;
                   hideConfig();                   
               }
            }
            Label {
                text: myAlert.message
                font.pixelSize: 12
                color: "red"
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
           iconSource: "qrc:/icons/monkey_on_32x32.png"
           onClicked: {
               showConfig();
           }
           visible: true;
        }
        ToolButton {
           id: configOffButton
           iconSource: "qrc:/icons/monkey_off_32x32.png"
           onClicked: {
               hideConfig();
           }
           visible: false;
        }
    }


    FileDialog {
        id: fileDialog
        title: "Choose the Start List file (in IOF 3.0 XML format)"
        nameFilters: [ "XML files (*.xml)", "All files (*)" ]
        onAccepted: {
            myConfig.fileUrl = fileDialog.fileUrl;
        }
    }
    FileDialog {
        id: startSoundFileDialog
        title: "Choose the sound file to play"
        nameFilters: [ "WAV files (*.wav)","All files (*)" ]
        onAccepted: {
           // startSoundFileName = startSoundFileDialog.fileUrl.toLocalFile;
            myConfig.startSoundFile = startSoundFileDialog.fileUrl.toLocalFile;
        }
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
                       font.pixelSize: Math.min(entry.width *.08, entry.height *.8)
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
                       font.pixelSize: Math.min(entry.width *.08, entry.height *.8)
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
                   font.pixelSize: Math.min(entry.width *.06, entry.height *.6)
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
    function hideConfig()
    {
        clockRect.visible = true;
        listview.visible = true;
        configOnButton.visible = true;
        configOffButton.visible = false;
        form.visible = false;
    }
    function showConfig()
    {
        configOffButton.visible = true;
        form.visible = true;
        clockRect.visible = false;
        listview.visible = false;
        configOnButton.visible = false;
    }

}
