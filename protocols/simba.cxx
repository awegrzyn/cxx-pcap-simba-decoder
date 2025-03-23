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
            case OrderExecution::templateId():
                mOrderExecutions.emplace_back(mUdpData.begin() + mParsingOffset);
                advanceOffset(OrderExecution::size());
                break;
            case OrderBookSnapshot::templateId():
                mOrderBookSnapshots.emplace_back(mUdpData.begin() + mParsingOffset);
                advanceOffset(OrderBookSnapshot::size());
                break;
            default:
                mParsingOffset += sbeHeader.BlockLength();
        }
    }
    return true;
}
} // namespace protocols