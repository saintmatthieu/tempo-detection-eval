#pragma once

#include <memory>
#include <optional>

namespace saint {
class BpmEstimator {
public:
  enum class Solution {
    MiniBpm,
    _count,
  };

  static std::unique_ptr<BpmEstimator> createInstance(Solution, int sampleRate);
  virtual void process(float *, size_t) = 0;
  virtual std::optional<float> getEstimate() = 0;
  virtual ~BpmEstimator() = default;
};
} // namespace saint