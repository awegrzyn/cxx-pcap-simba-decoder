#include "ethernet.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace protocols {

std::expected<size_t, Ethernet::Error> Ethernet::parse(const std::vector<uint8_t>& data, size_t offset) {
    // Minimum Ethernet frame size (without payload): 6 (dst MAC) + 6 (src MAC) + 2 (ethertype) = 14 bytes
    constexpr size_t MIN_FRAME_SIZE = 14;
    
    // Check if we have enough data
    if (data.size() < offset + MIN_FRAME_SIZE) {
        return std::unexpected(Error::INSUFFICIENT_DATA);
    }

    try {
        // Parse destination MAC (6 bytes)
        for (size_t i = 0; i < 6; ++i) {
            mDestMac.at(i) = data.at(offset + i);
        }

        // Parse source MAC (6 bytes)
        for (size_t i = 0; i < 6; ++i) {
            mSourceMac.at(i) = data.at(offset + 6 + i);
        }

        // Parse EtherType (2 bytes)
        mEtherType = (static_cast<uint16_t>(data.at(offset + 12)) << 8) | data.at(offset + 13);

        // Extract payload
        mPayload.clear();
        for (size_t i = offset + MIN_FRAME_SIZE; i < data.size(); ++i) {
            mPayload.push_back(data.at(i));
        }

        return MIN_FRAME_SIZE + mPayload.size();
    } catch (const std::out_of_range& e) {
        // This shouldn't happen given our size check, but just in case
        return std::unexpected(Error::INSUFFICIENT_DATA);
    }
}

const std::vector<uint8_t>& Ethernet::getPayload() const {
    return mPayload;
}

} // namespace protocols