#pragma once

#include <cstddef>  // For std::byte
#include <cstdint>
#include <vector>

namespace pcap {

// Record header structure (16 bytes)
struct RecordHeader {
    uint32_t ts_sec;         // Timestamp seconds
    uint32_t ts_subsec;      // Timestamp microseconds (or nanoseconds)
    uint32_t incl_len;       // Number of octets of packet saved in file
    uint32_t orig_len;       // Actual length of packet
};
static_assert(sizeof(RecordHeader) == 16, "RecordHeader size mismatch");

// Complete record with header and data
class Record {
public:
    RecordHeader header;
    Record(bool isNanosecond) : mIsNanosecond(isNanosecond) {}
    uint64_t timestamp() const {
        uint64_t subsecond_divisor = mIsNanosecond ? 1e9 : 1e6;
        return static_cast<uint64_t>(header.ts_sec + (header.ts_subsec / subsecond_divisor));
    }
    void resizeData(size_t size) {
        mData.resize(size);
    }
    std::byte* getDataPointer() {
        return mData.data();
    }
private:
    bool mIsNanosecond;
    std::vector<std::byte> mData;
};

} // namespace pcap
