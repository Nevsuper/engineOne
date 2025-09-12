#pragma once
#include <Windows.h>

class Timer
{
public:
	Timer() noexcept
		:
		m_Frequency{},
		m_Time{}
	{
		QueryPerformanceFrequency(&m_Frequency);
		QueryPerformanceCounter(&m_Time);
	}
	Timer(const Timer&) noexcept = delete;
	Timer& operator=(const Timer&) noexcept = delete;

	Timer(Timer&&) noexcept = default;
	Timer& operator=(Timer&&) noexcept = default;

	inline void reset()
	{
		QueryPerformanceCounter(&m_Time);
	}

	inline void resetFrequency() noexcept
	{
		QueryPerformanceFrequency(&m_Frequency);
	}

	
	inline double elapsed() const noexcept
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return static_cast<double>(currentTime.QuadPart - m_Time.QuadPart) / static_cast<double>(m_Frequency.QuadPart);
	}
	inline double elapsedAndReset() noexcept
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		double dt = static_cast<double>(currentTime.QuadPart - m_Time.QuadPart) / static_cast<double>(m_Frequency.QuadPart);
		m_Time = currentTime; // reset after computing elapsed
		return dt;
	}




private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_Time;
};

