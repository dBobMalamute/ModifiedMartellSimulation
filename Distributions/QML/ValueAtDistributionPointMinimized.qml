import QtQuick 2.12
import QtQuick.Controls 2.12

import com.distributions.dists 1.0

Rectangle
{
    height: 40
    width: 100
    color: "#5a0000"
    border.width: 3
    radius: 6
    border.color: "black"

    Text
    {
        anchors.centerIn: parent
        font.pointSize: 12
        color: "#cccccc"

        text: idea.value.toPrecision(4);
    }
}



