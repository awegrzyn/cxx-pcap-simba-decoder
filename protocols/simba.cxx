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

    // Start with opening the main JSON object
    json << "{";

    // Order updates
    json << R"("OrderUpdates":[)";
    for (size_t i = 0; i < mOrderUpdates.size(); i++) {
        const auto& orderUpdate = mOrderUpdates.at(i);
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
    json << "]";

    // Order executions
    json << R"(,"OrderExecutions":[)";
    for (size_t i = 0; i < mOrderExecutions.size(); i++) {
        const auto& orderExecution = mOrderExecutions.at(i);
        json << R"({"MdEntryId":)" << orderExecution.MDEntryID()
             << R"(,"MdEntryPx":)" << orderExecution.MDEntryPx()()
             << R"(,"MdEntrySize":)" << orderExecution.MDEntrySize()
             << R"(,"LastPx":)" << orderExecution.LastPx()()
             << R"(,"LastQty":)" << orderExecution.LastQty()
             << R"(,"TradeId":)" << orderExecution.TradeID()
             << R"(,"MdFlags":)" << static_cast<uint64_t>(orderExecution.MDFlags())
             << R"(,"MdFlags2":)" << orderExecution.MDFlags2()
             << R"(,"SecurityId":)" << orderExecution.SecurityID()
             << R"(,"RptSeq":)" << orderExecution.RptSeq()
             << R"(,"MdUpdateAction":)" << static_cast<int>(orderExecution.MDUpdateAction_())
             << R"(,"MdEntryType":")" << static_cast<char>(orderExecution.MDEntryType_()) << "\""
             << "}";
        if (i < mOrderExecutions.size() - 1) {
            json << ",";
        }
    }
    json << "]";

    // Order book snapshots
    json << R"(,"OrderBookSnapshots":[)";
    for (size_t i = 0; i < mOrderBookSnapshots.size(); i++) {
        const auto& orderBookSnapshot = mOrderBookSnapshots.at(i);
        
        // Start snapshot object
        json << "{";
        
        // Add snapshot fields
        json << R"("SecurityId":)" << orderBookSnapshot.SecurityID()
             << R"(,"LastMsgSeqNumProcessed":)" << orderBookSnapshot.LastMsgSeqNumProcessed()
             << R"(,"RptSeq":)" << orderBookSnapshot.RptSeq()
             << R"(,"ExchangeTradingSessionId":)" << orderBookSnapshot.ExchangeTradingSessionID()
             << R"(,"NoMDEntries":{"BlockLength":)" << orderBookSnapshot.NoMDEntries().blockLength
             << R"(,"NumInGroup":)" << static_cast<int>(orderBookSnapshot.NoMDEntries().numInGroup) << "}";
        
        // Add entries only if there are any
        const auto entries = orderBookSnapshot.getEntries();
        if (!entries.empty()) {
            json << R"(,"Entries":[)";
            for (size_t j = 0; j < entries.size(); j++) {
                const auto& entry = entries.at(j);
                json << R"({"MdEntryId":)" << entry.MDEntryID
                     << R"(,"TransactTime":)" << entry.TransactTime
                     << R"(,"MdEntryPx":)" << entry.MDEntryPx()
                     << R"(,"MdEntrySize":)" << entry.MDEntrySize
                     << R"(,"TradeId":)" << entry.TradeID
                     << R"(,"MdFlags":)" << static_cast<uint64_t>(entry.MDFlags)
                     << R"(,"MdFlags2":)" << entry.MDFlags2
                     << R"(,"MdEntryType":")" << static_cast<char>(entry.MDEntryType_) << "\""
                     << "}";
                if (j < entries.size() - 1) {
                    json << ",";
                }
            }
            json << "]";
        }
        
        // Close snapshot object
        json << "}";
        
        if (i < mOrderBookSnapshots.size() - 1) {
            json << ",";
        }
    }
    json << "]";
    
    // Close the main JSON object
    json << "}";
    return json.str();
}
} // namespace protocols