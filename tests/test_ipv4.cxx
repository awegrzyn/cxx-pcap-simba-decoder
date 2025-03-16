#include <gtest/gtest.h>
#include "../pcap/parser.h"
#include "../protocols/ethernet.h"
#include "../protocols/ipv4.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <iomanip>

namespace {
    std::string ipToString(std::span<const std::byte> ip) {
        std::ostringstream oss;
        for (size_t i = 0; i < ip.size(); i++) {
            if (i > 0) {
                oss << '.';
            }
            oss << std::to_integer<int>(ip[i]);
        }
        return oss.str();
    }

    std::filesystem::path pcapPath("simba-100.pcap");

    TEST(Ipv4ParserTest, VerifyIpAddresses) {
        pcap::Parser parser;
        parser.open(pcapPath);
        auto record = parser.readNextRecord().value();
        protocols::Ethernet frame(record.getData());
        frame.parse();
        protocols::Ipv4 ipv4(frame.getPayload());
        auto parsed = ipv4.parse();
        EXPECT_TRUE(parsed.has_value());
        ipv4.getSourceIp();
        EXPECT_EQ(ipToString(ipv4.getSourceIp()), "91.203.253.244");
    }
} // namespace
