#pragma once
#include<string>
#include<format>
#include<WindowsLean.h>

enum class LogLevel
{
	Trace = 0,
	Debug = 1,
	Info  = 2,
	Warn  = 3,
	Error = 4,
	Fatal = 5
};
inline constexpr std::string GetLevelString(LogLevel level) {
	switch (level) {
	case LogLevel::Trace: return "TRACE";
	case LogLevel::Debug: return "DEBUG";
	case LogLevel::Info:  return "INFO ";
	case LogLevel::Warn:  return "WARN ";
	case LogLevel::Error: return "ERROR";
	case LogLevel::Fatal: return "FATAL";
	default: return "?????";
	}
}
class Logger
{
public:
	static Logger& GetInstance() noexcept;

	void Init(LogLevel minLevel = LogLevel::Info) noexcept;

	// Template versions for formatting
	template<typename... Args>
	void Debug(std::format_string<Args...> fmt, Args&&... args)
	{
		Log(LogLevel::Debug, std::format(fmt, std::forward<Args>(args)...));
	}

	template<typename... Args>
	void Info(std::format_string<Args...> fmt, Args&&... args) 
	{
		Log(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...));
	}

	template<typename... Args>
	void Warn(std::format_string<Args...> fmt, Args&&... args)
	{
		Log(LogLevel::Warn, std::format(fmt, std::forward<Args>(args)...));
	}

	template<typename... Args>
	void Error(std::format_string<Args...> fmt, Args&&... args)
	{
		Log(LogLevel::Error, std::format(fmt, std::forward<Args>(args)...));
	}
	template<typename... Args>
	void Fatal(std::format_string<Args...> fmt, Args&&... args)
	{
		Log(LogLevel::Fatal, std::format(fmt, std::forward<Args>(args)...));
	}

	void Trace(const std::string& message) const noexcept { Log(LogLevel::Trace,message); };
	void Info(const std::string& message) const noexcept  { Log(LogLevel::Info,message);  };
	void Debug(const std::string& message) const noexcept { Log(LogLevel::Debug,message); };
	void Warn(const std::string& message) const noexcept  { Log(LogLevel::Warn,message);  };
	void Error(const std::string& message) const noexcept { Log(LogLevel::Error,message); };
	void Fatal(const std::string& message) const noexcept { Log(LogLevel::Fatal,message); };

	void SetLevel(LogLevel level) noexcept { m_MinLevel = level; }

private:
	void Log(LogLevel level, const std::string& msg) const noexcept;

	void SetLevelColor(LogLevel level) const noexcept;
	void ResetColor() const noexcept;
private:
	HANDLE m_hConsole = nullptr;
	LogLevel m_MinLevel = LogLevel::Trace;
	static Logger m_Instance;
};


#ifndef NDEBUG

#define LOG_TRACE(msg, ...) Logger::GetInstance().Trace(msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) Logger::GetInstance().Debug(msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) Logger::GetInstance().Info(msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) Logger::GetInstance().Warn(msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) Logger::GetInstance().Error(msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...) Logger::GetInstance().Fatal(msg, ##__VA_ARGS__)

#else

#define LOG_TRACE(msg, ...) 
#define LOG_DEBUG(msg, ...) 
#define LOG_INFO(msg, ...) 
#define LOG_WARN(msg, ...) 
#define LOG_ERROR(msg, ...) 

#endif // NDEBUG


