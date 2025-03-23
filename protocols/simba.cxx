#include "simba.h"
#include <algorithm>
#include <cstring>
#include <iostream>

#include "simba/MarketDataHeader.h"
#include "simba/IncrementalPacketHeader.h"
#include "simba/SbeMessageHeader.h"

namespace protocols {

SimbaSpectra::SimbaSpectra(std::span<const std::byte> udpData) 
    : mUdpData(udpData), mParsingOffset(0) {
}

std::expected<bool, SimbaSpectra::Error> SimbaSpectra::parse() {
    MarketDataHeader marketHeader(mUdpData.begin());
    advanceOffset(MarketDataHeader::size());

    if (marketHeader.IsIncremental()) {
        IncrementalPacketHeader incrementalHeader(mUdpData.begin() + mParsingOffset);
        advanceOffset(IncrementalPacketHeader::size());
    }

    while (mParsingOffset < mUdpData.size()) {
        SbeMessageHeader sbeHeader(mUdpData.begin() + mParsingOffset);
        advanceOffset(SbeMessageHeader::size());
        switch (sbeHeader.TemplateId()) {
            case OrderUpdate::templateId():
                mOrderUpdates.emplace_back(mUdpData.begin() + mParsingOffset);
                advanceOffset(OrderUpdate::size());
                break;
            /*case OrderExecution::templateId():
                //mOrderExecutions.emplace_back(parseOrderExecution());
                break;
            case OrderBookSnapshot::templateId():
                //mOrderBookSnapshots.emplace_back(parseOrderBookSnapshot());
                break;*/
            default:
                mParsingOffset += sbeHeader.BlockLength();
        }
    }
    return true;
}
/*
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
}*/
} // namespace protocols