#include "InputEvent.h"

CInputEvent::CInputEvent()
{
	eventId = 0;

	mouse.button = 0;
	mouse.wheelDelta = 0;
	mouse.coords[0] = 0;
	mouse.coords[1] = 0;

	keyboard.data = 0;
	keyboard.keyboardEvent = 0;

	window.windowEvent = 0;
	window.data1 = 0;
	window.data2 = 0;
}

CInputEvent::~CInputEvent()
{

}

void CInputEvent::clearEventData()
{
	eventId = 0;

	mouse.button = 0;
	mouse.wheelDelta = 0;
	mouse.coords[0] = 0;
	mouse.coords[1] = 0;

	keyboard.data = 0;
	keyboard.keyboardEvent = 0;

	window.windowEvent = 0;
	window.data1 = 0;
	window.data2 = 0;
}