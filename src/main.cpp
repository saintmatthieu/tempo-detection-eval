#include "MiniBpm.h"
#include "WavFileReader.h"

#include <iostream>

int main(int, char **) {
  WavFileReader reader(
      "C:\\Users\\saint\\Downloads\\Hipness_808_fonky3_89.wav");
  breakfastquay::MiniBPM miniBpm(48000);
  constexpr auto blockSize = 512;
  while (reader.getNumSamplesAvailable() > 0) {
    const auto numSamples =
        std::min(blockSize, reader.getNumSamplesAvailable());
    std::vector<float> buffer(numSamples);
    reader.read(buffer.data(), numSamples);
    miniBpm.process(buffer.data(), numSamples);
  }
  const auto result = miniBpm.estimateTempo();
  std::cout << result << std::endl;
}
