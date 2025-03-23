#include "simba.h"
#include <algorithm>
#include <cstring>
#include <sstream>

#include "simba/MarketDataHeader.h"
#include "simba/IncrementalPacketHeader.h"
#include "simba/SbeMessageHeader.h"
#include <format>

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

std::string SimbaSpectra::toJson() const {
    std::ostringstream json;
    json << R"({"OrderUpdates":[)";

    for (size_t i = 0; i < mOrderUpdates.size(); ++i) {
        const auto& orderUpdate = mOrderUpdates[i];

        json << R"({"MdEntryId":)" << orderUpdate.MDEntryID()
             << R"(,"MdEntryPx":)" << orderUpdate.MDEntryPx()()
             << R"(,"MdEntrySize":)" << orderUpdate.MDEntrySize()
             << R"(,"MdFlags":)" << static_cast<uint64_t>(orderUpdate.MDFlags())
             << R"(,"MdFlags2":)" << orderUpdate.MDFlags2()
             << R"(,"SecurityId":)" << orderUpdate.SecurityID()
             << R"(,"RptSeq":)" << orderUpdate.RptSeq()
             << R"(,"MdUpdateAction":)" << static_cast<int>(orderUpdate.MDUpdateAction_())
             << R"(,"MdEntryType":")" << static_cast<char>(orderUpdate.MDEntryType_()) << "\""
             << "}";
        if (i < mOrderUpdates.size() - 1) {
            json << ",";
        }
    }

    json << "]}";
    return json.str();
}
} // namespace protocols