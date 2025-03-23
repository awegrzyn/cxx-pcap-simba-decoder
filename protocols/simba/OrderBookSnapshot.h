#ifndef PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H
#define PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>
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
private:
    const std::span<const std::byte> mRawData;
};
} // namespace protocols
#endif // PROTOCOLS_SIMBA_ORDERBOOKSNAPSHOT_H
