#ifndef ETHERNET_H
#define ETHERNET_H

#include "../pcap/record.h"
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <expected> // C++23 feature, might need a fallback implementation

namespace protocols {

/**
 * Class representing an Ethernet frame
 */
class Ethernet {
public:
    // Error types for Ethernet parsing
    enum class Error {
        INSUFFICIENT_DATA,
        INVALID_FORMAT,
        UNSUPPORTED_PROTOCOL
    };

    // Parse Ethernet frame from raw data
    std::expected<size_t, Ethernet::Error> parse(const pcap::Record& record);
    
    // Get payload data
    const std::vector<std::byte>& getPayload() const;

    // Verify CRC of the Ethernet frame
    bool verifyCrc(const std::vector<std::byte>& data) const;
    
private:
    std::array<std::byte, 6> mDestMac;
    std::array<std::byte, 6> mSourceMac;
    uint16_t mEtherType;
    std::vector<std::byte> mPayload;
};

} // namespace protocols

#endif // ETHERNET_H