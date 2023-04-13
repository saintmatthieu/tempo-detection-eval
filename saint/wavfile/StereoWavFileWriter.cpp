#include "StereoWavFileWriter.h"

namespace saint {
namespace fs = std::filesystem;

namespace {
std::unique_ptr<juce::AudioFormatWriter>
getJuceWavFileWriter(const fs::path &path) {
  juce::WavAudioFormat format;
  std::unique_ptr<juce::AudioFormatWriter> writer;
  if (fs::exists(path)) {
    fs::remove(path);
  }
  writer.reset(format.createWriterFor(
      new juce::FileOutputStream(juce::File(path.string())), 48000.0, 2, 16, {},
      0));
  return writer;
}
} // namespace

StereoWavFileWriter::StereoWavFileWriter(const fs::path &path)
    : _juceWriter(getJuceWavFileWriter(path)) {}

bool StereoWavFileWriter::write(const float *audio, int size) {
  std::vector<float> left(size);
  std::vector<float> right(size);
  for (auto i = 0u; i < size; ++i) {
    left[i] = audio[i * 2u];
    right[i] = audio[i * 2u + 1u];
  }
  std::vector<const float *> channels(2);
  channels[0] = left.data();
  channels[1] = left.data();
  return _juceWriter->writeFromFloatArrays(
      channels.data(), static_cast<int>(channels.size()), size);
}
} // namespace saint
