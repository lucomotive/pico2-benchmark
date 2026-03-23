#include <chrono>
#include <iostream>

class Timer {
public:
  void start() { m_start = std::chrono::high_resolution_clock::now(); }

  void stop() { m_end = std::chrono::high_resolution_clock::now(); }

  double elapsedMs() const {
    return std::chrono::duration<double, std::milli>(m_end - m_start).count();
  }

  double elapsedSeconds() const {
    return std::chrono::duration<double>(m_end - m_start).count();
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start, m_end;
};
