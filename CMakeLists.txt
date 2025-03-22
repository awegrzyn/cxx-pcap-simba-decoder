CMAKE_MINIMUM_REQUIRED(VERSION 3.11.0 FATAL_ERROR)

project(PcapSimba
    VERSION 0.0.1
    LANGUAGES CXX
)

find_package(GTest CONFIG REQUIRED)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

add_executable(pcap main.cxx pcap/parser.cxx protocols/ethernet.cxx protocols/ipv4.cxx protocols/udp.cxx protocols/simba.cxx)
target_compile_features(pcap PRIVATE cxx_std_23)


if (GTest_FOUND)
    configure_file(tests/simba-100.pcap ${CMAKE_CURRENT_BINARY_DIR}/simba-100.pcap COPYONLY)
    include(GoogleTest)
    add_executable(test_pcap tests/test_pcap.cxx pcap/parser.cxx)
    target_compile_features(test_pcap PRIVATE cxx_std_23)
    target_link_libraries(test_pcap PRIVATE GTest::gtest GTest::gtest_main)

    add_executable(test_ethernet tests/test_ethernet.cxx pcap/parser.cxx protocols/ethernet.cxx)
    target_compile_features(test_ethernet PRIVATE cxx_std_23)
    target_link_libraries(test_ethernet PRIVATE GTest::gtest GTest::gtest_main)

    add_executable(test_ipv4 tests/test_ipv4.cxx pcap/parser.cxx protocols/ethernet.cxx protocols/ipv4.cxx)
    target_compile_features(test_ipv4 PRIVATE cxx_std_23)
    target_link_libraries(test_ipv4 PRIVATE GTest::gtest GTest::gtest_main)

    add_executable(test_udp tests/test_udp.cxx pcap/parser.cxx protocols/ethernet.cxx protocols/ipv4.cxx protocols/udp.cxx)
    target_compile_features(test_udp PRIVATE cxx_std_23)
    target_link_libraries(test_udp PRIVATE GTest::gtest GTest::gtest_main)

    add_executable(test_simba tests/test_simba.cxx pcap/parser.cxx protocols/ethernet.cxx protocols/ipv4.cxx protocols/udp.cxx protocols/simba.cxx)
    target_compile_features(test_simba PRIVATE cxx_std_23)
    target_link_libraries(test_simba PRIVATE GTest::gtest GTest::gtest_main)

    enable_testing()
    add_test(NAME test_pcap COMMAND test_pcap)
    add_test(NAME test_ethernet COMMAND test_ethernet)
    add_test(NAME test_ipv4 COMMAND test_ipv4)
    add_test(NAME test_udp COMMAND test_udp)
    add_test(NAME test_simba COMMAND test_simba)
    gtest_add_tests(TARGET test_simba test_simba)
    gtest_add_tests(TARGET test_udp test_udp)
    gtest_add_tests(TARGET test_pcap test_pcap)
    gtest_add_tests(TARGET test_ethernet test_ethernet)
    gtest_add_tests(TARGET test_ipv4 test_ipv4)
endif()
