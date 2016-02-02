#pragma once
#include "InputEvent.h"

class IInputHandler
{
public:
	static const short PRIORITY_MAX = 32767;
	static const short PRIORITY_MIN = 0;
private:
	short priority;
	bool isCapture;
public:
	virtual void inputEventHandler(const CInputEvent event) = 0;

	const short getPriority() const;
	void setPriority(const short newPriority);
	const int getIsCapturedEvent() const;
	void setIsCapturedEvent(const bool isCapture);
public:
	IInputHandler();
	virtual ~IInputHandler();
};

