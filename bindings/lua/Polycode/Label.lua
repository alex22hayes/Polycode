require "Polycode/Image"

class "Label" (Image)

Label.ANTIALIAS_FULL = 0
Label.ANTIALIAS_NONE = 1
Label.ANTIALIAS_STRONG = 2
Label.ANTIALIAS_LCD = 3
Label.ANTIALIAS_LCD_HINT = 4
Label.ANTIALIAS_FULL_HINT = 5

function Label:Label(...)
	local arg = {...}
	if type(arg[1]) == "table" and count(arg) == 1 then
		if ""..arg[1].__classname == "Image" then
			self.__ptr = arg[1].__ptr
			return
		end
	end
	for k,v in pairs(arg) do
		if type(v) == "table" then
			if v.__ptr ~= nil then
				arg[k] = v.__ptr
			end
		end
	end
	if self.__ptr == nil and arg[1] ~= "__skip_ptr__" then
		self.__ptr = Polycode.Label(unpack(arg))
	end
end

function Label:setText(text)
	local retVal = Polycode.Label_setText(self.__ptr, text)
end

function Label:getText()
	local retVal =  Polycode.Label_getText(self.__ptr)
	return retVal
end

function Label:getTextWidthForString(text)
	local retVal = Polycode.Label_getTextWidthForString(self.__ptr, text)
	return retVal
end

function Label:getTextHeightForString(text)
	local retVal = Polycode.Label_getTextHeightForString(self.__ptr, text)
	return retVal
end

function Label:getTextWidth()
	local retVal =  Polycode.Label_getTextWidth(self.__ptr)
	return retVal
end

function Label:getTextHeight()
	local retVal =  Polycode.Label_getTextHeight(self.__ptr)
	return retVal
end

function Label:setColorForRange(color, rangeStart, rangeEnd)
	local retVal = Polycode.Label_setColorForRange(self.__ptr, color.__ptr, rangeStart, rangeEnd)
end

function Label:clearColors()
	local retVal =  Polycode.Label_clearColors(self.__ptr)
end

function Label:getColorForIndex(index)
	local retVal = Polycode.Label_getColorForIndex(self.__ptr, index)
	if retVal == nil then return nil end
	local __c = _G["Color"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function Label:getPremultiplyAlpha()
	local retVal =  Polycode.Label_getPremultiplyAlpha(self.__ptr)
	return retVal
end

function Label:setPremultiplyAlpha(val)
	local retVal = Polycode.Label_setPremultiplyAlpha(self.__ptr, val)
end

function Label:setFont(newFont)
	local retVal = Polycode.Label_setFont(self.__ptr, newFont.__ptr)
end

function Label:getFont()
	local retVal =  Polycode.Label_getFont(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["shared_ptr<Font>"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function Label:setSize(newSize)
	local retVal = Polycode.Label_setSize(self.__ptr, newSize)
end

function Label:getSize()
	local retVal =  Polycode.Label_getSize(self.__ptr)
	return retVal
end

function Label:getAntialiasMode()
	local retVal =  Polycode.Label_getAntialiasMode(self.__ptr)
	return retVal
end

function Label:setAntialiasMode(newMode)
	local retVal = Polycode.Label_setAntialiasMode(self.__ptr, newMode)
end

function Label:getBaselineAdjust()
	local retVal =  Polycode.Label_getBaselineAdjust(self.__ptr)
	return retVal
end

function Label:setBackgroundColor(color)
	local retVal = Polycode.Label_setBackgroundColor(self.__ptr, color.__ptr)
end

function Label:setForegroundColor(color)
	local retVal = Polycode.Label_setForegroundColor(self.__ptr, color.__ptr)
end

function Label:setColors(backgroundColor, foregroundColor)
	local retVal = Polycode.Label_setColors(self.__ptr, backgroundColor.__ptr, foregroundColor.__ptr)
end

function Label:getBackgroundColor()
	local retVal =  Polycode.Label_getBackgroundColor(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["Color"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function Label:getForegroundColor()
	local retVal =  Polycode.Label_getForegroundColor(self.__ptr)
	if retVal == nil then return nil end
	local __c = _G["Color"]("__skip_ptr__")
	__c.__ptr = retVal
	return __c
end

function Label:optionsChanged()
	local retVal =  Polycode.Label_optionsChanged(self.__ptr)
	return retVal
end

function Label:__delete()
	if self then Polycode.delete_Label(self.__ptr) end
end
