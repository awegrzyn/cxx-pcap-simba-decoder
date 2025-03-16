#include "ethernet.h"
#include <stdexcept>

namespace protocols {

std::expected<size_t, Ethernet::Error> Ethernet::parse() {
    // Minimum Ethernet frame size (without payload): 6 (dst MAC) + 6 (src MAC) + 2 (ethertype) = 14 bytes
    constexpr size_t MIN_FRAME_SIZE = 14;
    
    // Check if we have enough data
    if (mRecordData.size() < MIN_FRAME_SIZE) {
        return std::unexpected(Error::InsufficientData);
    }

    mDestMac = std::span(mRecordData.data(), 6);
    mSourceMac = std::span(mRecordData.data() + 6, 6);
    mPayload = std::span(mRecordData.begin() + MIN_FRAME_SIZE, mRecordData.end());

    return MIN_FRAME_SIZE + mPayload.size();
}

} // namespace protocols