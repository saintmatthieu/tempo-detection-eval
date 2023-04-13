#include "BpmEstimator.h"
#include "WavFileReader.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "need to provide a dir argument" << std::endl;
    return 1;
  }
  const fs::path audioDir{argv[1]};
  if (!fs::exists(audioDir)) {
    std::cerr << audioDir << " doesn't exist" << std::endl;
    return 1;
  }

  std::vector<fs::path> files;
  std::vector<std::optional<int>> tempi;
  for (const auto &entry : fs::recursive_directory_iterator(audioDir)) {
    const fs::path path = entry;
    if (path.extension() != ".wav") {
      continue;
    }
  }

  using Solution = saint::BpmEstimator::Solution;

  struct Result {
    bool falsePositive = false;
    bool falseNegative = false;
    std::optional<float> error;
  };

  std::map<Solution, std::vector<Result>> results;

  for (auto i = 0u; i < files.size(); ++i) {
    saint::WavFileReader reader(files[i]);
    const auto sr = reader.getSampleRate();
    const auto actualBpm = tempi[i];
    constexpr auto numSolutions = static_cast<int>(Solution::_count);
    for (auto j = 0; j < numSolutions; ++j) {
      const auto solution = static_cast<Solution>(j);
      const auto estimator = saint::BpmEstimator::createInstance(solution, sr);
      constexpr auto blockSize = 512;
      while (reader.getNumSamplesAvailable() > 0) {
        const auto numSamples =
            std::min(blockSize, reader.getNumSamplesAvailable());
        std::vector<float> buffer(numSamples);
        reader.read(buffer.data(), numSamples);
        estimator->process(buffer.data(), buffer.size());
      }
      const auto estimate = estimator->getEstimate();
      Result result;
      result.falseNegative = actualBpm.has_value() && !estimate.has_value();
      result.falsePositive = !actualBpm.has_value() && estimate.has_value();
      if (actualBpm.has_value() && estimate.has_value()) {
        result.error = *estimate - *actualBpm;
      }
      results[solution].emplace_back(std::move(result));
    }
  }

  struct Stats {
    float RMS = 0.f;
    float std = 0.f;
    float min = 0.f;
    float max = 0.f;
    float falsePositiveRate = 0.f;
    float falseNegativeRate = 0.f;
  };

  std::map<Solution, Stats> stats;
  for (const auto &entry : stats) {
  }
}
