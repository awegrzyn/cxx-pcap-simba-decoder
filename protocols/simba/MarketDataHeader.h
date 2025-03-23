#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>

// uint32_t MsgSeqNum;
// uint16_t MsgSize;
// uint16_t MsgFlags;
// uint64_t SendingTime;
class MarketDataHeader {
public:
    static consteval std::size_t size() {
        return sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint64_t);
    }
    MarketDataHeader(const std::span<const std::byte>::iterator rawDataBegin) : mRawHeader(rawDataBegin, MarketDataHeader::size()) {
        assert(rawHeader.size() < MarketDataHeader::size());
    }
    bool IsFragmented() const { return MsgFlags() & 0x1; }
    bool IsStartOfSnapshot() const { return MsgFlags() & 0x2; }
    bool IsEndOfSnapshot() const { return MsgFlags() & 0x4; }
    bool IsIncremental() const { return MsgFlags() & 0x8; }
    bool IsPossDupFlag() const { return MsgFlags() & 0x10; }
private:
    const std::span<const std::byte> mRawHeader;
    const uint32_t MsgSeqNum() const { return *reinterpret_cast<const uint32_t*>(mRawHeader.data());}
    const uint16_t MsgSize() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint32_t)); }
    const uint16_t MsgFlags() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint32_t) + sizeof(uint16_t)); }
    const uint64_t SendingTime() const { return *reinterpret_cast<const uint64_t*>(mRawHeader.data() + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint16_t)); }
};