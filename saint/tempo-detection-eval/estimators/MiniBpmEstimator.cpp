#include "MiniBpmEstimator.h"

namespace saint {
MiniBpmEstimator::MiniBpmEstimator(int sampleRate)
    : _miniBpm(static_cast<float>(sampleRate)) {}

void MiniBpmEstimator::process(float *audio, size_t len) {
  _miniBpm.process(audio, static_cast<int>(len));
}

std::optional<float> MiniBpmEstimator::getEstimate() {
  return _miniBpm.estimateTempo();
}
} // namespace saint