import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle
{
    id: rectangle
    height: 40
    color: "#7a8481"

    property int gridY: 0
    property int gridX: 0
    property int gridHeight: 0;
    property int gridWidth: 0;
    property int cellSize: 20
    property alias model: gridView.model
    property int level: 0;

    GridView
    {
        id: gridView;
        y: gridY
        x: gridX
        width: gridWidth
        height: gridHeight;
        cellWidth: cellSize
        cellHeight: cellSize
        delegate: Rectangle
        {
            x: 1
            y: 1
            width: cellSize - 2;
            height: cellSize - 2;
            color: getColorForGroup(model.type);// model.type === 0 ? "black" : model.type === 1 ? "#1212cd" : "#6f3c60"
        }
        onModelChanged:
        {
            connection.target = model;
        }
    }

    function getColorForGroup(groupID)
    {
        switch(groupID)
        {
        case 0:
            return "black"
        case 1:
            return "#1212cd"
        case 2:
            return "#6f3c60"
        case 3:
            return "#307baf"
        case 4:
            return "#a170ea"
        }
    }

    Connections
    {
        id: connection
        function onRemovePerson(index, gender)
        {
            var comp; var obj;
            comp = Qt.createComponent("RemovePersonAnimationSquare.qml");
            obj = comp.createObject(root);
            obj.width = cellSize - 2;
            obj.height = cellSize - 6;
            obj.x = rectangle.x + gridX + (index % Math.floor((gridWidth/cellSize))) * cellSize
            obj.y = 24 + rectangle.y + gridY + cellSize * Math.floor(index / Math.floor(gridWidth / cellSize));
            obj.color = getColorForGroup(gender);
            obj.finalX = 730;
            obj.finalY = 115;
            obj.startAnimating();
        }
        function onPromotePerson(oldIndex, newIndex, gender)
        {
            var comp; var obj;
            comp = Qt.createComponent("PromotePersonAnimationSquare.qml");
            obj = comp.createObject(root);
            obj.height = oldSize(level) - 2
            obj.width = obj.height - 4;
            obj.color = getColorForGroup(gender);
            obj.x = oldXValue(oldIndex, level) + 2;
            obj.y = oldYValue(oldIndex, level);
            obj.finalX = rectangle.x + gridX + (newIndex % Math.floor((gridWidth/cellSize))) * cellSize
            obj.finalY = 24 + rectangle.y + gridY + cellSize * Math.floor(newIndex / Math.floor(gridWidth / cellSize));
            obj.startAnimating();
        }
        function onNewHire(index, gender)
        {
            var comp; var obj;
            comp = Qt.createComponent("NewHireAnimationSquare.qml");
            obj = comp.createObject(root);
            obj.width = cellSize - 2;
            obj.height = cellSize - 2;
            obj.color = getColorForGroup(gender);
            obj.x = 60;
            obj.y = 115
            obj.finalX = rectangle.x + gridX + (index % (gridWidth/cellSize)) * cellSize
            obj.finalY = 24 + rectangle.y + gridY + cellSize * Math.floor(index / (gridWidth / cellSize));
            obj.startAnimating();
        }

        function oldXValue(index, level)
        {
            var xStarts = [9, 57, 109, 133, 187, 245, 268, 309];
            var gridOverCellSizes = [100, 88, 60, 50, 34, 25, 20, 10];
            var cellSizes = [8, 8, 10, 11, 13, 13, 14, 20];
            return xStarts[level - 2] + (index) % gridOverCellSizes[level-2] * cellSizes[level - 2];
        }
        function oldYValue(index, level)
        {
            var yStarts = [333, 293, 245, 204, 157, 113, 76, 34];
            var gridOverCellSizes = [100, 88, 60, 50, 34, 25, 20, 10];
            var cellSizes = [8, 8, 10, 11, 13, 13, 14, 20];
            return yStarts[level - 2] + cellSizes[level-2] * Math.floor((index) / gridOverCellSizes[level - 2]);
        }
        function oldSize(level)
        {
            var cellSizes = [8, 8, 10, 11, 13, 13, 14, 20];
            return cellSizes[level - 2];
        }
    }
}
