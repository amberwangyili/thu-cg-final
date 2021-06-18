var speedfast = 600 // ms
var speedslow = 2500 // ms
var data
var colorDataset = []
var myNodeTree = new kdTree(colorDataset, colorFormatter, ['black', 'black'])
var rebalancing = false
var myWidth = $('#treeContainer').width()
var myHeight = $('#treeContainer').height()
var linesG, pointsG, bLinesG, bPointsG, diagonal, beginTips, pointRadius
var width, height
var x, y

$(document).ready(function () {
    drawTree()
})

function drawTree() {
    var margin = {
        top: 0,
        right: 0,
        bottom: 0,
        left: 0,
    };
    (width = myWidth - margin.left - margin.right),
        (height = myHeight - margin.top - margin.bottom)
    pointRadius = 6

    x = d3.scale.linear().range([0, width]).domain([0, 255])

    y = d3.scale.linear().range([height, 0]).domain([0, 255])

    beginTips = d3
        .select('#treeContainer')
        .append('p')
        .text('press start bottom first!')
        .attr('id', 'tiptext')
        .attr('opacity', 1)

    var svg = d3
        .select('#treeContainer')
        .append('svg')
        .attr('id', 'treeSvg')
        .attr('class', 'kdtree')
        // .select('#treeSvg')
        .attr('width', width + margin.left + margin.right)
        .attr('height', height + margin.top + margin.bottom)

    var graph = svg
        .append('g')
        .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')')
        .attr('width', width)
        .attr('height', height)

    linesG = graph.append('g').attr('class', 'lines')

    pointsG = graph.append('g').attr('class', 'points')

    var tooltip = d3
        .select('body')
        .append('div')
        .attr('class', 'tooltip')
        .style('position', 'absolute')
        .style('z-index', '10')
        .style('visibility', 'hidden')

    var swatch = tooltip.append('div').attr('class', 'tooltip-swatch')

    var label = tooltip.append('p').text('a simple tooltip')

    diagonal = d3.svg.diagonal().projection(function (d) {
        return [d.y, d.x]
    })

    svg.on('click', function () {
        coordinates = d3.mouse(this)
        var red = x.invert(coordinates[0])
        var green = y.invert(coordinates[1])
        var color = d3.rgb(red, green, 128)
        var object = {
            title: 'user defined',
            hex: color.toString(),
        }
        hexToRgb(object)
        var potentialDuplicates = myNodeTree.nearest(object, 1)
        if (potentialDuplicates.length > 0) {
            var distance = potentialDuplicates[0][1]
            if (distance < 10) return
        }
        colorDataset.push(object)
        myNodeTree.insert(object)
        data = myNodeTree.pointsBFS()
        drawDataSet(data)
    })

    data = myNodeTree.pointsBFS()
    drawDataSet(data)
}

function StartHandle() {
    document.getElementById('buttonStart').disabled = true
    balanceHandle()
}

function balanceHandle() {
    myNodeTree = new kdTree(colorDataset, colorFormatter, ['red', 'green'])
    data = myNodeTree.pointsBFS()
    rebalancing = true
    drawDataSet(data)
}

function clearHandle() {
    document.getElementById('buttonStart').disabled = false
    data = []
    colorDataset = []
    myNodeTree = new kdTree(colorDataset, colorFormatter, ['black', 'black'])
    d3.select('#treeContainer').selectAll('*').remove()
    var p = d3.select('#tiptext')
    var svg = d3.select('#treeSvg')
    p.remove()
    svg.remove()
    drawTree()
}

function updateTree(color) {
    $('#picked').css('background', color.toHex())
    var rgb = color.toRgb()
    var search = {
        red: rgb.r,
        green: rgb.g,
        blue: rgb.b,
    }
    var nearest = myNodeTree.nearest(search, 4)
    nearest.sort(function (a, b) {
        return a[1] - b[1]
    })

    var $list = $('#results ul')
    $list.html('')
    for (var i = 0; i < nearest.length; i++) {
        var c = nearest[i][0]

        var $box = $('<div>')
            .css('background', c.hex)
            .css('display', 'inline-block')
            .css('margin-right', '10px')
            .height('30px')
            .width('30px')
        var $line = $('<li>').append($box).append(c.title)
        $list.append($line)
    }

    updateSearchResults(nearest)
}

