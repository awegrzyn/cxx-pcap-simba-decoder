#include "simba.h"
#include <algorithm>
#include <arpa/inet.h>
#include <cstring>

namespace protocols {

std::expected<size_t, SimbaSpectra::Error> SimbaSpectra::parse() {
    // SIMBA SPECTRA header structure:
    // - Magic bytes "SPCTR" (5 bytes)
    // - Version string "5.0.0" (5 bytes)
    // - Message type (1 byte)
    // - Body length (2 bytes, network byte order)
    // - Sequence number (4 bytes, network byte order)
    // - Timestamp (8 bytes, network byte order)
    // - Checksum (2 bytes, network byte order)
    // Total header size: 27 bytes

    constexpr size_t MAGIC_SIZE = 5;
    constexpr size_t VERSION_SIZE = 5;
    constexpr size_t MAGIC_AND_VERSION_SIZE = MAGIC_SIZE + VERSION_SIZE;
    constexpr size_t HEADER_SIZE = MAGIC_AND_VERSION_SIZE + 1 + 2 + 4 + 8 + 2;
    
    // Check if we have enough data for the header
    if (mUdpData.size() < HEADER_SIZE) {
        return std::unexpected(Error::InsufficientData);
    }

    // Verify magic bytes "SPCTR"
    const char* expectedMagic = "SPCTR";
    for (size_t i = 0; i < MAGIC_SIZE; i++) {
        if (std::to_integer<char>(mUdpData[i]) != expectedMagic[i]) {
            return std::unexpected(Error::MalformedHeader);
        }
    }

    // Extract version string
    char versionStr[VERSION_SIZE + 1] = {0}; // +1 for null terminator
    for (size_t i = 0; i < VERSION_SIZE; i++) {
        versionStr[i] = std::to_integer<char>(mUdpData[MAGIC_SIZE + i]);
    }
    mVersion = versionStr;

    // Verify it's version 5.0.0
    if (mVersion != "5.0.0") {
        return std::unexpected(Error::InvalidVersion);
    }

    // Extract message type
    mMessageType = static_cast<MessageType>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE]));
    
    
    // Extract body length (2 bytes, network byte order)
    mBodyLength = (std::to_integer<uint16_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 1]) << 8) |
                   std::to_integer<uint16_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 2]);
    
    // Extract sequence number (4 bytes, network byte order)
    mSequenceNumber = (std::to_integer<uint32_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 3]) << 24) |
                      (std::to_integer<uint32_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 4]) << 16) |
                      (std::to_integer<uint32_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 5]) << 8) |
                       std::to_integer<uint32_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 6]);
    
    // Extract timestamp (8 bytes, network byte order)
    mTimestamp = (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 7])) << 56) |
                 (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 8])) << 48) |
                 (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 9])) << 40) |
                 (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 10])) << 32) |
                 (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 11])) << 24) |
                 (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 12])) << 16) |
                 (static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 13])) << 8) |
                  static_cast<uint64_t>(std::to_integer<uint8_t>(mUdpData[MAGIC_AND_VERSION_SIZE + 14]));
    
    // Verify we have enough data for the entire message
    size_t totalLength = HEADER_SIZE + mBodyLength;
    if (mUdpData.size() < totalLength) {
        return std::unexpected(Error::InsufficientData);
    }
    
    // Validate checksum
    if (!validateChecksum()) {
        return std::unexpected(Error::InvalidChecksum);
    }
    
    // Extract payload
    mPayload = std::span(mUdpData.begin() + HEADER_SIZE, mUdpData.begin() + totalLength);
    
    return totalLength;
}

bool SimbaSpectra::validateChecksum() const {
    // Get the checksum from the header
    uint16_t receivedChecksum = (std::to_integer<uint16_t>(mUdpData[25]) << 8) |
                                 std::to_integer<uint16_t>(mUdpData[26]);
    
    // Calculate checksum over header (excluding the checksum field itself) and payload
    // For simplicity, assume the checksum is a simple sum of bytes
    std::span<const std::byte> checksumData(mUdpData.data(), 25); // Header without checksum bytes
    uint16_t calculatedChecksum = calculateChecksum(checksumData);
    
    return (receivedChecksum == calculatedChecksum);
}

uint16_t SimbaSpectra::calculateChecksum(std::span<const std::byte> data) const {
    // Simple implementation - in a real application, you would implement 
    // the actual checksum algorithm used by SIMBA SPECTRA
    uint16_t sum = 0;
    for (const auto& byte : data) {
        sum += std::to_integer<uint16_t>(byte);
    }
    return sum;
}
std::string SimbaSpectra::toJson() const {
    std::string json = "{";
    json += "\"version\": \"" + mVersion + "\",";
    json += "\"messageType\": " + std::to_string(static_cast<int>(mMessageType)) + ",";
    json += "\"sequenceNumber\": " + std::to_string(mSequenceNumber) + ",";
    json += "\"timestamp\": " + std::to_string(mTimestamp) + ",";
    json += "\"payload\": \"";
    for (const auto& byte : mPayload) {
        json += std::to_integer<char>(byte);
    }
    json += "\"}";
    return json;
}

} // namespace protocols