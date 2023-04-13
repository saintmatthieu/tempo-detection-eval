add_executable(tempo-detection-eval
  main.cpp
  BpmEstimator.cpp
  estimators/MiniBpmEstimator.cpp
)

target_link_libraries(tempo-detection-eval
  PRIVATE
    wavfile
    MiniBpm
)
