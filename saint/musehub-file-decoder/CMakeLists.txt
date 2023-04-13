add_executable(musehub-file-decoder
  main.cpp
)

target_link_libraries(musehub-file-decoder PRIVATE
  opusfile
  wavfile
)
