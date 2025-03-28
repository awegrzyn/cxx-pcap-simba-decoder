/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef PROTOCOLS_SIMBA_ORDEREXECUTION_H
#define PROTOCOLS_SIMBA_ORDEREXECUTION_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>
#include "common.h"

namespace protocols {

/// OrderExecution
/// Consists of 12 fields:
/// - MDEntryID: ID of the market data entry (int64_t)
/// - MDEntryPx: Price of the market data entry (Decimal5NULL)
/// - MDEntrySize: Size of the market data entry (int64_t)
/// - LastPx: Price of the last trade (Decimal5)
/// - LastQty: Quantity of the last trade (int64_t)
/// - TradeID: ID of the trade (int64_t)
/// - MDFlags: Flags indicating the type of market data entry (MDFlagsSet)
/// - MDFlags2: Additional flags (uint64_t)
/// - SecurityID: ID of the security (int32_t)
/// - RptSeq: Sequence number of the report (uint32_t)
/// - MDUpdateAction_: Action to be taken (MDUpdateAction)
/// - MDEntryType_: Type of the market data entry (MDEntryType)
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