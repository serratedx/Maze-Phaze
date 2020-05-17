#pragma once
/*
**The Frame timer, takes a fps value
**When you call framesToUpdate it returns to you how many frames you need to update
**To maintain that fps value. Example, if you asked for 60FPS and the computer could only
**render 30, frames to update would return 2.
*/

class FrameTimer
{
public:
	FrameTimer(void);
	~FrameTimer(void);

	void init(int fps);
	int framesToUpdate();

private:
	
	// timer variables
	LARGE_INTEGER timeNow;
	LARGE_INTEGER timePrevious;
	LARGE_INTEGER timerFreq;

	float fps;
	float intervals;
	float intervalsSinceLastUpdate;
	float framesInLastSecond;
	float intervalsPerFrame;
	int Requested_FPS;

};