#pragma once

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

    SimbaSpectra(std::span<const std::byte> udpData);
    std::expected<bool, Error> parse();
    const std::vector<OrderUpdate>& getOrderUpdates() const noexcept { return mOrderUpdates; }
    const std::vector<OrderExecution>& getOrderExecutions() const noexcept { return mOrderExecutions; }
    const std::vector<OrderBookSnapshot>& getOrderBookSnapshots() const noexcept { return mOrderBookSnapshots; }

private:
    mutable std::size_t mParsingOffset;
    void advanceOffset(std::size_t size) { mParsingOffset += size; }
    std::span<const std::byte> mUdpData;
    std::vector<OrderUpdate> mOrderUpdates;
    std::vector<OrderExecution> mOrderExecutions;
    std::vector<OrderBookSnapshot> mOrderBookSnapshots;
};

} // namespace protocols

#pragma pack(pop)