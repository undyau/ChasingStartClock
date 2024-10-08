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

/*!
    \qmltype TextFieldStyle
    \inqmlmodule QtQuick.Controls.Styles 1.0
    \since QtQuick.Controls.Styles 1.0
    \ingroup controlsstyling
    \brief Provides custom styling for TextField.

    Example:
    \qml
    TextField {
        style: TextFieldStyle {
            textColor: "black"
            background: Rectangle {
                radius: 2
                implicitWidth: 100
                implicitHeight: 24
                border.color: "#333"
                border.width: 1
            }
        }
    }
    \endqml
*/

Style {
    id: style

    /*! \internal */
    property var __syspal: SystemPalette {
        colorGroup: control.enabled ?
                        SystemPalette.Active : SystemPalette.Disabled
    }
    /*! The \l TextField attached to this style. */
    readonly property TextField control: __control

    /*! The content margins of the text field. */
    padding { top: 4 ; left: 6 ; right: 6 ; bottom:4 }

    /*! The current font. */
    property font font

    /*! The text color. */
    property color textColor: __syspal.text

    /*! The text highlight color, used behind selections. */
    property color selectionColor: __syspal.highlight

    /*! The highlighted text color, used in selections. */
    property color selectedTextColor: __syspal.highlightedText

    /*! The background of the text field. */
    property Component background: Item {
        implicitWidth: 100
        implicitHeight: 25
        BorderImage {
            anchors.fill: parent
            source: "images/editbox.png"
            border.left: 4
            border.right: 4
            border.top: 4
            border.bottom: 4
            BorderImage {
                anchors.fill: parent
                anchors.margins: -1
                anchors.topMargin: -2
                anchors.rightMargin: 0
                anchors.bottomMargin: 1
                source: "images/focusframe.png"
                visible: control.activeFocus
                border.left: 4
                border.right: 4
                border.top: 4
                border.bottom: 4
            }
        }
    }

    /*! \internal */
    property Component panel: Item {
        anchors.fill: parent

        property int topMargin: padding.top
        property int leftMargin: padding.left
        property int rightMargin: padding.right
        property int bottomMargin: padding.bottom

        property color textColor: style.textColor
        property color selectionColor: style.selectionColor
        property color selectedTextColor: style.selectedTextColor

        implicitWidth: backgroundLoader.implicitWidth ? backgroundLoader.implicitWidth : 100
        implicitHeight: backgroundLoader.implicitHeight ? backgroundLoader.implicitHeight : 20

        property color placeholderTextColor: Qt.rgba(0, 0, 0, 0.5)
        property font font: style.font

        Loader {
            id: backgroundLoader
            sourceComponent: background
            anchors.fill: parent
        }
    }
}
