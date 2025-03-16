#include "ethernet.h"
#include <stdexcept>

namespace protocols {

std::expected<size_t, Ethernet::Error> Ethernet::parse() {
    // Minimum Ethernet frame size (without payload): 6 (dst MAC) + 6 (src MAC) + 2 (ethertype) = 14 bytes
    constexpr size_t MIN_FRAME_SIZE = 14;
    
    // Check if we have enough data
    if (mRecordData.size() < MIN_FRAME_SIZE) {
        return std::unexpected(Error::INSUFFICIENT_DATA);
    }

    // Parse destination MAC (6 bytes)
    mDestMac = std::span<const std::byte>(mRecordData.data(), 6);

    // Parse source MAC (6 bytes)
    mSourceMac = std::span<const std::byte>(mRecordData.data() + 6, 6);

    // Extract payload using vector constructor with iterators
    mPayload = std::span<const std::byte>(mRecordData.begin() + MIN_FRAME_SIZE, mRecordData.end());

    return MIN_FRAME_SIZE + mPayload.size();
}

} // namespace protocols