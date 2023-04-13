#include "StereoWavFileWriter.h"
#include "opusfile.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace std::literals::string_literals;

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "need to provide input and output dir arguments" << std::endl;
    return 1;
  }
  const fs::path audioDir{argv[1]};
  if (!fs::exists(audioDir)) {
    std::cerr << audioDir << " doesn't exist" << std::endl;
    return 1;
  }

  const fs::path outputDir{argv[2]};
  if (fs::exists(outputDir)) {
    fs::remove_all(outputDir);
  }
  if (!fs::create_directory(outputDir)) {
    std::cerr << "Could not create output dir " << outputDir << std::endl;
    return 1;
  }

  for (const auto &entry : fs::recursive_directory_iterator(audioDir)) {
    const fs::path opusPath = entry;
    if (opusPath.extension() != ".opus") {
      continue;
    }
    auto err = 0;
    const auto file = op_open_file(opusPath.string().c_str(), &err);
    const auto tags = op_tags(file, 0);
    std::optional<std::string> bpm;
    std::string type;
    std::optional<std::string> key;
    for (auto i = 0; i < tags->comments; ++i) {
      std::string comment = tags->user_comments[i];
      std::transform(comment.begin(), comment.end(), comment.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      if (comment.find("type=") == 0u) {
        type = {comment.begin() + 5, comment.end()};
      } else if (comment.find("bpm=") == 0u) {
        bpm = {comment.begin() + 4, comment.end()};
      } else if (comment.find("key=") == 0u) {
        key = {comment.begin() + 4, comment.end()};
      }
    }
    if (type.empty()) {
      std::cerr << "Could not parse type of " << opusPath << std::endl;
      continue;
    }
    if (type == "loop" && !bpm.has_value()) {
      std::cerr << "Could not find BPM value for loop " << opusPath
                << std::endl;
      continue;
    }
    bpm = "bpm="s + (bpm.has_value() ? *bpm : "none");
    key = "key="s + (key.has_value() ? *key : "none");
    const auto filename = fs::path{*bpm}
                              .concat("_")
                              .concat(*key)
                              .concat("__")
                              .concat(opusPath.stem().string())
                              .concat(".wav");
    const fs::path wavPath = outputDir / filename;
    saint::StereoWavFileWriter writer{wavPath};
    std::array<float, 512> buff;
    while (const auto len =
               op_read_float_stereo(file, buff.data(), buff.size())) {
      writer.write(buff.data(), len);
    }
  }
}
