#pragma once

struct SMouseInput
{
	int button;
	int coords[2];
	int wheelDelta;
};

struct SWindow
{
	int windowEvent;
	int data1;
	int data2;	
};

struct SKeyboard
{
	int keyboardEvent;
	int data;
};

class CInputEvent
{
public:
	static const int IEVENT_KEY_DOWN = 1;
	static const int IEVENT_KEY_UP = 2;
	static const int IEVENT_MOUSE_MOVE = 4;
	static const int IEVENT_MOUSE_WHEEL = 8;
	static const int IEVENT_MOUSE_KEYDOWN = 16;
	static const int IEVENT_MOUSE_KEYUP = 32;
	static const int IEVENT_WINDOW = 64;
public:
	int eventId;
	SMouseInput mouse;
	SKeyboard keyboard;
	SWindow window;
public:
	void clearEventData();

	CInputEvent();
	~CInputEvent();
};

