/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Private 1.0

ScrollViewStyle {
    id: root

    property var __syspal: SystemPalette {
        colorGroup: control.enabled ?
                        SystemPalette.Active : SystemPalette.Disabled
    }
    readonly property TableView control: __control
    property bool activateItemOnSingleClick: __styleitem.styleHint("activateItemOnSingleClick")
    property color textColor: __styleitem.textColor
    property color backgroundColor: __syspal.base
    property color highlightedTextColor: __styleitem.highlightedTextColor

    property StyleItem __styleitem: StyleItem{
        property color textColor: styleHint("textColor")
        property color highlightedTextColor: styleHint("highlightedTextColor")
        elementType: "item"
        visible: false
        active: control.activeFocus
        onActiveChanged: {
            highlightedTextColor = styleHint("highlightedTextColor")
            textColor = styleHint("textColor")
        }
    }

    property Component headerDelegate: StyleItem {
        elementType: "header"
        activeControl: itemSort
        raised: true
        sunken: styleData.pressed
        text: styleData.value
        hover: styleData.containsMouse
        hints: control.styleHints
        properties: {"headerpos": headerPosition}
        property string itemSort:  (control.sortIndicatorVisible && styleData.column === control.sortIndicatorColumn) ? (control.sortIndicatorOrder == Qt.AscendingOrder ? "up" : "down") : "";
        property string headerPosition: control.columnCount === 1 ? "only" :
                                                          styleData.column === control.columnCount-1 ? "end" :
                                                                                  styleData.column === 0 ? "beginning" : ""
    }

    property Component rowDelegate: StyleItem {
        id: rowstyle
        elementType: "itemrow"
        activeControl: styleData.alternate ? "alternate" : ""
        selected: styleData.selected ? true : false
        height: Math.max(16, rowstyle.implicitHeight)
        active: styleData.hasActiveFocus
    }

    property Component itemDelegate: Item {
        height: Math.max(16, label.implicitHeight)
        property int implicitWidth: sizehint.paintedWidth + 16

        Text {
            id: label
            objectName: "label"
            width: parent.width
            anchors.leftMargin: 8
            font: __styleitem.font
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: styleData.textAlignment
            anchors.verticalCenter: parent.verticalCenter
            elide: styleData.elideMode
            text: styleData.value !== undefined ? styleData.value : ""
            color: styleData.textColor
            renderType: Text.NativeRendering
        }
        Text {
            id: sizehint
            font: label.font
            text: styleData.value ? styleData.value : ""
            visible: false
        }
    }
}

