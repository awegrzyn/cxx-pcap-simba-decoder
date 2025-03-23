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

namespace Test {
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
        EXPECT_TRUE(result.has_value());
        const std::vector<protocols::SimbaSpectra::OrderUpdate>& orderUpdates = simba.getOrderUpdates();
        EXPECT_EQ(orderUpdates.size(), 1);
        const protocols::SimbaSpectra::OrderUpdate& orderUpdate = orderUpdates.front();
        EXPECT_EQ(orderUpdate.MDEntryID, 1949243857585620999);
        EXPECT_EQ(orderUpdate.MDEntryPx(), 144415);
        EXPECT_EQ(orderUpdate.MDEntrySize, 10);
        EXPECT_EQ(orderUpdate.MDFlags2, 0);
        EXPECT_EQ(orderUpdate.SecurityID, 3707491);
        EXPECT_EQ(orderUpdate.RptSeq, 881716);




    }

} // namespace