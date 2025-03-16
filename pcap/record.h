#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <span>

namespace pcap {

// Record header structure (16 bytes)
struct RecordHeader {
    uint32_t ts_sec;         // Timestamp seconds
    uint32_t ts_subsec;      // Timestamp microseconds (or nanoseconds)
    uint32_t incl_len;       // Number of octets of packet saved in file
    uint32_t orig_len;       // Actual length of packet
};

// Complete record with header and data
class Record {
public:
    friend class Parser;
    RecordHeader header;
    Record(bool isNanosecond) : mIsNanosecond(isNanosecond) {}
    uint64_t timestamp() const {
        uint64_t subsecond_divisor = mIsNanosecond ? 1e9 : 1e6;
        return static_cast<uint64_t>(header.ts_sec + (header.ts_subsec / subsecond_divisor));
    }
    const std::span<const std::byte> getData() const {
        return std::span<const std::byte>(mData.begin(), mData.end());
    }
private:
    void resizeData(size_t size) {
        mData.resize(size);
    }
    std::byte* getDataPointer() {
        return mData.data();
    }
    bool mIsNanosecond;
    std::vector<std::byte> mData;
};

} // namespace pcap
