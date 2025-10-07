cmake_minimum_required(VERSION 3.16)
project(posenet_cli CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -march=armv8-a+simd -mtune=cortex-a53")

set(TF_ROOT "$ENV{HOME}/tensorflow")

find_package(OpenCV REQUIRED)

set(TFLITE_LIB_DIR "${TF_ROOT}/tensorflow/lite/tools/make/gen/linux_aarch64/lib")
set(TFLITE_LIB "${TFLITE_LIB_DIR}/libtensorflow-lite.a")

include_directories(
  ${TF_ROOT}
  ${TF_ROOT}/tensorflow/lite/tools/make/downloads/flatbuffers/include
)


find_package(OpenCV 4 REQUIRED)

add_executable(posenet_cli Pose_single.cpp)

target_link_libraries(posenet_cli
  ${OpenCV_LIBS}
  ${TFLITE_LIB}
  dl pthread rt m
)
