import QtQuick

Canvas {
    id: grid
    anchors.fill: parent
    opacity: 0.15 // Делаем сетку неяркой, чтобы она не отвлекала от игры

    onPaint: {
        var ctx = getContext("2d");
        ctx.strokeStyle = "#ffffff"; // Белый цвет линий
        ctx.lineWidth = 1;

        // Рисуем вертикальные полосы через каждые 50 пикселей
        for (var x = 50; x < width; x += 50) {
            ctx.beginPath();
            ctx.moveTo(x, 0);
            ctx.lineTo(x, height);
            ctx.stroke();
        }

        // Рисуем горизонтальные полосы через каждые 50 пикселей
        for (var y = 50; y < height; y += 50) {
            ctx.beginPath();
            ctx.moveTo(0, y);
            ctx.lineTo(width, y);
            ctx.stroke();
        }
    }
}
