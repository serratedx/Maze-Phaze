//    2005 Experimentsingameprogramming.com		   //
// for further information refer to license.txt    //
// or www.experimentsingameprogramming.com/license //

#include <windows.h>
#include "FrameTimer.h"

FrameTimer::FrameTimer(){

}

FrameTimer::~FrameTimer(){

}

//
//   FUNCTION: init(int fps)
//
//   PURPOSE: sets up the timer with a requested frame rate of the program, if it cannot be maintained frames will be skipped
//
void FrameTimer::init(int fps){
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&timeNow);
	QueryPerformanceCounter(&timePrevious);
	
	//init fps time info
	Requested_FPS=fps;

	//The number of intervals in the given timer, per frame at the requested rate.
	intervalsPerFrame = ( (float)timerFreq.QuadPart / Requested_FPS );

}

//
//   FUNCTION: framesToUpdate()
//
//   PURPOSE: returns the number of frames that need to be updated since
//	 the last time this function was called (a system that can keep this under 1 is ideal)
//
int FrameTimer::framesToUpdate(){
	int framesToUpdate = 0;
	QueryPerformanceCounter(&timeNow);

	intervalsSinceLastUpdate = (float)timeNow.QuadPart - (float)timePrevious.QuadPart;
	
	framesToUpdate = (int)( intervalsSinceLastUpdate / intervalsPerFrame );

	//If we are not updateing any frames, keep the old previous timer count
	if (framesToUpdate != 0){
		QueryPerformanceCounter(&timePrevious);
	}

	return framesToUpdate;
}