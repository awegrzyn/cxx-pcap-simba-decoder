#include "ethernet.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace protocols {

std::expected<size_t, Ethernet::Error> Ethernet::parse(const pcap::Record& record) {
    // Minimum Ethernet frame size (without payload): 6 (dst MAC) + 6 (src MAC) + 2 (ethertype) = 14 bytes
    constexpr size_t MIN_FRAME_SIZE = 14;
    
    // Get data from the pcap record
    const std::vector<std::byte>& data = record.getData();
    
    // Check if we have enough data
    if (data.size() < MIN_FRAME_SIZE) {
        return std::unexpected(Error::INSUFFICIENT_DATA);
    }

    // Check if the Ethernet frame has a valid CRC
    //if (!verifyCrc(data)) {
    //    return std::unexpected(Error::INVALID_FORMAT);
    //}

    // Parse destination MAC (6 bytes)
    std::copy(data.begin(), data.begin() + 6, mDestMac.begin());

    // Parse source MAC (6 bytes)
    std::copy(data.begin() + 6, data.begin() + 12, mSourceMac.begin());

    // Parse EtherType (2 bytes) - network byte order (big-endian)
    //mEtherType = (static_cast<uint16_t>(data.at(12)) << 8) | data.at(13);

    // Extract payload using vector constructor with iterators
    mPayload.assign(data.begin() + MIN_FRAME_SIZE, data.end());

    return MIN_FRAME_SIZE + mPayload.size();
}

const std::vector<std::byte>& Ethernet::getPayload() const {
    return mPayload;
}
/*
bool Ethernet::verifyCrc(const std::vector<std::byte>& data) const {
    // Ethernet frame CRC is 4 bytes at the end of the frame
    constexpr size_t CRC_SIZE = 4;
    
    // The CRC-32 polynomial used in Ethernet
    constexpr uint32_t CRC32_POLYNOMIAL = 0xEDB88320;
    
    // Check if there's enough data for a valid Ethernet frame with CRC
    if (data.size() < CRC_SIZE) {
        return false;
    }
    
    // Calculate CRC32 on the entire frame except the CRC field itself
    const size_t dataLength = data.size() - CRC_SIZE;
    uint32_t calculatedCrc = 0xFFFFFFFF; // Initial value for CRC calculation
    
    // Process each byte in the frame (excluding CRC bytes)
    for (size_t i = 0; i < dataLength; ++i) {
        calculatedCrc ^= std::to_integer<uint8_t>(data[i]);
        for (int j = 0; j < 8; ++j) {
            calculatedCrc = (calculatedCrc >> 1) ^ ((calculatedCrc & 1) ? CRC32_POLYNOMIAL : 0);
        }
    }
    calculatedCrc = ~calculatedCrc; // Final XOR value
    
    // Extract the CRC from the frame (last 4 bytes)
    uint32_t frameCrc = 
        (std::to_integer<uint32_t>(data[dataLength]) << 0) |
        (std::to_integer<uint32_t>(data[dataLength + 1]) << 8) |
        (std::to_integer<uint32_t>(data[dataLength + 2]) << 16) |
        (std::to_integer<uint32_t>(data[dataLength + 3]) << 24);
    
    return calculatedCrc == frameCrc;
}*/

} // namespace protocols