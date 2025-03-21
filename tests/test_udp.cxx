#include <gtest/gtest.h>
#include "../pcap/parser.h"
#include "../protocols/ethernet.h"
#include "../protocols/ipv4.h"
#include "../protocols/udp.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <iomanip>

namespace {
    std::filesystem::path pcapPath("simba-100.pcap");

    TEST(UdpParserTest, VerifyPorts) {
        pcap::Parser parser;
        parser.open(pcapPath);
        auto record = parser.readNextRecord().value();
        protocols::Ethernet frame(record.getData());
        frame.parse();
        protocols::Ipv4 ipv4(frame.getPayload());
        ipv4.parse();
        protocols::Udp udp(ipv4.payload());
        auto parsed = udp.parse();
        EXPECT_TRUE(parsed.has_value());
        EXPECT_EQ(udp.getDestinationPort(), 20081);
        EXPECT_EQ(udp.getSourcePort(), 50139);
    }
} // namespace
