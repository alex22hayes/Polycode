class "SpriteState"


function SpriteState:SpriteState(...)
	local arg = {...}
	for k,v in pairs(arg) do
		if type(v) == "table" then
			if v.__ptr ~= nil then
				arg[k] = v.__ptr
			end
		end
	end
	if self.__ptr == nil and arg[1] ~= "__skip_ptr__" then
		self.__ptr = Polycode.SpriteState(unpack(arg))
	end
end

function SpriteState:setName(name)
	local retVal = Polycode.SpriteState_setName(self.__ptr, name)
end

function SpriteState:getName()
	local retVal =  Polycode.SpriteState_getName(self.__ptr)
	return retVal
end

function SpriteState:getNumFrameIDs()
	local retVal =  Polycode.SpriteState_getNumFrameIDs(self.__ptr)
	return retVal
end

function SpriteState:getFrameIDAtIndex(index)
	local retVal = Polycode.SpriteState_getFrameIDAtIndex(self.__ptr, index)
	return retVal
end

function SpriteState:getMeshForFrameIndex(index)
	local retVal = Polycode.SpriteState_getMeshForFrameIndex(self.__ptr, index)
	if retVal == nil then return nil end
	local __c = _G["shared_ptr<Mesh>"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function SpriteState:insertFrame(index, frameID)
	local retVal = Polycode.SpriteState_insertFrame(self.__ptr, index, frameID)
end

function SpriteState:removeFrameByIndex(frameIndex)
	local retVal = Polycode.SpriteState_removeFrameByIndex(self.__ptr, frameIndex)
end

function SpriteState:clearFrames()
	local retVal =  Polycode.SpriteState_clearFrames(self.__ptr)
end

function SpriteState:setPixelsPerUnit(ppu)
	local retVal = Polycode.SpriteState_setPixelsPerUnit(self.__ptr, ppu)
end

function SpriteState:getPixelsPerUnit()
	local retVal =  Polycode.SpriteState_getPixelsPerUnit(self.__ptr)
	return retVal
end

function SpriteState:rebuildStateMeshes()
	local retVal =  Polycode.SpriteState_rebuildStateMeshes(self.__ptr)
end

function SpriteState:setStateFPS(fps)
	local retVal = Polycode.SpriteState_setStateFPS(self.__ptr, fps)
end

function SpriteState:getStateFPS()
	local retVal =  Polycode.SpriteState_getStateFPS(self.__ptr)
	return retVal
end

function SpriteState:getLargestFrameBoundingBox()
	local retVal =  Polycode.SpriteState_getLargestFrameBoundingBox(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["Vector3"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function SpriteState:setBoundingBox(boundingBox)
	local retVal = Polycode.SpriteState_setBoundingBox(self.__ptr, boundingBox.__ptr)
end

function SpriteState:getBoundingBox()
	local retVal =  Polycode.SpriteState_getBoundingBox(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["Vector2"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function SpriteState:getSpriteOffset()
	local retVal =  Polycode.SpriteState_getSpriteOffset(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["Vector2"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function SpriteState:setSpriteOffset(offset)
	local retVal = Polycode.SpriteState_setSpriteOffset(self.__ptr, offset.__ptr)
end

function SpriteState:__delete()
	if self then Polycode.delete_SpriteState(self.__ptr) end
end
