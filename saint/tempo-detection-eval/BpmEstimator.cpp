#include "BpmEstimator.h"

#include "estimators/MiniBpmEstimator.h"

namespace saint {
std::unique_ptr<BpmEstimator> BpmEstimator::createInstance(Solution solution,
                                                           int sampleRate) {
  switch (solution) {
  case Solution::MiniBpm:
    return std::make_unique<MiniBpmEstimator>(sampleRate);
  default:
    return nullptr;
  }
}
} // namespace saint