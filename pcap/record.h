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
    Record(RecordHeader header) : mHeader(header) {}

    const std::span<const std::byte> getData() const {
        return std::span<const std::byte>(mData.begin(), mData.end());
    }
    const RecordHeader& getHeader() const {
        return mHeader;
    }
private:
    void resizeData(size_t size) {
        mData.resize(size);
    }
    std::byte* getDataPointer() {
        return mData.data();
    }
    RecordHeader mHeader;
    std::vector<std::byte> mData;
};

} // namespace pcap
