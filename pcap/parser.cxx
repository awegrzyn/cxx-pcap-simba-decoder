#include "parser.h"
#include <format>
#include <iostream>

namespace pcap {

std::string error_message(Error error) {
    switch (error) {
        case Error::FileNotFound: return "File not found";
        case Error::ReadError: return "Error reading file";
        case Error::InvalidMagicNumber: return "Invalid magic number";
        case Error::UnsupportedVersion: return "Unsupported PCAP version";
        case Error::UnexpectedEndOfFile: return "Unexpected end of file";
    }
    return "Unknown error";
}

Parser::Parser(const std::string& filename) {
    mIsNanosecond = false;
    open(filename);
}

Parser::~Parser() {
    if (file.is_open()) {
        file.close();
    }
}

std::expected<void, Error> Parser::open(const std::filesystem::path& path) {
    file.open(path, std::ios::binary);
    if (!file) {
        return std::unexpected(Error::FileNotFound);
    }

    if (!file.read(reinterpret_cast<char*>(&file_header_), sizeof(file_header_))) {
        return std::unexpected(Error::ReadError);
    }

    // Check magic number
    if (!isValidMagicNumber(file_header_.magic_number)) {
        return std::unexpected(Error::InvalidMagicNumber);
    }
    if (file_header_.magic_number == MAGIC_NUMBER_NATIVE_NANO) {
        mIsNanosecond = true;
    }

    return {};
}

// Reads next record from the file
std::expected<Record, Error> Parser::readNextRecord() {
    RecordHeader record_header;
    std::cout << "Trying to read record" << std::endl;
    if (!file.read(reinterpret_cast<char*>(&record_header), sizeof(record_header))) {
        if (file.eof()) {
            file.clear();
            return std::unexpected(Error::UnexpectedEndOfFile);
        }
        return std::unexpected(Error::ReadError);
    }
    Record record(mIsNanosecond);
    record.header = record_header;
    record.resizeData(record_header.incl_len);
    
    if (!file.read(reinterpret_cast<char*>(record.getDataPointer()), record_header.incl_len)) {
        return std::unexpected(Error::ReadError);
    }

    return record;
}

bool Parser::isValidMagicNumber(uint32_t magic) {
    return magic == MAGIC_NUMBER_NATIVE || magic == MAGIC_NUMBER_NATIVE_NANO;
}

} // namespace pcap
