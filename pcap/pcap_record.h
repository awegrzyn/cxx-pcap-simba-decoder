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
    std::vector<std::byte> data;
    
    // Timestamp in seconds (with fractional part)
    double timestamp(bool is_nanosecond) const;
};

} // namespace pcap
