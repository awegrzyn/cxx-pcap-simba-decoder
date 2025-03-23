#ifndef PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H
#define PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <cstring>
#include "common.h"

namespace protocols {

// int32_t SecurityID;
// uint32_t LastMsgSeqNumProcessed;
// uint32_t RptSeq;
// uint32_t ExchangeTradingSessionID;
// GroupSize NoMDEntries;
// struct Entry {
//    int64_t MDEntryID;
//    uint64_t TransactTime;
//    Decimal5NULL MDEntryPx;
//    int64_t MDEntrySize;
//    int64_t TradeID;
//    MDFlagsSet MDFlags;
//    uint64_t MDFlags2;
//    MDEntryType MDEntryType_;
//};
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
