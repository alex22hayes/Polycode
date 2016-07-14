class "LightInfo"


function LightInfo:__getvar(name)
	if name == "importance" then
		return Polycode.LightInfo_get_importance(self.__ptr)
	elseif name == "position" then
		local retVal = Polycode.LightInfo_get_position(self.__ptr)
		if retVal == nil then return nil end
		local __c = _G["Vector3"]("__skip_ptr__")
		__c.__ptr = retVal
		return __c
	elseif name == "direction" then
		local retVal = Polycode.LightInfo_get_direction(self.__ptr)
		if retVal == nil then return nil end
		local __c = _G["Vector3"]("__skip_ptr__")
		__c.__ptr = retVal
		return __c
	elseif name == "type" then
		return Polycode.LightInfo_get_type(self.__ptr)
	elseif name == "diffuseColor" then
		local retVal = Polycode.LightInfo_get_diffuseColor(self.__ptr)
		if retVal == nil then return nil end
		local __c = _G["Color"]("__skip_ptr__")
		__c.__ptr = retVal
		return __c
	elseif name == "specularColor" then
		local retVal = Polycode.LightInfo_get_specularColor(self.__ptr)
		if retVal == nil then return nil end
		local __c = _G["Color"]("__skip_ptr__")
		__c.__ptr = retVal
		return __c
	elseif name == "constantAttenuation" then
		return Polycode.LightInfo_get_constantAttenuation(self.__ptr)
	elseif name == "linearAttenuation" then
		return Polycode.LightInfo_get_linearAttenuation(self.__ptr)
	elseif name == "quadraticAttenuation" then
		return Polycode.LightInfo_get_quadraticAttenuation(self.__ptr)
	elseif name == "intensity" then
		return Polycode.LightInfo_get_intensity(self.__ptr)
	elseif name == "spotlightCutoff" then
		return Polycode.LightInfo_get_spotlightCutoff(self.__ptr)
	elseif name == "spotlightExponent" then
		return Polycode.LightInfo_get_spotlightExponent(self.__ptr)
	elseif name == "shadowsEnabled" then
		return Polycode.LightInfo_get_shadowsEnabled(self.__ptr)
	elseif name == "lightViewMatrix" then
		local retVal = Polycode.LightInfo_get_lightViewMatrix(self.__ptr)
		if retVal == nil then return nil end
		local __c = _G["Matrix4"]("__skip_ptr__")
		__c.__ptr = retVal
		return __c
	end
end

function LightInfo:__setvar(name,value)
	if name == "importance" then
		Polycode.LightInfo_set_importance(self.__ptr, value)
		return true
	elseif name == "position" then
		Polycode.LightInfo_set_position(self.__ptr, value.__ptr)
		return true
	elseif name == "direction" then
		Polycode.LightInfo_set_direction(self.__ptr, value.__ptr)
		return true
	elseif name == "type" then
		Polycode.LightInfo_set_type(self.__ptr, value)
		return true
	elseif name == "diffuseColor" then
		Polycode.LightInfo_set_diffuseColor(self.__ptr, value.__ptr)
		return true
	elseif name == "specularColor" then
		Polycode.LightInfo_set_specularColor(self.__ptr, value.__ptr)
		return true
	elseif name == "constantAttenuation" then
		Polycode.LightInfo_set_constantAttenuation(self.__ptr, value)
		return true
	elseif name == "linearAttenuation" then
		Polycode.LightInfo_set_linearAttenuation(self.__ptr, value)
		return true
	elseif name == "quadraticAttenuation" then
		Polycode.LightInfo_set_quadraticAttenuation(self.__ptr, value)
		return true
	elseif name == "intensity" then
		Polycode.LightInfo_set_intensity(self.__ptr, value)
		return true
	elseif name == "spotlightCutoff" then
		Polycode.LightInfo_set_spotlightCutoff(self.__ptr, value)
		return true
	elseif name == "spotlightExponent" then
		Polycode.LightInfo_set_spotlightExponent(self.__ptr, value)
		return true
	elseif name == "shadowsEnabled" then
		Polycode.LightInfo_set_shadowsEnabled(self.__ptr, value)
		return true
	elseif name == "lightViewMatrix" then
		Polycode.LightInfo_set_lightViewMatrix(self.__ptr, value.__ptr)
		return true
	end
	return false
end
function LightInfo:__delete()
	if self then Polycode.delete_LightInfo(self.__ptr) end
end
