#include <gtest/gtest.h>
#include "../pcap/parser.h"
#include "../protocols/ethernet.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <iomanip>

namespace {
    std::string getMacAsString(std::span<const std::byte> macAddress) {
        std::ostringstream oss;
        for (size_t i = 0; i < macAddress.size(); i++) {
            if (i > 0) {
                oss << ':';
            }
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(macAddress[i]);
        }
        return oss.str();
    }

    std::filesystem::path pcapPath("simba-100.pcap");

    TEST(EthernetParserTest, VerifyMacAddress) {
        pcap::Parser parser;
        parser.open(pcapPath);
        auto record = parser.readNextRecord().value();
        protocols::Ethernet frame(record.getData());
        frame.parse();
        EXPECT_EQ(getMacAsString(frame.getSourceMac()), "78:ac:44:3e:22:42");
    }
} // namespace