#include <gtest/gtest.h>
#include "../pcap/parser.h"
#include <filesystem>
#include <string>

namespace {
    //std::string pcapPath = std::filesystem::current_path().string() + "/simba-100.pcap";
    std::filesystem::path pcapPath("simba-100.pcap");

    // Test that the parser can read at least one record
    TEST(PcapParserTest, ReadRecord) {
        pcap::Parser parser;
        auto file = parser.open(pcapPath);
        EXPECT_TRUE(file.has_value());
        EXPECT_EQ(parser.getFileHeader().version_major, 2);
        EXPECT_EQ(parser.getFileHeader().version_minor, 4);
    }

    // Test parsing all records in the file and verify the count
    TEST(PcapParserTest, ParseAllRecords) {
        pcap::Parser parser;
        parser.open(pcapPath);
        int packet_count = 0;
        while (true) {
            auto record_result = parser.readNextRecord();
            if (!record_result.has_value()) {
                if (record_result.error() == pcap::Error::UnexpectedEndOfFile) {
                    break;
                } else {
                    FAIL() << "Error reading record: " << pcap::error_message(record_result.error());
                }
            }
            packet_count++;
        }
        EXPECT_EQ(packet_count, 100) << "Expected 100 packets in the test file";
    }
} // namespace
