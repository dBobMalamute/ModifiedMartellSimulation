import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle
{
    id: rectangle
    height: (360 / idea.numLevels) - 2
    width: 180 + 360.0 * level / idea.numLevels
    y: 40 + (360 / idea.numLevels) * level;
    x: 280 - (180 + 360 * level / idea.numLevels) / 2;
    color: "#3e3124"
    visible: level < idea.numLevels

    property int level: 0
    property int dotSize: 10
    property alias model: gridView.model
    property int numVertical;

    GridView
    {
        id: gridView;
        width: parent.width
        height: parent.height
        x: 0
        y: 0
        cellWidth: 10
        cellHeight: 10
        flow: GridView.TopToBottom
        model: 1
        delegate: Rectangle
        {
            radius: width
            width: dotSize;
            height: width;
            color: model.color;
        }
        onModelChanged:
        {
            connections.target = model;
        }
        onCountChanged:
        {
            resize(count)
        }
        function resize(count)
        {
            let areaPerDot = rectangle.width * rectangle.height / count;
            let tryDotSize = Math.floor(Math.sqrt(areaPerDot));

            numVertical = Math.floor(rectangle.height / tryDotSize);
            let numHorizontal = Math.ceil(count / numVertical);

            while(numHorizontal * tryDotSize > rectangle.width)
            {
                tryDotSize = tryDotSize - 1;
                numVertical = Math.floor(rectangle.height / tryDotSize);
                numHorizontal = Math.ceil(count / numVertical);
            }

            dotSize = tryDotSize;

            let extraVertical = ((rectangle.height - numVertical * dotSize) /(numVertical));
            let extraHorizontal = ((rectangle.width - numHorizontal * dotSize) /(numHorizontal));
            gridView.cellWidth = extraHorizontal + dotSize;
            gridView.cellHeight = extraVertical + dotSize;
            gridView.x = extraHorizontal / 2;
            gridView.y = extraVertical / 2;
        }

    }
    Connections
    {
        id: connections
        target: null
        function onRemovePerson(index, color)
        {
            var comp; var obj;
            comp = Qt.createComponent("RemovePersonAnimationCircle.qml");
            obj = comp.createObject(rectangle, {
                                    width: dotSize, height: dotSize, radius: dotSize,
                                        color: color,
                                        x: gridView.x + gridView.cellWidth * Math.floor(index / numVertical),
                                        y: gridView.y + gridView.cellHeight * (index % numVertical)
                                    })
        }
        function onAddNewHire(index, color)
        {
            var comp; var obj;
            comp = Qt.createComponent("NewHireAnimationCircle.qml");
            obj = comp.createObject(rectangle, {
                                    width: dotSize, height: dotSize, radius: dotSize,
                                        color: color,
                                        x: gridView.x + gridView.cellWidth * Math.floor(index / numVertical),
                                        finalY: gridView.y + gridView.cellHeight * (index % numVertical),
                                        y: gridView.y + gridView.height + 0.5 * dotSize
                                    })

        }
    }
}
