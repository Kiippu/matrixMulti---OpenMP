#pragma once
#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <iostream>


/*

Class:	Global Timer
Description: timer class to measure elapsed time metric.

*/
/// time types
enum class TIME_MEASURMENT {
	Nano,
	Milli,
	Second
};

/// typedefs for easy type creation
using TIME_REGISTER = std::map <std::string,std::chrono::time_point<std::chrono::steady_clock>>;
using TIME_VECTOR_PAIR = std::pair<std::string, long long>;
using TIME_VECTOR = std::vector<TIME_VECTOR_PAIR>;

class GlobalTimer
{
public:
	GlobalTimer() {};
	~GlobalTimer() {};
	/// adds lable and time type to map
	void addStartTime(TIME_MEASURMENT, std::string displayName);
	/// finds lable and enters  finish time
	void addFinishTime(std::string id);
	/// print the timers added and finished to screen
	void printFinalTimeSheet();

private:

	TIME_REGISTER m_beginTimerList;
	TIME_REGISTER m_finishTimerList;
	TIME_VECTOR m_finalTimerSheetSec;
	TIME_VECTOR m_finalTimerSheetMs;
	TIME_VECTOR m_finalTimerSheetNs;
};