#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger
{
public:
  void setup(int baudRate);

  template <typename... Args>
  void debug(Args... args);

  template <typename... Args>
  void info(Args... args);

  template <typename... Args>
  void warn(Args... args);

  template <typename... Args>
  void error(Args... args);

private:
  template <typename... Args>
  void print(const String &severity, Args... args);

  template <typename T>
  void print_helper(T arg);

  template <typename T, typename... Args>
  void print_helper(T arg, Args... args);
};

#endif