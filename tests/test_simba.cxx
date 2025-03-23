#include <gtest/gtest.h>
#include "../pcap/parser.h"
#include "../protocols/ethernet.h"
#include "../protocols/ipv4.h"
#include "../protocols/udp.h"
#include "../protocols/simba.h"
#include <filesystem>
#include <string>
#include <sstream>
#include <iomanip>

namespace {
    std::filesystem::path pcapPath("simba-100.pcap");

    TEST(SimbaParserTest, MessageType) {
        pcap::Parser parser;
        parser.open(pcapPath);
        auto record = parser.readNextRecord().value();
        protocols::Ethernet frame(record.getData());
        frame.parse();
        protocols::Ipv4 ipv4(frame.getPayload());
        ipv4.parse();
        protocols::Udp udp(ipv4.payload());
        auto parsed = udp.parse();
        protocols::SimbaSpectra simba(udp.getPayload());
        auto result = simba.parse();
        //EXPECT_TRUE(result.has_value());
        //EXPECT_EQ(simba.getMessageType(), protocols::SimbaSpectra::MessageType::MarketData);
    }
} // namespace
