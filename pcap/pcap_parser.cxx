#include "pcap_parser.h"
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

bool FileHeader::is_nanosecond() const {
    return magic_number == MAGIC_NUMBER_NATIVE_NANO;
}

Parser::Parser(const std::string& filename) {
    file.open(filename, std::ios::binary);
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
    if (!is_valid_magic_number(file_header_.magic_number)) {
        return std::unexpected(Error::InvalidMagicNumber);
    }

    return {};
}

// Reads next record from the file
std::expected<Record, Error> Parser::read_next_record() {
    if (!file) {
        return std::unexpected(Error::ReadError);
    }
    RecordHeader record_header;
    if (!file.read(reinterpret_cast<char*>(&record_header), sizeof(record_header))) {
        if (file.eof()) {
            file.clear();
            return std::unexpected(Error::UnexpectedEndOfFile);
        }
        return std::unexpected(Error::ReadError);
    }
    Record record;
    record.header = record_header;
    record.data.resize(record_header.incl_len);
    
    if (!file.read(reinterpret_cast<char*>(record.data.data()), record_header.incl_len)) {
        return std::unexpected(Error::ReadError);
    }

    return record;
}

bool Parser::is_valid_magic_number(uint32_t magic) {
    return magic == MAGIC_NUMBER_NATIVE || magic == MAGIC_NUMBER_NATIVE_NANO;
}

bool Parser::isOpen() const {
    return file.is_open();
}

bool Parser::isEof() const {
    return file.eof();
}

Record Parser::readNextRecord() {
    Record record;
    
    // Read record header
    file.read(reinterpret_cast<char*>(&record.header), sizeof(RecordHeader));
    
    if (file) {
        // Allocate space for data and read it
        record.data.resize(record.header.incl_len);
        file.read(reinterpret_cast<char*>(record.data.data()), record.header.incl_len);
    }
    
    return record;
}

std::vector<Record> Parser::readAllRecords() {
    std::vector<Record> records;
    
    while (file && !file.eof()) {
        Record record = readNextRecord();
        if (file) {
            records.push_back(std::move(record));
        }
    }
    
    return records;
}

} // namespace pcap
