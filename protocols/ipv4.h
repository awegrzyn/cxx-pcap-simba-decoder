/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef PROTOCOLS_IPV4_H
#define PROTOCOLS_IPV4_H

#include <cstdint>
#include <cstddef>
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

    /// Constructor
    /// \param data The raw IPv4 packet
    Ipv4(std::span<const std::byte> data) : mEthernetData(data) {}

    /// Parses the IPv4 header and payload
    /// \return The length of the IPv4 packet on success, or an error on failure
    std::expected<size_t, Error> parse();

    /// \return IPv4 source address
    std::span<const std::byte> getSourceIp() const { return mSourceIP; }

    /// \return IPv4 destination address
    std::span<const std::byte> getDestIp() const { return mDestIP; }

    /// \return IPv4 payload
    std::span<const std::byte> payload() const { return mPayload; }

private:
    /// The raw IPv4 packet data
    std::span<const std::byte> mEthernetData;

    /// IPv4 source address in byte representation
    std::span<const std::byte> mSourceIP;

    /// IPv4 destination address in byte representation
    std::span<const std::byte> mDestIP;

    /// IPv4 payload data
    std::span<const std::byte> mPayload;
};

} // namespace protocols

#endif // PROTOCOLS_IPV4_H