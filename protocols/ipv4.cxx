#include "ipv4.h"
#include <stdexcept>

namespace protocols {

std::expected<size_t, Ipv4::Error> Ipv4::parse() {
    // Minimum IPv4 header size (without options): 20 bytes
    constexpr size_t MIN_HEADER_SIZE = 20;
    
    // Check if we have enough data
    if (mEthernetData.size() < MIN_HEADER_SIZE) {
        return std::unexpected(Error::InsufficientData);
    }

    // Get version and header length
    uint8_t versionAndIhl = std::to_integer<uint8_t>(mEthernetData[0]);
    uint8_t version = (versionAndIhl >> 4) & 0x0F;
    uint8_t headerLength = (versionAndIhl & 0x0F) * 4; // IHL is in 4-byte units

    // Verify it's IPv4
    if (version != 4) {
        return std::unexpected(Error::InvalidVersion);
    }

    // Verify header length is valid
    if (headerLength < MIN_HEADER_SIZE) {
        return std::unexpected(Error::InvalidHeaderLength);
    }

    // Check if we have enough data for the full header
    if (mEthernetData.size() < headerLength) {
        return std::unexpected(Error::InsufficientData);
    }

    // Total length is in network byte order
    uint16_t totalLength = std::to_integer<uint16_t>(mEthernetData[2]) << 8 | std::to_integer<uint16_t>(mEthernetData[3]);

    // Verify total length doesn't exceed packet size
    if (mEthernetData.size() < totalLength) {
        return std::unexpected(Error::InvalidTotalLength);
    }

    // Source and destination addresses
    mSourceIP = std::span(mEthernetData.data() + 12, 4);
    mDestIP = std::span(mEthernetData.data() + 16, 4);

    // Calculate payload offset and size
    mPayload = std::span(mEthernetData.begin() + headerLength, mEthernetData.begin() + totalLength);

    return totalLength;
}

} // namespace protocols