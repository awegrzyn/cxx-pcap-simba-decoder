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
        INSUFFICIENT_DATA,
        INVALID_FORMAT,
        UNSUPPORTED_PROTOCOL
    };

    Ethernet(const std::vector<std::byte>& data)
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
    const std::span<const std::byte> getDestMac() const {
        return mDestMac;
    }
private:
    const std::vector<std::byte>& mRecordData;
    std::span<const std::byte> mDestMac;
    std::span<const std::byte> mSourceMac;
    std::span<const std::byte> mPayload;
};

} // namespace protocols

#endif // ETHERNET_H