#pragma once

namespace saint {
class ProgressBar {
public:
  ProgressBar(size_t numSamples);
  void increment();

private:
  const size_t _numSamples;
  size_t _count = 0u;
};

} // namespace saint
