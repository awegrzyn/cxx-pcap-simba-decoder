/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef ETHERNET_H
#define ETHERNET_H

#include <cstdint>
#include <vector>
#include <expected>
#include <span>

namespace protocols {

class Ethernet {
public:
    enum class Error {
        InsufficientData,
        InvalidFormat
    };
    /// Constructor
    /// \param data The raw Ethernet frame
    Ethernet(const std::span<const std::byte> data)
        : mRecordData(data) {}

    /// Parses the Ethernet frame
    /// \return The length of the Ethernet frame on success, or an error on failure
    std::expected<size_t, Ethernet::Error> parse();
    
    /// \return Returns Ethernet payload
    const std::span<const std::byte> getPayload() const {
        return mPayload;
    }

    /// \return Returns Ethernet source MAC address
    const std::span<const std::byte> getSourceMac() const {
        return mSourceMac;
    }
    /// \return Returns Ethernet destination MAC address
    const std::span<const std::byte> getDestMac() const {
        return mDestMac;
    }
private:
    /// Raw record data
    const std::span<const std::byte> mRecordData;

    /// Destination MAC address
    std::span<const std::byte> mDestMac;

    /// Source MAC address
    std::span<const std::byte> mSourceMac;

    /// Payload data
    std::span<const std::byte> mPayload;
};

} // namespace protocols

#endif // ETHERNET_H
