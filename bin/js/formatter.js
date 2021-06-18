var colorFormatter = function (a, b) {
    var r = a.red - b.red
    var g = a.green - b.green
    var b = a.blue - b.blue
    var redMean = (a.red + b.red) / 2
    return (
        (2 + redMean / 256) * r * r +
        4 * g * g +
        (2 + (255 - redMean) / 256) * b * b
    )
}

function hexToRgb(hex_color) {
    var color = new tinycolor(hex_color.hex).toRgb()
    hex_color.red = color.r
    hex_color.green = color.g
    hex_color.blue = color.b
}