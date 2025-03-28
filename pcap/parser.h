/*
 * Author: Adam Wegrzynek
 * License: GPL-3.0
 */

#ifndef PCAP_PARSER_H
#define PCAP_PARSER_H

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

    /// Reads next record from PCAP file
    /// \return Record object containing the data or an error
    std::expected<Record, Error> readNextRecord();

    /// \return The file header
    const FileHeader& getFileHeader() const { return mFileHeader; }

    /// Opens a PCAP file for reading
    std::expected<void, Error> open(const std::filesystem::path &path);
private:
    /// File stream for reading the PCAP file
    std::ifstream mFile;

    /// File header
    FileHeader mFileHeader;

    /// Indicates if the file uses nanosecond timestamps
    bool mIsNanosecond;
};

} // namespace pcap
#endif // PCAP_PARSER_H
