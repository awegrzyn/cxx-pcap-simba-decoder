/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

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

    /// Constructor
    /// \param data The raw UDP packet
    Udp(std::span<const std::byte> data) : mIpData(data) {}

    /// Parses the UDP header and payload
    /// \return The length of the UDP packet on success, or an error on failure
    std::expected<size_t, Error> parse();

    /// \return The source port
    uint16_t getSourcePort() const { return mSourcePort; }

    /// \return The destination port
    uint16_t getDestinationPort() const { return mDestPort; }

    /// \return UDP payload 
    std::span<const std::byte> getPayload() const { return mData; }

private:
    /// The raw UDP packet data
    std::span<const std::byte> mIpData;

    /// The UDP payload data
    std::span<const std::byte> mData;

    /// The source port
    uint16_t mSourcePort;

    /// The destination port
    uint16_t mDestPort;
};

} // namespace protocols

#endif // PROTOCOLS_IPV4_H