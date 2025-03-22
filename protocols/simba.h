#pragma once

#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <vector>

namespace protocols {

class SimbaSpectra {
public:
    enum class Error {
        InsufficientData,
        InvalidVersion,
        InvalidLength,
        InvalidChecksum,
        MalformedHeader
    };

    enum class MessageType : uint8_t {
        MarketData = 0x01,
        Trade = 0x02,
        OrderBook = 0x03,
        Heartbeat = 0x04,
        OrderUpdate = 0x05,
        OrderExecution = 0x06,
        OrderBookSnapshot = 0x07,
        OrderBookDelta = 0x08,
        OrderBookClear = 0x09,
        OrderBookReject = 0x0A,
        OrderBookAck = 0x0B,
        OrderBookRequest = 0x0C,
        OrderBookCancel = 0x0D,
        OrderBookCancelReject = 0x0E,
        OrderBookCancelAck = 0x0F,
        OrderBookCancelRequest = 0x10,
        OrderBookModify = 0x11
    };

    // Constructor takes UDP payload
    SimbaSpectra(std::span<const std::byte> udpData) : mUdpData(udpData) {}

    // Parse SIMBA SPECTRA message
    std::expected<size_t, Error> parse();

    // Getters
    std::string getVersion() const { return mVersion; }
    MessageType getMessageType() const { return mMessageType; }
    uint32_t getSequenceNumber() const { return mSequenceNumber; }
    uint64_t getTimestamp() const { return mTimestamp; }
    std::span<const std::byte> getPayload() const { return mPayload; }
    std::string toJson() const;

private:
    std::span<const std::byte> mUdpData;
    std::span<const std::byte> mPayload;
    
    // Header fields
    std::string mVersion;
    MessageType mMessageType = MessageType::Heartbeat;
    uint32_t mSequenceNumber = 0;
    uint64_t mTimestamp = 0;
    uint16_t mBodyLength = 0;
    
    // Helper methods
    bool validateChecksum() const;
    uint16_t calculateChecksum(std::span<const std::byte> data) const;
};

} // namespace protocols