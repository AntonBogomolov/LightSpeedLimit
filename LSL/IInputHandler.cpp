#include "IInputHandler.h"

IInputHandler::IInputHandler()
{
	priority = 0;
	isCapture = false;
}


IInputHandler::~IInputHandler()
{

}

const short IInputHandler::getPriority() const
{
	return priority;
}

void IInputHandler::setPriority(const short newPriority)
{
	this->priority = newPriority;

	if (this->priority > PRIORITY_MAX) this->priority = PRIORITY_MAX;
	if (this->priority < PRIORITY_MIN) this->priority = PRIORITY_MIN;
}

const int IInputHandler::getIsCapturedEvent() const
{
	return isCapture;
}

void IInputHandler::setIsCapturedEvent(const bool isCapture)
{
	this->isCapture = isCapture;
}