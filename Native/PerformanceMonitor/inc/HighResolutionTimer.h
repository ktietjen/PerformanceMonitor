// File:		HighResolutionTimer.h
// Description:	Provides a high resolution timer for benchmarking purposes
//
// Copyright (c) 2018 Ken Tietjen. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
// 

#pragma once

#include <Windows.h>

// Class:		HighResolutionTimer 
// Description:	This class implements a high resolution timer for benchmarking purpose with a start, stop and
//				can return the number of real world seconds passed
class HighResolutionTimer
{
private:
	const long long _frequency;
	long long _startTime;
	long long _stopTime;
	bool _running;

public:
	// Method:		HighResolutionTimer
	// Description: Constructor that provides initialization
	HighResolutionTimer() noexcept : _frequency{ Frequency() }, _startTime{ 0 }, _stopTime{ 0 }, _running{ false }
	{};

	// Method:		Start
	// Description: Starts or restarts the high resolution timer
	inline void Start() noexcept
	{
		_running = true;
		_startTime = Now();
	};

	// Method:		Stop
	// Description: Stops the high resolution timer
	inline void Stop() noexcept
	{
		_running = false;
		_stopTime = Now();
	};

	// Method:		ElapsedSeconds
	// Description: Returns the elapsed time in seconds
	inline double ElapsedSeconds() noexcept
	{
		return (double)(_stopTime - _startTime) / _frequency;
	};

	// Method:		IsRunning
	// Description:	Returns true if the timer has started
	inline bool IsRunning()
	{
		if ((_startTime != 0) && (_stopTime == 0))
			return true;

		return false;
	}

private:
	// Method:		Frequency
	// Description: Provides the frequency of the high resolution timer
	inline long long Frequency() noexcept
	{
		LARGE_INTEGER f;
		::QueryPerformanceFrequency(&f);
		return f.QuadPart;
	};

	// Method:		Now
	// Description: Gets the current high resolution time
	inline long long Now() noexcept
	{
		LARGE_INTEGER i;
		::QueryPerformanceCounter(&i);
		return i.QuadPart;
	};
};