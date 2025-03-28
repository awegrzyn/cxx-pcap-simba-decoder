/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef SIMBA_SPECTRA_H
#define SIMBA_SPECTRA_H

#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include <memory>
#include <expected>

// Use packed structs for binary protocol parsing
#pragma pack(push, 1)
#include "simba/OrderUpdate.h"
#include "simba/OrderExecution.h"
#include "simba/OrderBookSnapshot.h"

namespace protocols {

class SimbaSpectra {
public:
    static constexpr std::string_view ProtocolName = "Simba Spectra";
    static constexpr std::uint8_t VersionMajor = 5;
    static constexpr std::uint8_t VersionMinor = 0;
    static constexpr std::uint8_t VersionPatch = 0;

    enum class Error {
        None,
        InsufficientData,
        InvalidVersion,
        InvalidLength,
        InvalidChecksum,
        MalformedHeader,
        UnsupportedMessageType
    };

    /// Constructor
    /// \param udpData The raw UDP packet
    SimbaSpectra(std::span<const std::byte> udpData);

    /// Parses the UDP packet
    /// \return True if parsing was successful, false otherwise
    std::expected<bool, Error> parse();

    /// \return The order updates inside packet
    const std::vector<OrderUpdate>& getOrderUpdates() const noexcept { return mOrderUpdates; }

    /// \return The order executions inside packet
    const std::vector<OrderExecution>& getOrderExecutions() const noexcept { return mOrderExecutions; }

    /// \return The order book snapshots inside packet
    const std::vector<OrderBookSnapshot>& getOrderBookSnapshots() const noexcept { return mOrderBookSnapshots; }

    /// \return JSON representation of the packet
    std::string toJson() const;

private:
    /// Data offset to simplify parsing 
    mutable std::size_t mParsingOffset;

    /// Advances the parsing offset
    void advanceOffset(std::size_t size) { mParsingOffset += size; }

    /// The raw UDP packet data
    std::span<const std::byte> mUdpData;

    /// The order updates
    std::vector<OrderUpdate> mOrderUpdates;

    /// The order executions
    std::vector<OrderExecution> mOrderExecutions;

    /// The order book snapshots
    std::vector<OrderBookSnapshot> mOrderBookSnapshots;
};

} // namespace protocols

#pragma pack(pop)

#endif // SIMBA_SPECTRA_H