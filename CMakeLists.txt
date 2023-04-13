cmake_minimum_required(VERSION 3.0.0)
project(tempo-detection-eval VERSION 0.1.0)

set(OP_DISABLE_DOCS 1)

add_subdirectory(JUCE)
add_subdirectory(minibpm)
add_subdirectory(ogg)
add_subdirectory(opus)
add_subdirectory(opusfile)
add_subdirectory(saint/musehub-file-decoder)
add_subdirectory(saint/tempo-detection-eval)
add_subdirectory(saint/wavfile)
