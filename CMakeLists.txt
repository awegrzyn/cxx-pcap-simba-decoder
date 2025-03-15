CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

project(PcapSimba
    VERSION 0.0.1
    LANGUAGES CXX
)

find_package(GTest CONFIG REQUIRED)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

add_executable(pcap main.cxx pcap/parser.cxx protocols/ethernet.cxx)
target_compile_features(pcap PRIVATE cxx_std_23)


if (GTest_FOUND)
    configure_file(tests/simba-100.pcap ${CMAKE_CURRENT_BINARY_DIR}/simba-100.pcap COPYONLY)
    include(GoogleTest)
    add_executable(test_pcap tests/test_pcap.cxx pcap/parser.cxx)
    target_compile_features(test_pcap PRIVATE cxx_std_23)
    target_link_libraries(test_pcap PRIVATE GTest::gtest GTest::gtest_main)

    enable_testing()
    add_test(NAME test_pcap COMMAND test_pcap)
    gtest_add_tests(TARGET test_pcap test_pcap)
endif()
