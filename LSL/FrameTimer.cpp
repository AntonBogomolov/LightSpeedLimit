#include "FrameTimer.h"
#include "Log.h"

CFrameTimer::CFrameTimer(const unsigned long print_interval, const unsigned long update_interval)
{
	_nframes = 0;
	_print_nframes = 0;
	_fps = 0;
	dt = 0;

	fpsList = new std::vector<float>();
	needToCollectFps = false;

	reset();
	_print_interval = print_interval;
	_update_interval = update_interval;
	printStr = "FPS: ";
}

void CFrameTimer::addFrame()
{
	Stop();
	dt = GetInterval();
	
	_nframes++;
	_print_nframes++;
		
	if (_print_interval != 0 && _nframes != 0 && (GetTotal() > _print_interval))
	{
		float _print_fps = (float)(_print_nframes) / (_print_interval);		
		CLog::getInstance()->addInfo(printStr + std::to_string((int)_print_fps));
		if (needToCollectFps) fpsList->push_back(_print_fps);

		_print_nframes = 0;
		Reset();
	}
	Continue();
}

void CFrameTimer::setPrintString(const std::string str)
{
	printStr = str;
}

CFrameTimer::~CFrameTimer()
{
	if (fpsList) delete fpsList;
}

const double CFrameTimer::getDT()const
{
	return dt;
}

void CFrameTimer::reset()
{

}

const long CFrameTimer::getFrame()const
{
	return _nframes;
}

const long CFrameTimer::getInterval()const
{
	return _frame_interval;
}

const float CFrameTimer::getFPS()const
{
	return _fps;
}

std::vector<float>* CFrameTimer::getFpsList()const
{
	return fpsList;
}

const int CFrameTimer::getFpsListSize()const
{
	return fpsList->size();
}

void CFrameTimer::clearFpsList()
{
	fpsList->clear();
}

const float CFrameTimer::getAverageFps(const int sec)const
{
	if (!needToCollectFps) return -1;
	int size = getFpsListSize();
	if (size < sec)	return -1;

	float  sum = 0;
	int index = size - 1;
	for (int i = index; i >= size - sec; i--)
	{
		sum += (*fpsList)[i];
	}

	return sum / sec;
}

const bool CFrameTimer::isNeedToCollectFps()const
{
	return needToCollectFps;
}

void CFrameTimer::setNeedToCollectFps(const bool needToCollectFps)
{
	this->needToCollectFps = needToCollectFps;
	clearFpsList();
}