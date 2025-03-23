#include "udp.h"
#include <arpa/inet.h>
#include <iostream>

namespace protocols {

std::expected<size_t, Udp::Error> Udp::parse() {
    // UDP header size is 8 bytes
    constexpr size_t UDP_HEADER_SIZE = 8;
    
    // Check if we have enough data for the UDP header
    if (mIpData.size() < UDP_HEADER_SIZE) {
        return std::unexpected(Error::InsufficientData);
    }

    // Extract header fields (all are in network byte order)
    mSourcePort = (std::to_integer<uint8_t>(mIpData[0]) << 8) | std::to_integer<uint8_t>(mIpData[1]);
    mDestPort = (std::to_integer<uint16_t>(mIpData[2]) << 8) | std::to_integer<uint16_t>(mIpData[3]);
    uint16_t length = (std::to_integer<uint16_t>(mIpData[4]) << 8) | std::to_integer<uint16_t>(mIpData[5]);
    
    // Verify length is valid (must be at least header size)
    if (length < UDP_HEADER_SIZE) {
        return std::unexpected(Error::InvalidLength);
    }

    // Verify we have enough data for the entire UDP packet
    if (mIpData.size() < length) {
        return std::unexpected(Error::InsufficientData);
    }

    // Set the payload (data after the header)
    mData = std::span(mIpData.begin() + UDP_HEADER_SIZE, mIpData.begin() + length);
    
    return length;
}

} // namespace protocols