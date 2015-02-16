var columnSize = 6,
columnPadding = 0,
botPadding = 30,
colors = pv.Colors.category20().range();

var nbFrame = 0;
var maxX = 0;
var maxY = 0;
for(i = 0; i < MeanDiff.length; i++)
{
	nbFrame = MeanDiff[i].Frame > nbFrame ? MeanDiff[i].Frame : nbFrame;
	maxX = MeanDiff[i].Meanx + VarTip[i].Varx > maxX ? MeanDiff[i].Meanx + VarTip[i].Varx : maxX; 
	maxX = MeanDiff[i].Groundx > maxX ? MeanDiff[i].Groundx : maxX; 
	maxY = MeanDiff[i].Meany + VarTip[i].Vary > maxY ? MeanDiff[i].Meany + VarTip[i].Vary : maxY; 
	maxY = MeanDiff[i].Groundy> maxY ? MeanDiff[i].Groundy : maxY; 
}

var vis = new pv.Panel()
	.width((nbFrame*2)*(columnSize+columnPadding))
	.height(Math.max(maxX,maxY))
	.left(10);

var column = vis.add(pv.Panel)
	.data(MeanDiff)
	.left(function() botPadding +  this.index * (columnSize+columnPadding))
	.width(columnSize)
	.height(Math.max(maxX,maxY));

var barX = column.add(pv.Panel)
	// .top(function(a,b) b.Meanx)
	.height(function(a,b) b.Meanx)
	.bottom(botPadding)
	.fillStyle(function() colors[1]);

var varX = vis.add(pv.Panel)
	.data(VarTip)
	.height(function() VarTip[this.index].Varx +4)
	.bottom(function() botPadding + MeanDiff[this.index].Meanx - VarTip[this.index].Varx/2 - 2)
	.fillStyle("rgba(255,0,0,.5)")
	.width(columnSize)
	.left(function() botPadding + this.index * (columnSize + columnPadding));

var groundX = vis.add(pv.Line)
	.data(MeanDiff)
    	.bottom(function() botPadding + MeanDiff[this.index].Groundx)
    	.left(function() botPadding + this.index * (columnSize + columnPadding))
    	.lineWidth(3)
    	.strokeStyle (colors[0]);

var curveKalman = vis.add(pv.Line)
	.data(Kalman)
	.bottom(function() botPadding + Kalman[this.index].Kalmanx)
	.left(function() botPadding + this.index * (columnSize + columnPadding))
	.lineWidth(3)
	.strokeStyle(colors[4])

var errorX = vis.add(pv.Line)
	.data(MeanDiff)
	.bottom(function() botPadding + Math.abs(MeanDiff[this.index].Meanx- MeanDiff[this.index].Groundx))
	.left(function() botPadding + this.index * (columnSize + columnPadding))
	.lineWidth(3)
	.strokeStyle(colors[2]);

var errorKalman = vis.add(pv.Line)
	.data(Kalman)
	.bottom(function() botPadding + Math.abs(Kalman[this.index].Kalmanx - MeanDiff[this.index].Groundx))
	.left(function() botPadding + this.index * (columnSize + columnPadding))
	.lineWidth(3)
	.strokeStyle(colors[4]);

var label = vis.add(pv.Label)
	.data(pv.range(0,nbFrame/5))
	.bottom(botPadding/2)
	.text(function() MeanDiff[this.index*5].Frame)
	.left(function() botPadding + 5*this.index * (columnSize + columnPadding) - (columnSize+ columnPadding) / 2 );

var frameLegend = vis.add(pv.Label)
	.left(botPadding + (nbFrame) * (columnSize + columnPadding) / 2)
	.bottom(0)
	.text("Frame");

var legend = vis.add(pv.Panel)
	.left(botPadding + nbFrame * (columnSize + columnPadding))
	.width((nbFrame)*(columnSize+columnPadding))
	.height(Math.max(maxX,maxY))
	.bottom(botPadding); 

var measureLegend = legend.add(pv.Panel)
	.left(4*columnSize)
	.bottom(4*columnSize)
	.height(2*columnSize)
	.width(2*columnSize)
	.fillStyle(colors[1])
	.add(pv.Label)
		.bottom(0)
		.left(3*columnSize)
		.text("Measure")
	;

var varLegend = legend.add(pv.Panel)
	.left(4*columnSize)
	.bottom(8*columnSize)
	.height(2*columnSize)
	.width(2*columnSize)
	.fillStyle("rgba(255,0,0,.5)")
	.add(pv.Label)
		.bottom(0)
		.left(3*columnSize)
		.text("Variance")
	;

var groundLegend = legend.add(pv.Panel)
	.left(4*columnSize)
	.bottom(12*columnSize+columnSize)
	.height(3)
	.width(2*columnSize)
	.fillStyle(colors[2])
	.add(pv.Label)
		.bottom(-columnSize)
		.left(3*columnSize)
		.text("Error")
	;

var errorLegend = legend.add(pv.Panel)
	.left(4*columnSize)
	.bottom(16*columnSize+columnSize)
	.height(3)
	.width(2*columnSize)
	.fillStyle(colors[0])
	.add(pv.Label)
		.bottom(-columnSize)
		.left(3*columnSize)
		.text("Ground truth")
	;

var errorKalmanLegend = legend.add(pv.Panel)
	.left(4*columnSize)
	.bottom(20*columnSize+columnSize)
	.height(3)
	.width(2*columnSize)
	.fillStyle(colors[4])
	.add(pv.Label)
		.bottom(-columnSize)
		.left(3*columnSize)
		.text("Error Kalman")
	;

// var horLine = row.add(pv.Line)
// 	.data(pv.range(0,nbFrame+2))
// 	.bottom(0)
// 	.left(function() this.index * (columnSize + columnPadding))
// 	.lineWidth(1)
// 	.strokeStyle("rgba(0,0,0,0.1)");

var lines = vis.add(pv.Panel)
	.data(pv.range(0,Math.max(maxX,maxY)/50))
	.left(botPadding)
	.bottom(botPadding);

var line = lines.add(pv.Line)
	.data([0,nbFrame+1])
	.bottom(function(a,b) b*50)
	.left(function(d) (d-1)*(columnSize+columnPadding)+columnSize)
	.strokeStyle(function(a,b) b*50 % 100 == 0 ? "rgba(0,0,0,0.2)" : "rgba(0,0,0,0.066)")
	.add(pv.Label)
		.text(function(a,b) a== 0 ? (b*50%100 == 0 ? b*50 : "") : "")
    		.textAlign("right")
    		.bottom(function(a,b) b*50 - 5);

var pixeLegent = vis.add(pv.Label)
	.left(botPadding/3)
	.bottom(Math.max(maxX,maxY)/2)
	.textAngle(-Math.PI / 2)
	.text("Pixel");

vis.render();
