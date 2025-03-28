/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H
#define PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cstring>
#include "common.h"

namespace protocols {

/// OrderBookSnapshot (16 bytes + size of all entries)
/// Consists of 5 fields:
/// - SecurityID: ID of the security (int32_t)
/// - LastMsgSeqNumProcessed: Sequence number of the last message processed (uint32_t)
/// - RptSeq: Sequence number of the report (uint32_t)
/// - ExchangeTradingSessionID: ID of the exchange trading session (uint32_t)
/// - NoMDEntries: Number of market data entries (GroupSize)
/// In addition it stores a variable number of entries, each consisting of 8 fields:
/// - MDEntryID: ID of the market data entry (int64_t)
/// - TransactTime: Time the transaction occurred (uint64_t)
/// - MDEntryPx: Price of the market data entry (Decimal5NULL)
/// - MDEntrySize: Size of the market data entry (int64_t)
/// - TradeID: ID of the trade (int64_t)
/// - MDFlags: Flags indicating the type of market data entry (MDFlagsSet)
/// - MDFlags2: Additional flags (uint64_t)
/// - MDEntryType_: Type of the market data entry (MDEntryType)
class OrderBookSnapshot {
public:
    struct Entry {
        int64_t MDEntryID;
        uint64_t TransactTime;
        Decimal5NULL MDEntryPx;
        int64_t MDEntrySize;
        int64_t TradeID;
        MDFlagsSet MDFlags;
        uint64_t MDFlags2;
        MDEntryType MDEntryType_;

        static consteval std::size_t size() {
            return sizeof(MDEntryID) + sizeof(TransactTime) + Decimal5NULL::size() + sizeof(MDEntrySize) + sizeof(TradeID) + sizeof(MDFlags) + sizeof(MDFlags2) + sizeof(MDEntryType);
        }
    };

    static consteval std::size_t size() {
        return sizeof(int32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + GroupSize::size();
    }
    static consteval uint16_t templateId() {
        return 17;
    }

    OrderBookSnapshot(const std::span<const std::byte>::iterator rawDataBegin) : mRawData(rawDataBegin, OrderBookSnapshot::size()) {
        assert(mRawData.size() == OrderBookSnapshot::size());
    }

    const int32_t SecurityID() const { return *reinterpret_cast<const int32_t*>(mRawData.data()); }
    const uint32_t LastMsgSeqNumProcessed() const { return *reinterpret_cast<const uint32_t*>(mRawData.data() + sizeof(int32_t)); }
    const uint32_t RptSeq() const { return *reinterpret_cast<const uint32_t*>(mRawData.data() + sizeof(int32_t) + sizeof(uint32_t)); }
    const uint32_t ExchangeTradingSessionID() const { return *reinterpret_cast<const uint32_t*>(mRawData.data() + sizeof(int32_t) + sizeof(uint32_t) + sizeof(uint32_t)); }
    const GroupSize NoMDEntries() const { return *reinterpret_cast<const GroupSize*>(mRawData.data() + sizeof(int32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)); }
    const std::vector<Entry> getEntries() const {
        std::vector<Entry> entries(NoMDEntries().numInGroup);
        std::size_t entryOffset = 0;
        const std::span<const std::byte> entriesData = getRawEntries();
        for (Entry& entry : entries) {
            std::memcpy(&entry, entriesData.data() + entryOffset, Entry::size());
            entryOffset += Entry::size();
        }
        return entries;
    }
private:
    const std::span<const std::byte> mRawData;
    const std::span<const std::byte> getRawEntries() const {
        return std::span<const std::byte>(mRawData.data() + sizeof(int32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + GroupSize::size(), mRawData.size() - (sizeof(int32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + GroupSize::size()));
    }
};
} // namespace protocols
#endif // PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H
