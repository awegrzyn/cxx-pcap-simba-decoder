#pragma once

#include "record.h"
#include <cstdint>
#include <expected>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace pcap {

// Error types for the parser
enum class Error {
    FileNotFound,
    ReadError,
    InvalidMagicNumber,
    UnsupportedVersion,
    UnexpectedEndOfFile
};

// String representation of errors
std::string error_message(Error error);

// Magic numbers for different endianness
constexpr uint32_t MAGIC_NUMBER_NATIVE = 0xa1b2c3d4;
constexpr uint32_t MAGIC_NUMBER_NATIVE_NANO = 0xa1b23c4d;

// File header structure (24 bytes)
struct FileHeader {
    uint32_t magic_number;   // Magic number
    uint16_t version_major;  // Major version
    uint16_t version_minor;  // Minor version
    int32_t  thiszone;       // GMT to local correction
    uint32_t sigfigs;        // Accuracy of timestamps
    uint32_t snaplen;        // Max length saved portion of each packet
    uint32_t network;        // Data link type
};

// PCAP file parser
class Parser {
public:
    Parser();
    ~Parser();
    std::expected<Record, Error> readNextRecord();
    const FileHeader& getFileHeader() const { return file_header_; }
    std::expected<void, Error> open(const std::filesystem::path &path);
private:
    std::ifstream file;
    FileHeader file_header_;
    bool isValidMagicNumber(uint32_t magic);
    bool mIsNanosecond;
};

} // namespace pcap