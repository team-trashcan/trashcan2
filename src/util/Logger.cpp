#include "util/Logger.h"
#include "util/Config.h"

void Logger::setup(int baudRate)
{
  Serial.begin(baudRate);
}

template <typename... Args>
void Logger::debug(Args... args)
{
  if (DEBUG_LOGGING_ENABLED)
  {
    print("DEBUG", args...);
  }
}

template <typename... Args>
void Logger::info(Args... args)
{
  print("INFO", args...);
}

template <typename... Args>
void Logger::warn(Args... args)
{
  print("WARN", args...);
}

template <typename... Args>
void Logger::error(Args... args)
{
  print("ERROR", args...);
}

template <typename... Args>
void Logger::print(const String &severity, Args... args)
{
  Serial.print("[");
  Serial.print(severity);
  Serial.print("] ");
  log_helper(args...);
}

template <typename T>
void Logger::print_helper(T arg)
{
  Serial.print(arg);
  Serial.println();
}

template <typename T, typename... Args>
void Logger::print_helper(T arg, Args... args)
{
  Serial.print(arg);
  log_helper(args...);
}

template void Logger::print<>(const String &, const char *);
template void Logger::debug<>(const char *);
template void Logger::info<>(const char *);
template void Logger::warn<>(const char *);
template void Logger::error<>(const char *);