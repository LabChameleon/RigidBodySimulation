#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer()		{}

	void start()
	{
		m_startTime = std::chrono::system_clock::now();
	}

	std::chrono::duration<double> restart()
	{
		std::chrono::duration<double> duration = std::chrono::system_clock::now() - m_startTime;
		
		start();

		return duration;
	}

	std::chrono::duration<double> getElapsedTime()
	{
		return std::chrono::system_clock::now() - m_startTime;
	}

private:
	std::chrono::system_clock::time_point m_startTime;
};

#endif // TIMER_H