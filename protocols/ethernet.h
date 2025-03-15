#ifndef ETHERNET_H
#define ETHERNET_H

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
    // Common EtherType values
    enum class EtherType : uint16_t {
        IPV4 = 0x0800,
        ARP = 0x0806,
        WAKE_ON_LAN = 0x0842,
        VLAN = 0x8100,
        IPV6 = 0x86DD,
        UNKNOWN = 0xFFFF
    };
    
    // Error types for Ethernet parsing
    enum class Error {
        INSUFFICIENT_DATA,
        INVALID_FORMAT,
        UNSUPPORTED_PROTOCOL
    };

    /**
     * Parse Ethernet frame from raw data
     * @param data Raw packet data from PCAP
     * @param offset Offset to start parsing
     * @return Expected with consumed bytes or an error
     */
    std::expected<size_t, Error> parse(const std::vector<uint8_t>& data, size_t offset = 0);
    
    /**
     * Get payload data
     * @return Payload bytes
     */
    const std::vector<uint8_t>& getPayload() const;
    
private:
    std::array<uint8_t, 6> mDestMac;
    std::array<uint8_t, 6> mSourceMac;
    uint16_t mEtherType;
    std::vector<uint8_t> mPayload;
};

} // namespace protocols

#endif // ETHERNET_H