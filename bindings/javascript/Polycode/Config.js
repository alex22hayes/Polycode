function Config() {
	if(arguments[0] != "__skip_ptr__") {
		this.__ptr = Polycode.Config()
	}
}


Duktape.fin(Config.prototype, function (x) {
	if (x === Config.prototype) {
		return;
	}
	Polycode.Config__delete(x.__ptr)
})

Config.prototype.loadConfig = function(configNamespace,fileName) {
	Polycode.Config_loadConfig(this.__ptr, configNamespace, fileName)
}

Config.prototype.saveConfig = function(configNamespace,fileName) {
	Polycode.Config_saveConfig(this.__ptr, configNamespace, fileName)
}

Config.prototype.setStringValue = function(configNamespace,key,value) {
	Polycode.Config_setStringValue(this.__ptr, configNamespace, key, value)
}

Config.prototype.setNumericValue = function(configNamespace,key,value) {
	Polycode.Config_setNumericValue(this.__ptr, configNamespace, key, value)
}

Config.prototype.getNumericValue = function(configNamespace,key) {
	return Polycode.Config_getNumericValue(this.__ptr, configNamespace, key)
}

Config.prototype.getStringValue = function(configNamespace,key) {
	return Polycode.Config_getStringValue(this.__ptr, configNamespace, key)
}

Config.prototype.setBoolValue = function(configNamespace,key,value) {
	Polycode.Config_setBoolValue(this.__ptr, configNamespace, key, value)
}

Config.prototype.getBoolValue = function(configNamespace,key) {
	return Polycode.Config_getBoolValue(this.__ptr, configNamespace, key)
}
