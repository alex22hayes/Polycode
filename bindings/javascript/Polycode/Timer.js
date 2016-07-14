require('Polycode/EventDispatcher')

function Timer(triggerMode,msecs) {
	if(arguments[0] != "__skip_ptr__") {
		this.__ptr = Polycode.Timer(triggerMode,msecs)
	}
}

Timer.EVENT_TRIGGER = 0

Timer.prototype = Object.create(EventDispatcher.prototype)

Duktape.fin(Timer.prototype, function (x) {
	if (x === Timer.prototype) {
		return;
	}
	Polycode.Timer__delete(x.__ptr)
})

Timer.prototype.Pause = function(paused) {
	Polycode.Timer_Pause(this.__ptr, paused)
}

Timer.prototype.isPaused = function() {
	return Polycode.Timer_isPaused(this.__ptr)
}

Timer.prototype.getTicks = function() {
	return Polycode.Timer_getTicks(this.__ptr)
}

Timer.prototype.Update = function(ticks) {
	Polycode.Timer_Update(this.__ptr, ticks)
}

Timer.prototype.Reset = function() {
	Polycode.Timer_Reset(this.__ptr)
}

Timer.prototype.hasElapsed = function() {
	return Polycode.Timer_hasElapsed(this.__ptr)
}

Timer.prototype.getElapsedf = function() {
	return Polycode.Timer_getElapsedf(this.__ptr)
}

Timer.prototype.setTimerInterval = function(msecs) {
	Polycode.Timer_setTimerInterval(this.__ptr, msecs)
}
