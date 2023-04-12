add_executable(tempo-detection-eval
  main.cpp
  WavFileReader.cpp
  BpmEstimator.cpp
  estimators/MiniBpmEstimator.cpp
)

target_link_libraries(tempo-detection-eval
  juce::juce_audio_utils
  juce::juce_audio_formats
  MiniBpm
)
