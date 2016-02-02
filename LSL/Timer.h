#pragma once
#include "../../tools/includes/SDL.h"

class CTimer
{
protected:
	/// Is the timer running?
	bool _running;
	/// Start time mark
	Uint64 _countPerSec;
	Uint64 _starttime;
	/// Stop time mark
	Uint64 _stoptime;
	/// Total elapsed time
	Uint64 _total;
public:
	/// Empty constructor
	CTimer();
	/// Reset the timer
	void Reset();
	/// Start the timer
	void Start();
	/// Stop the timer without resetting it
	void Stop();
	/// Resume the timer
	void Continue();
	/// Return (stop - start) or (now - start) time interval
	const double GetInterval()const;
	/// Return total elapsed time
	const double GetTotal()const;
	/// Return current time
	const Uint64 GetNow()const;
};

