#ifndef PROTOCOLS_SIMBA_ORDEREXECUTION_H
#define PROTOCOLS_SIMBA_ORDEREXECUTION_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include "common.h"

namespace protocols {

// int64_t MDEntryID
// Decimal5NULL MDEntryPx
// int64_t MDEntrySize
// Decimal5 LastPx
// int64_t LastQty
// int64_t TradeID
// MDFlagsSet MDFlags
// uint64_t MDFlags2
// int32_t SecurityID
// uint32_t RptSeq
// MDUpdateAction MDUpdateAction_
// MDEntryType MDEntryType_
class OrderExecution {
public:
    static consteval std::size_t size() {
        return sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t) + sizeof(MDFlagsSet) + sizeof(uint64_t) + sizeof(int32_t) + sizeof(uint32_t) + sizeof(MDUpdateAction) + sizeof(MDEntryType);
    }
    static consteval uint16_t templateId() {
        return 16;
    }
    OrderExecution(const std::span<const std::byte>::iterator rawDataBegin) : mRawData(rawDataBegin, OrderExecution::size()) {
        assert(mRawData.size() == OrderExecution::size());
    }
    const int64_t MDEntryID() const { return *reinterpret_cast<const int64_t*>(mRawData.data()); }
    const Decimal5NULL MDEntryPx() const { return *reinterpret_cast<const Decimal5NULL*>(mRawData.data() + sizeof(int64_t)); }
    const int64_t MDEntrySize() const { return *reinterpret_cast<const int64_t*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size()); }
    const Decimal5 LastPx() const { return *reinterpret_cast<const Decimal5*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t)); }
    const int64_t LastQty() const { return *reinterpret_cast<const int64_t*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size()); }
    const int64_t TradeID() const { return *reinterpret_cast<const int64_t*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t)); }
    const MDFlagsSet MDFlags() const { return *reinterpret_cast<const MDFlagsSet*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t)); }
    const uint64_t MDFlags2() const { return *reinterpret_cast<const uint64_t*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t) + sizeof(MDFlagsSet)); }
    const int32_t SecurityID() const { return *reinterpret_cast<const int32_t*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t) + sizeof(MDFlagsSet) + sizeof(uint64_t)); }
    const uint32_t RptSeq() const { return *reinterpret_cast<const uint32_t*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t) + sizeof(MDFlagsSet) + sizeof(uint64_t) + sizeof(int32_t)); }
    const MDUpdateAction MDUpdateAction_() const { return *reinterpret_cast<const MDUpdateAction*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t) + sizeof(MDFlagsSet) + sizeof(uint64_t) + sizeof(int32_t) + sizeof(uint32_t)); }
    const MDEntryType MDEntryType_() const { return *reinterpret_cast<const MDEntryType*>(mRawData.data() + sizeof(int64_t) + Decimal5NULL::size() + sizeof(int64_t) + Decimal5::size() + sizeof(int64_t) + sizeof(int64_t) + sizeof(MDFlagsSet) + sizeof(uint64_t) + sizeof(int32_t) + sizeof(uint32_t) + sizeof(MDUpdateAction)); }
private:
    const std::span<const std::byte> mRawData;
};
} // namespace protocols
#endif // PROTOCOLS_SIMBA_ORDEREXECUTION_H