#ifndef PROTOCOLS_UDP_H
#define PROTOCOLS_UDP_H

#include <cstdint>
#include <expected>
#include <span>

namespace protocols {

class Udp {
public:
    enum class Error {
        InsufficientData,
        InvalidLength
    };

    Udp(std::span<const std::byte> data) : mIpData(data) {}

    std::expected<size_t, Error> parse();

    // Getters
    uint16_t getSourcePort() const { return mSourcePort; }
    uint16_t getDestinationPort() const { return mDestPort; }
    std::span<const std::byte> getPayload() const { return mPayload; }

private:
    std::span<const std::byte> mIpData;
    std::span<const std::byte> mPayload;
    uint16_t mSourcePort;
    uint16_t mDestPort;
};

} // namespace protocols

#endif // PROTOCOLS_IPV4_H