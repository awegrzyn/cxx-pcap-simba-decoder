#ifndef INCREMENTALPACKETHEADER_H
#define INCREMENTALPACKETHEADER_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>

// uint64_t TransactTime;
// uint32_t ExchangeTradingSessionID;
class IncrementalPacketHeader {
public:
    static consteval std::size_t size() {
        return sizeof(uint64_t) + sizeof(uint32_t);
    }
    static consteval uint32_t getExchangeTradingSessionIDNull() {
        return 4294967295;
    }

    IncrementalPacketHeader(const std::span<const std::byte>::iterator rawDataBegin) : mRawHeader(rawDataBegin, IncrementalPacketHeader::size()) {
        assert(rawHeader.size() < IncrementalPacketHeader::size());
    }
    const uint64_t TransactTime() const { return *reinterpret_cast<const uint64_t*>(mRawHeader.data());}
    const uint32_t ExchangeTradingSessionID() const { return *reinterpret_cast<const uint32_t*>(mRawHeader.data() + sizeof(uint64_t)); }
private:
    const std::span<const std::byte> mRawHeader;
};
#endif // INCREMENTALPACKETHEADER_H