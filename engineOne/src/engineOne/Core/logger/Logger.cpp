#include "pch.h"
#include "Logger.h"

Logger Logger::m_Instance;

Logger& Logger::GetInstance() noexcept
{
    return m_Instance;
}

void Logger::Init(LogLevel minLevel) noexcept
{
    m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(m_hConsole, &mode);
    SetConsoleMode(m_hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

 void Logger::Log(LogLevel level, const std::string& msg) const noexcept
{
    if (level < m_MinLevel) return;

    auto levelString = GetLevelString(level);
    SetLevelColor(level);
    std::string fullMessage = std::format("[{}]  {}\n", levelString, msg);
    std::cout << fullMessage;
    ResetColor();
}

void Logger::SetLevelColor(LogLevel level) const noexcept
{
    switch (level) {
    case LogLevel::Trace: std::cout << "\033[94m"; break; // bright blue
    case LogLevel::Debug: std::cout << "\033[90m"; break; // bright black / gray
    case LogLevel::Info:  std::cout << "\033[92m"; break; // bright green
    case LogLevel::Warn:  std::cout << "\033[93m"; break; // bright yellow
    case LogLevel::Error: std::cout << "\033[91m"; break; // bright red
    case LogLevel::Fatal: std::cout << "\033[95m"; break; // bright magenta
    }

}

void Logger::ResetColor() const noexcept
{

    std::cout << "\033[0m"; // reset color
}
