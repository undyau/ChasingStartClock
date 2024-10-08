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

/*!
        \qmltype TabBar
        \internal
        \inqmlmodule QtQuick.Controls.Private 1.0
*/
FocusScope {
    id: tabbar
    height: Math.max(tabrow.height, Math.max(leftCorner.height, rightCorner.height))
    width: tabView.width

    activeFocusOnTab: true

    Keys.onRightPressed: {
        if (tabView && tabView.currentIndex < tabView.count - 1)
            tabView.currentIndex = tabView.currentIndex + 1
    }
    Keys.onLeftPressed: {
        if (tabView && tabView.currentIndex > 0)
            tabView.currentIndex = tabView.currentIndex - 1
    }

    onTabViewChanged: parent = tabView
    visible: tabView ? tabView.tabsVisible : true

    property var tabView
    property var style
    property var styleItem: tabView.__styleItem ? tabView.__styleItem : null

    property bool tabsMovable: styleItem ? styleItem.tabsMovable : false

    property int tabsAlignment: styleItem ? styleItem.tabsAlignment : Qt.AlignLeft

    property int tabOverlap: styleItem ? styleItem.tabOverlap : 0

    property int elide: Text.ElideRight

    property real availableWidth: tabbar.width - leftCorner.width - rightCorner.width

    property var __selectedTabRect

    function tab(index) {
        for (var i = 0; i < tabrow.children.length; ++i) {
            if (tabrow.children[i].tabindex == index) {
                return tabrow.children[i]
            }
        }
        return null;
    }

    /*! \internal */
    function __isAncestorOf(item, child) {
        //TODO: maybe removed from 5.2 if the function was merged in qtdeclarative
        if (child === item)
            return false;

        while (child) {
            child = child.parent;
            if (child === item)
                return true;
        }
        return false;
    }

    ListView {
        id: tabrow
        objectName: "tabrow"
        Accessible.role: Accessible.PageTabList
        spacing: -tabOverlap
        orientation: Qt.Horizontal
        interactive: false
        focus: true

        width: Math.min(availableWidth, count ? contentWidth : availableWidth)
        height: currentItem ? currentItem.height : 0

        highlightMoveDuration: 0
        currentIndex: tabView.currentIndex
        onCurrentIndexChanged: tabrow.positionViewAtIndex(currentIndex, ListView.Contain)

        moveDisplaced: Transition {
            NumberAnimation {
                property: "x"
                duration: 125
                easing.type: Easing.OutQuad
            }
        }

        states: [
            State {
                name: "left"
                when: tabsAlignment === Qt.AlignLeft
                AnchorChanges { target:tabrow ; anchors.left: parent.left }
                PropertyChanges { target:tabrow ; anchors.leftMargin: leftCorner.width }
            },
            State {
                name: "center"
                when: tabsAlignment === Qt.AlignHCenter
                AnchorChanges { target:tabrow ; anchors.horizontalCenter: tabbar.horizontalCenter }
            },
            State {
                name: "right"
                when: tabsAlignment === Qt.AlignRight
                AnchorChanges { target:tabrow ; anchors.right: parent.right }
                PropertyChanges { target:tabrow ; anchors.rightMargin: rightCorner.width }
            }
        ]

        model: tabView.__tabs

        delegate: MouseArea {
            id: tabitem
            objectName: "mousearea"
            hoverEnabled: true
            focus: true

            Binding {
                target: tabbar
                when: selected
                property: "__selectedTabRect"
                value: Qt.rect(x, y, width, height)
            }

            drag.target: tabsMovable ? tabloader : null
            drag.axis: Drag.XAxis
            drag.minimumX: drag.active ? 0 : -Number.MAX_VALUE
            drag.maximumX: tabrow.width - tabitem.width

            property int tabindex: index
            property bool selected : tabView.currentIndex === index
            property string title: modelData.title
            property bool nextSelected: tabView.currentIndex === index + 1
            property bool previousSelected: tabView.currentIndex === index - 1

            z: selected ? 1 : -index
            implicitWidth: tabloader.implicitWidth
            implicitHeight: tabloader.implicitHeight

            onPressed: {
                tabView.currentIndex = index;
                var next = tabbar.nextItemInFocusChain(true);
                if (__isAncestorOf(tabView.getTab(currentIndex), next))
                    next.forceActiveFocus();
            }

            Loader {
                id: tabloader

                property Item control: tabView
                property int index: tabindex

                property QtObject styleData: QtObject {
                    readonly property alias index: tabitem.tabindex
                    readonly property alias selected: tabitem.selected
                    readonly property alias title: tabitem.title
                    readonly property alias nextSelected: tabitem.nextSelected
                    readonly property alias previsousSelected: tabitem.previousSelected
                    readonly property alias hovered: tabitem.containsMouse
                    readonly property bool activeFocus: tabbar.activeFocus
                    readonly property real availableWidth: tabbar.availableWidth
                }

                sourceComponent: loader.item ? loader.item.tab : null

                Drag.keys: "application/x-tabbartab"
                Drag.active: tabitem.drag.active
                Drag.source: tabitem

                property real __prevX: 0
                property real __dragX: 0
                onXChanged: {
                    if (Drag.active) {
                        // keep track for the snap back animation
                        __dragX = tabitem.mapFromItem(tabrow, tabloader.x, 0).x

                        // when moving to the left, the hot spot is the left edge and vice versa
                        Drag.hotSpot.x = x < __prevX ? 0 : width
                        __prevX = x
                    }
                }

                width: tabitem.width
                state: Drag.active ? "drag" : ""

                transitions: [
                    Transition {
                        to: "drag"
                        PropertyAction { target: tabloader; property: "parent"; value: tabrow }
                    },
                    Transition {
                        from: "drag"
                        SequentialAnimation {
                            PropertyAction { target: tabloader; property: "parent"; value: tabitem }
                            NumberAnimation {
                                target: tabloader
                                duration: 50
                                easing.type: Easing.OutQuad
                                property: "x"
                                from: tabloader.__dragX
                                to: 0
                            }
                        }
                    }
                ]
            }

            Accessible.role: Accessible.PageTab
            Accessible.name: modelData.title
        }
    }

    Loader {
        id: leftCorner
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        sourceComponent: styleItem ? styleItem.leftCorner : undefined
        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0
    }

    Loader {
        id: rightCorner
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        sourceComponent: styleItem ? styleItem.rightCorner : undefined
        width: item ? item.implicitWidth : 0
        height: item ? item.implicitHeight : 0
    }

    DropArea {
        anchors.fill: tabrow
        keys: "application/x-tabbartab"
        onPositionChanged: {
            var source = drag.source
            var target = tabrow.itemAt(drag.x, drag.y)
            if (source && target && source !== target) {
                source = source.drag.target
                target = target.drag.target
                var center = target.parent.x + target.width / 2
                if ((source.index > target.index && source.x < center)
                        || (source.index < target.index && source.x + source.width > center))
                    tabView.moveTab(source.index, target.index)
            }
        }
    }
}
