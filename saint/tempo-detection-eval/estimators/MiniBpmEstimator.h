#pragma once

#include "../BpmEstimator.h"

#include "MiniBpm.h"

namespace saint {
class MiniBpmEstimator : public BpmEstimator {
public:
  MiniBpmEstimator(int sampleRate);
  void process(float *, size_t) override;
  std::optional<float> getEstimate() override;

private:
  breakfastquay::MiniBPM _miniBpm;
};
} // namespace saint
