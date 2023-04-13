#include "ProgressBar.h"

#include <iostream>

namespace saint {
ProgressBar::ProgressBar(size_t numSamples) : _numSamples(numSamples) {}

void ProgressBar::increment() {
  // Adapted from
  // https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
  int barWidth = 70;
  std::cout << "[";
  const auto progress =
      static_cast<float>(_count) / static_cast<float>(_numSamples);
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";
  std::cout.flush();
  ++_count;
}
} // namespace saint