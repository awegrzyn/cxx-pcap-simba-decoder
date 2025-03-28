#include "udp.h"
#include <arpa/inet.h>
#include <iostream>

namespace protocols {

std::expected<size_t, Udp::Error> Udp::parse() {
    constexpr size_t UDP_HEADER_SIZE = 8;
    
    if (mIpData.size() < UDP_HEADER_SIZE) {
        return std::unexpected(Error::InsufficientData);
    }

    mSourcePort = (std::to_integer<uint8_t>(mIpData[0]) << 8) | std::to_integer<uint8_t>(mIpData[1]);
    mDestPort = (std::to_integer<uint16_t>(mIpData[2]) << 8) | std::to_integer<uint16_t>(mIpData[3]);
    uint16_t length = (std::to_integer<uint16_t>(mIpData[4]) << 8) | std::to_integer<uint16_t>(mIpData[5]);
    
    if (length < UDP_HEADER_SIZE) {
        return std::unexpected(Error::InvalidLength);
    }

    if (mIpData.size() < length) {
        return std::unexpected(Error::InsufficientData);
    }

    mData = std::span(mIpData.begin() + UDP_HEADER_SIZE, mIpData.begin() + length);
    
    return length;
}

} // namespace protocols