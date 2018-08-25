/*
 * Copyright 2013-2016 Canonical Ltd.
 *
 * This file is part of webbrowser-app.
 *
 * webbrowser-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * webbrowser-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

ModalDialog {
    id: colorDialog
    objectName: "colorDialog"
    title: i18n.tr("Select Color")
    
    property string defaultValue
    property int countColors
    
    signal accept(string color)
    signal reject()
    
    onAccept: hide()
    onReject: hide()

    TextField {
        id: input
        objectName: "inputTextField"
        text: defaultValue
        onAccepted: accept(input.text)
    }
    
    Text {
        width: parent.width
        text: "type color names like <a href=\"skyblue\">skyblue</a> or hex codes like <a href=\"#dd6622\">#ddee22</a> or click the colors."
        onLinkActivated: {input.text = link}
        wrapMode: Text.WordWrap
    }
    
    Row {
    Repeater {
     model: ["green", "yellow", "blue", "red", "grey"]
           Rectangle {
             width: units.gu(6)
             height: units.gu(6)
             border.width: 1
             color: modelData

            MouseArea {
                anchors.fill: parent
            onClicked: {
                input.text = modelData
            }
            }
           }
     }
    }    
    
    Button {
        text: i18n.tr("OK")
        color: theme.palette.normal.positive
        objectName: "okButton"
        onClicked: accept(input.text)
    }

    Button {
        objectName: "cancelButton"
        text: i18n.tr("Cancel")
        onClicked: reject()
    }

    /*
    Binding {
        target: model
        property: "currentValue"
        value: input.text
    }
    */
}