var i = 1

function drawIteratively() {
    drawDataSet(data.slice(0, i))
    if (i < data.length) {
        i++
        setTimeout(drawIteratively, 250)
    } else {
        i = 1
    }
}

function finishTransition() {
    rebalancing = false
}

function drawDataSet(dataSubset) {
    if (dataSubset.length > 0) {
        beginTips.style('opacity', 0.0)
    } else {
        beginTips.style('opacity', 1.0)
    }

    // points
    {
        var pointsSelection = pointsG
            .selectAll('.point')
            .data(dataSubset, function (d) {
                return d.hex + 'point'
            })

        pointsSelection
            .enter()
            .append('circle')
            .attr('class', function (d) {
                d.fresh = true
                return 'point'
            })
            .attr('class', 'point')
            .attr('dimension', function (d) {
                return d.dimension
            })
            .attr('r', pointRadius)
            .attr('cx', function (d) {
                return x(d.red)
            })
            .attr('cy', function (d) {
                return y(d.green)
            })
            .style('fill', function (d) {
                // return d3.rgb(d.hex)
                return '#b2b2b2'
            })
            .style('stroke', function (d) {
                return d3.rgb(d.hex)
            })
            .on('mouseover', function (d) {
                if (d.fresh) return
                var parent = d.node
                while (parent) {
                    parent.onAccessPath = true
                    parent = parent.parent
                }
                drawDataSet(data)
            })
            .on('mouseout', function (d) {
                if (d.fresh) {
                    d.fresh = false
                    return
                }
                var parent = d.node
                while (parent) {
                    parent.onAccessPath = false
                    parent = parent.parent
                }
                drawDataSet(data)
            })

        pointsSelection.style('stroke-width', function (d) {
            return d.node.onAccessPath || d.node.isSearchResult ? 2 : 0
        })

        // lines

        var selection = linesG
            .selectAll('.point-line')
            .data(dataSubset, function (d) {
                return d.hex + 'line'
            })

        selection // update
            .style('stroke-width', function (d) {
                return d.node.onAccessPath ? (2 * height) / 400 : height / 400
            })
            .transition()
            .duration(function () {
                return rebalancing ? speedslow : speedfast
            })
            .each('end', finishTransition)
            .attr('x1', function (d) {
                return x(d.x1)
            })
            .attr('y1', function (d) {
                return y(d.y1)
            })
            .attr('x2', function (d) {
                return x(d.x2)
            })
            .attr('y2', function (d) {
                return y(d.y2)
            })
            .attr('stroke', function (d) {
                if (d.dimension === 0) {
                    return 'red'
                }
                if (d.dimension === 1) {
                    return 'green'
                }
            })

        selection
            .enter()
            .append('line')
            .attr('class', 'point-line')
            // .attr("stroke-width", function(d) { return w(d.depth); })
            .attr('stroke-width', height / 400)
            // .style("opacity", 0.8)
            // start all animation from point
            .attr('x1', function (d) {
                return x(d.red)
            })
            .attr('y1', function (d) {
                return y(d.green)
            })
            .attr('x2', function (d) {
                return x(d.red)
            })
            .attr('y2', function (d) {
                return y(d.green)
            })
            .attr('stroke', function (d) {
                if (d.dimension === 0) {
                    return 'red'
                }
                if (d.dimension === 1) {
                    return 'green'
                }
            })
            .transition()
            .attr('x1', function (d) {
                return x(d.x1)
            })
            .attr('y1', function (d) {
                return y(d.y1)
            })
            .attr('x2', function (d) {
                return x(d.x2)
            })
            .attr('y2', function (d) {
                return y(d.y2)
            })

        selection // enter + update
        // .transition()

        selection.exit().remove()
    }
}