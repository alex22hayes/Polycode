class "SceneRenderTexture"


function SceneRenderTexture:__getvar(name)
	if name == "enabled" then
		return Polycode.SceneRenderTexture_get_enabled(self.__ptr)
	end
end

function SceneRenderTexture:__setvar(name,value)
	if name == "enabled" then
		Polycode.SceneRenderTexture_set_enabled(self.__ptr, value)
		return true
	end
	return false
end
function SceneRenderTexture:SceneRenderTexture(...)
	local arg = {...}
	for k,v in pairs(arg) do
		if type(v) == "table" then
			if v.__ptr ~= nil then
				arg[k] = v.__ptr
			end
		end
	end
	if self.__ptr == nil and arg[1] ~= "__skip_ptr__" then
		self.__ptr = Polycode.SceneRenderTexture(unpack(arg))
	end
end

function SceneRenderTexture:getTargetTexture()
	local retVal =  Polycode.SceneRenderTexture_getTargetTexture(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["shared_ptr<Texture>"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function SceneRenderTexture:resizeRenderTexture(newWidth, newHeight)
	local retVal = Polycode.SceneRenderTexture_resizeRenderTexture(self.__ptr, newWidth, newHeight)
end

function SceneRenderTexture:__delete()
	if self then Polycode.delete_SceneRenderTexture(self.__ptr) end
end
