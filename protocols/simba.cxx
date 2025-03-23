#include "simba.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iomanip>

namespace protocols {

// Constructor
SimbaSpectra::SimbaSpectra(std::span<const std::byte> udpData) 
    : mUdpData(udpData), mParsingOffset(0) {
}


// Implementation of parse method - similar to reference implementation
std::expected<bool, SimbaSpectra::Error> SimbaSpectra::parse() {
    MarketDataHeader header = parseMarketDataHeader();
    if (header.IsIncremental()) {
        IncrementalPacketHeader incrementalHeader = parseIncrementalPacketHeader();
    }
    while (mParsingOffset < mUdpData.size()) {
        SbeMessageHeader sbeHeader = parseSbeMessageHeader();
        if (sbeHeader.TemplateId == OrderUpdate::templateId()) {
            mOrderUpdates.emplace_back(parseOrderUpdate());
        } else if (sbeHeader.TemplateId == OrderExecution::templateId()) {
            mOrderExecutions.emplace_back(parseOrderExecution());
        } else if (sbeHeader.TemplateId == OrderBookSnapshot::templateId()) {
            mOrderBookSnapshots.emplace_back(parseOrderBookSnapshot());
        } else {
            mParsingOffset += sbeHeader.BlockLength;
        }
    }
    return true;
}
SimbaSpectra::MarketDataHeader SimbaSpectra::parseMarketDataHeader() const {
    MarketDataHeader header;
    std::memcpy(&header, mUdpData.data() + mParsingOffset, MarketDataHeader::size());
    mParsingOffset += MarketDataHeader::size();
    return header;
}

SimbaSpectra::IncrementalPacketHeader SimbaSpectra::parseIncrementalPacketHeader() const {
    IncrementalPacketHeader header;
    std::memcpy(&header, mUdpData.data() + mParsingOffset, IncrementalPacketHeader::size());
    mParsingOffset += IncrementalPacketHeader::size();
    return header;
}

SimbaSpectra::SbeMessageHeader SimbaSpectra::parseSbeMessageHeader() const {
    SbeMessageHeader header;
    std::memcpy(&header, mUdpData.data() + mParsingOffset, SbeMessageHeader::size());
    mParsingOffset += SbeMessageHeader::size();
    return header;
}

SimbaSpectra::OrderUpdate SimbaSpectra::parseOrderUpdate() const {
    OrderUpdate update;
    std::memcpy(&update, mUdpData.data() + mParsingOffset, OrderUpdate::size());
    mParsingOffset += OrderUpdate::size();
    return update;
}

SimbaSpectra::OrderExecution SimbaSpectra::parseOrderExecution() const {
    OrderExecution execution;
    std::memcpy(&execution, mUdpData.data() + mParsingOffset, OrderExecution::size());
    mParsingOffset += OrderExecution::size();
    return execution;
}
SimbaSpectra::OrderBookSnapshot SimbaSpectra::parseOrderBookSnapshot() const {
    OrderBookSnapshot snapshot;
    std::memcpy(&snapshot, mUdpData.data() + mParsingOffset, OrderBookSnapshot::size());
    mParsingOffset += OrderBookSnapshot::size();
    snapshot.entries.resize(snapshot.NoMDEntries.numInGroup);
    for (auto& entry : snapshot.entries) {
        std::memcpy(&entry, mUdpData.data() + mParsingOffset, OrderBookSnapshot::Entry::size());
        mParsingOffset += OrderBookSnapshot::Entry::size();
    }
    return snapshot;
}
} // namespace protocols