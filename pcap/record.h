/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef PCAP_RECORD_H
#define PCAP_RECORD_H

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

    /// Constructor
    /// \param header The record header
    Record(RecordHeader header) : mHeader(header) {}

    /// \return Raw data of the record
    const std::span<const std::byte> getData() const {
        return std::span<const std::byte>(mData.begin(), mData.end());
    }

    /// \return Length of the record data 
    const RecordHeader& getHeader() const {
        return mHeader;
    }
private:
    /// Resize raw data buffer
    /// \param size New size of the data buffer
    void resizeData(size_t size) {
        mData.resize(size);
    }

    /// \return Pointer to the raw data buffer
    std::byte* getDataPointer() {
        return mData.data();
    }

    /// Record header
    RecordHeader mHeader;

    /// Raw data of the record
    std::vector<std::byte> mData;
};

} // namespace pcap
#endif // PCAP_RECORD_H
