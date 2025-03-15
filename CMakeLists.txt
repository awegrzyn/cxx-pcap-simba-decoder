CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

project(PcapSimba
  VERSION 0.0.1
  LANGUAGES CXX
)

add_executable(pcap main.cxx pcap/parser.cxx)
target_compile_features(pcap PRIVATE cxx_std_23)
