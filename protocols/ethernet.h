#ifndef ETHERNET_H
#define ETHERNET_H

#include <cstdint>
#include <vector>
#include <expected>
#include <span>

namespace protocols {

/**
 * Class representing an Ethernet frame
 */
class Ethernet {
public:
    // Error types for Ethernet parsing
    enum class Error {
        InsufficientData,
        InvalidFormat
    };
    // Constructor
    Ethernet(const std::span<const std::byte> data)
        : mRecordData(data) {}

    // Parse Ethernet frame from raw data
    std::expected<size_t, Ethernet::Error> parse();
    
    // Get payload data
    const std::span<const std::byte> getPayload() const {
        return mPayload;
    }

    // Get destination MAC address as string
    const std::span<const std::byte> getSourceMac() const {
        return mSourceMac;
    }
    // Get source MAC address as string
    const std::span<const std::byte> getDestMac() const {
        return mDestMac;
    }
private:
    // Raw record data
    const std::span<const std::byte> mRecordData;

    // Destination MAC address
    std::span<const std::byte> mDestMac;

    // Source MAC address
    std::span<const std::byte> mSourceMac;

    // Payload data
    std::span<const std::byte> mPayload;
};

} // namespace protocols

#endif // ETHERNET_H