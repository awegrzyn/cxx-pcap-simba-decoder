/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef INCREMENTALPACKETHEADER_H
#define INCREMENTALPACKETHEADER_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>

/// Incremental Packet Header (12 bytes)
/// Consists of 2 fields:
/// - TransactTime: Time the transaction occurred (uint64_t)
/// - ExchangeTradingSessionID: ID of the exchange trading session (uint32_t)
class IncrementalPacketHeader {
public:
    /// \return Size of the header in bytes
    static consteval std::size_t size() {
        return sizeof(uint64_t) + sizeof(uint32_t);
    }
    /// \return Null value for ExchangeTradingSessionID
    static consteval uint32_t getExchangeTradingSessionIDNull() {
        return 4294967295;
    }

    /// Constructor
    /// \param rawDataBegin Pointer to the beginning of the raw data
    IncrementalPacketHeader(const std::span<const std::byte>::iterator rawDataBegin) : mRawHeader(rawDataBegin, IncrementalPacketHeader::size()) {
        assert(rawHeader.size() < IncrementalPacketHeader::size());
    }
    const uint64_t TransactTime() const { return *reinterpret_cast<const uint64_t*>(mRawHeader.data());}
    const uint32_t ExchangeTradingSessionID() const { return *reinterpret_cast<const uint32_t*>(mRawHeader.data() + sizeof(uint64_t)); }
private:
    /// The raw header data
    const std::span<const std::byte> mRawHeader;
};
#endif // INCREMENTALPACKETHEADER_H
