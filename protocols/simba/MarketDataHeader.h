/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef MARKETDATAHEADER_H
#define MARKETDATAHEADER_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>

/// Market Data Packet Header (16 bytes)
/// Consists of 4 fields:
/// - MsgSeqNum: Sequence number of the message (uint32_t)
/// - MsgSize: Size of the message excluding the header (uint16_t)
/// - MsgFlags: Flags indicating the type of message (uint16_t)
/// - SendingTime: Time the message was sent (uint64_t)
class MarketDataHeader {
public:
    /// \return Size of the header in bytes
    static consteval std::size_t size() {
        return sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint64_t);
    }
    /// Constructor
    /// \param rawDataBegin Pointer to the beginning of the raw data
    MarketDataHeader(const std::span<const std::byte>::iterator rawDataBegin) : mRawHeader(rawDataBegin, MarketDataHeader::size()) {
        assert(rawHeader.size() < MarketDataHeader::size());
    }
    const uint32_t MsgSeqNum() const { return *reinterpret_cast<const uint32_t*>(mRawHeader.data());}
    const uint16_t MsgSize() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint32_t)); }
    const uint16_t MsgFlags() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint32_t) + sizeof(uint16_t)); }
    const uint64_t SendingTime() const { return *reinterpret_cast<const uint64_t*>(mRawHeader.data() + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint16_t)); }
    bool IsFragmented() const { return MsgFlags() & 0x1; }
    bool IsStartOfSnapshot() const { return MsgFlags() & 0x2; }
    bool IsEndOfSnapshot() const { return MsgFlags() & 0x4; }
    bool IsIncremental() const { return MsgFlags() & 0x8; }
    bool IsPossDupFlag() const { return MsgFlags() & 0x10; }
private:
    /// The raw header data
    const std::span<const std::byte> mRawHeader;
};

#endif // MARKETDATAHEADER_H