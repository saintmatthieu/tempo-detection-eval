#include "BpmEstimator.h"
#include "ProgressBar.h"
#include "WavFileReader.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;
using namespace std::literals::string_literals;

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
  const std::regex regex{"bpm=([0-9]+|none)"};
  for (const auto &entry : fs::recursive_directory_iterator(audioDir)) {
    const fs::path path = entry;
    if (path.extension() != ".wav") {
      continue;
    }
    const auto filename = path.stem().string();
    std::smatch match;
    if (!std::regex_search(filename, match, regex)) {
      std::cerr << filename + " has unexpected format" << std::endl;
      continue;
    } else {
      const auto bpmStr = match[1].str();
      std::optional<int> bpm =
          bpmStr == "none" ? std::nullopt : std::optional<int>{stoi(bpmStr)};
      tempi.push_back(bpm);
      files.push_back(path);
    }
  }

  using Solution = saint::BpmEstimator::Solution;

  struct Result {
    bool falsePositive = false;
    bool falseNegative = false;
    std::optional<float> error;
  };

  std::map<Solution, std::vector<Result>> results;

  saint::ProgressBar progress(files.size());
  std::cout << "Running tempo detection on " << files.size() << " files"
            << std::endl;
  for (auto i = 0u; i < files.size(); ++i) {
    progress.increment();
    saint::WavFileReader reader(files[i]);
    const auto sr = reader.getSampleRate();
    const auto actualBpm = tempi[i];
    constexpr auto numSolutions = static_cast<int>(Solution::_count);
    for (auto j = 0; j < numSolutions; ++j) {
      const auto solution = static_cast<Solution>(j);
      const auto estimator = saint::BpmEstimator::createInstance(solution, sr);
      constexpr auto blockSize = 512;
      while (reader.getNumSamplesPerChannelAvailable() > 0) {
        const auto numSamples =
            std::min(blockSize, reader.getNumSamplesPerChannelAvailable());
        std::vector<float> buffer(numSamples);
        reader.read(buffer.data(), numSamples);
        // Only keep left channel
        for (auto i = 0; i < numSamples / 2; ++i) {
          buffer[i] = buffer[i * 2];
        }
        estimator->process(buffer.data(), buffer.size() / 2u);
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
