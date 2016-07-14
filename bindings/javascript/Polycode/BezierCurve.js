function BezierCurve() {
	if(arguments[0] != "__skip_ptr__") {
		this.__ptr = Polycode.BezierCurve()
	}
	Object.defineProperties(this, {
		'evaluationAccuracy': { enumerable: true, configurable: true, get: BezierCurve.prototype.__get_evaluationAccuracy, set: BezierCurve.prototype.__set_evaluationAccuracy}
	})
}


BezierCurve.prototype.__get_evaluationAccuracy = function() {
	return Polycode.BezierCurve__get_evaluationAccuracy(this.__ptr)
}

BezierCurve.prototype.__set_evaluationAccuracy = function(val) {
	Polycode.BezierCurve__set_evaluationAccuracy(this.__ptr, val)
}

Duktape.fin(BezierCurve.prototype, function (x) {
	if (x === BezierCurve.prototype) {
		return;
	}
	Polycode.BezierCurve__delete(x.__ptr)
})

BezierCurve.prototype.getNumControlPoints = function() {
	return Polycode.BezierCurve_getNumControlPoints(this.__ptr)
}

BezierCurve.prototype.addControlPoint = function(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z) {
	Polycode.BezierCurve_addControlPoint(this.__ptr, p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z)
}

BezierCurve.prototype.addControlPoint3dWithHandles = function(p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z) {
	Polycode.BezierCurve_addControlPoint3dWithHandles(this.__ptr, p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z)
}

BezierCurve.prototype.addControlPoint3d = function(x,y,z) {
	Polycode.BezierCurve_addControlPoint3d(this.__ptr, x, y, z)
}

BezierCurve.prototype.addControlPoint2dWithHandles = function(p1x,p1y,p2x,p2y,p3x,p3y) {
	Polycode.BezierCurve_addControlPoint2dWithHandles(this.__ptr, p1x, p1y, p2x, p2y, p3x, p3y)
}

BezierCurve.prototype.addControlPoint2d = function(x,y) {
	Polycode.BezierCurve_addControlPoint2d(this.__ptr, x, y)
}

BezierCurve.prototype.getPointAt = function(a) {
	var retVal = new Vector3("__skip_ptr__")
	retVal.__ptr = Polycode.BezierCurve_getPointAt(this.__ptr, a)
	return retVal
}

BezierCurve.prototype.clearControlPoints = function() {
	Polycode.BezierCurve_clearControlPoints(this.__ptr)
}

BezierCurve.prototype.getYValueAtX = function(x) {
	return Polycode.BezierCurve_getYValueAtX(this.__ptr, x)
}

BezierCurve.prototype.getTValueAtX = function(x) {
	return Polycode.BezierCurve_getTValueAtX(this.__ptr, x)
}

BezierCurve.prototype.setHeightCacheResolution = function(resolution) {
	Polycode.BezierCurve_setHeightCacheResolution(this.__ptr, resolution)
}

BezierCurve.prototype.rebuildHeightCache = function() {
	Polycode.BezierCurve_rebuildHeightCache(this.__ptr)
}

BezierCurve.prototype.recalculateDistances = function() {
	Polycode.BezierCurve_recalculateDistances(this.__ptr)
}
