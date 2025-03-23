#ifndef SBE_MESSAGE_HEADER_H
#define SBE_MESSAGE_HEADER_H

#include <span>
#include <cstdint>
#include <cstddef>
#include <cassert>

// uint16_t BlockLength;
// uint16_t TemplateId;
// uint16_t SchemaId;
// uint16_t Version;
class SbeMessageHeader {
public:
    static consteval std::size_t size() {
        return sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t);
    }
    SbeMessageHeader(const std::span<const std::byte>::iterator rawDataBegin) : mRawHeader(rawDataBegin, SbeMessageHeader::size()) {
        assert(rawHeader.size() < SbeMessageHeader::size());
    }
    const uint16_t BlockLength() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data());}
    const uint16_t TemplateId() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint16_t)); }
    const uint16_t SchemaId() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint16_t) + sizeof(uint16_t)); }
    const uint16_t Version() const { return *reinterpret_cast<const uint16_t*>(mRawHeader.data() + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t)); }
private:    
    const std::span<const std::byte> mRawHeader;
};
#endif // SBE_MESSAGE_HEADER_H