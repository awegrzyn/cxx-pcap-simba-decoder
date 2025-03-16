#ifndef PROTOCOLS_IPV4_H
#define PROTOCOLS_IPV4_H

#include <cstdint>
#include <cstddef>  // For std::byte
#include <span>
#include <expected>
#include <vector>
#include <string>

namespace protocols {

class Ipv4 {
public:
    enum class Error {
        InsufficientData,
        InvalidVersion,
        InvalidHeaderLength,
        InvalidTotalLength,
        ChecksumError
    };

    // Constructor takes a span of raw packet data
    Ipv4(std::span<const std::byte> data) : mEthernetData(data) {}

    // Parse the IPv4 packet
    std::expected<size_t, Error> parse();

    // Raw access to addresses
    std::span<const std::byte> getSourceIp() const { return mSourceIP; }
    std::span<const std::byte> getDestIp() const { return mDestIP; }

    // Access to the payload
    std::span<const std::byte> payload() const { return mPayload; }

private:
    std::span<const std::byte> mEthernetData;

    // IP addresses are stored as spans into the original data
    std::span<const std::byte> mSourceIP;
    std::span<const std::byte> mDestIP;

    // Payload (data after the header)
    std::span<const std::byte> mPayload;
};

} // namespace protocols

#endif // PROTOCOLS_IPV4_H