class "SoundManager"


function SoundManager:SoundManager(...)
	local arg = {...}
	for k,v in pairs(arg) do
		if type(v) == "table" then
			if v.__ptr ~= nil then
				arg[k] = v.__ptr
			end
		end
	end
	if self.__ptr == nil and arg[1] ~= "__skip_ptr__" then
		self.__ptr = Polycode.SoundManager(unpack(arg))
	end
end

function SoundManager:setListenerPosition(position)
	local retVal = Polycode.SoundManager_setListenerPosition(self.__ptr, position.__ptr)
end

function SoundManager:setListenerOrientation(orientation)
	local retVal = Polycode.SoundManager_setListenerOrientation(self.__ptr, orientation.__ptr)
end

function SoundManager:recordSound(rate, sampleSize)
	local retVal = Polycode.SoundManager_recordSound(self.__ptr, rate, sampleSize)
	return retVal
end

function SoundManager:stopRecording(generateFloatBuffer)
	local retVal = Polycode.SoundManager_stopRecording(self.__ptr, generateFloatBuffer)
	if retVal == nil then return nil end
	local __c = _G["Sound"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function SoundManager:setAudioInterface(audioInterface)
	local retVal = Polycode.SoundManager_setAudioInterface(self.__ptr, audioInterface.__ptr)
end

function SoundManager:Update()
	local retVal =  Polycode.SoundManager_Update(self.__ptr)
end

function SoundManager:setGlobalVolume(globalVolume)
	local retVal = Polycode.SoundManager_setGlobalVolume(self.__ptr, globalVolume)
end

function SoundManager:registerSound(sound)
	local retVal = Polycode.SoundManager_registerSound(self.__ptr, sound.__ptr)
end

function SoundManager:unregisterSound(sound)
	local retVal = Polycode.SoundManager_unregisterSound(self.__ptr, sound.__ptr)
end

function SoundManager:__delete()
	if self then Polycode.delete_SoundManager(self.__ptr) end
end
