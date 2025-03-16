#include "parser.h"
#include <format>
#include <iostream>

namespace pcap {

Parser::Parser() {
    mIsNanosecond = false;
}

Parser::~Parser() {
    if (mFile.is_open()) {
        mFile.close();
    }
}

std::expected<void, Error> Parser::open(const std::filesystem::path& path) {
    mFile.open(path, std::ios::binary);
    if (!mFile) {
        return std::unexpected(Error::FileNotFound);
    }

    if (!mFile.read(reinterpret_cast<char*>(&mFileHeader), sizeof(mFileHeader))) {
        return std::unexpected(Error::ReadError);
    }

    // Check magic number
    if (mFileHeader.magic_number != MAGIC_NUMBER_NATIVE && mFileHeader.magic_number != MAGIC_NUMBER_NATIVE_NANO) {
        return std::unexpected(Error::InvalidMagicNumber);
    }
    // Set timestamp resolution
    if (mFileHeader.magic_number == MAGIC_NUMBER_NATIVE_NANO) {
        mIsNanosecond = true;
    }

    return {};
}

// Reads next record from the file
std::expected<Record, Error> Parser::readNextRecord() {
    RecordHeader record_header;
    if (!mFile.read(reinterpret_cast<char*>(&record_header), sizeof(record_header))) {
        if (mFile.eof()) {
            mFile.clear();
            return std::unexpected(Error::UnexpectedEndOfFile);
        }
        return std::unexpected(Error::ReadError);
    }
    Record record(record_header);
    record.resizeData(record_header.incl_len);
    
    if (!mFile.read(reinterpret_cast<char*>(record.getDataPointer()), record_header.incl_len)) {
        return std::unexpected(Error::ReadError);
    }

    return record;
}

} // namespace pcap
