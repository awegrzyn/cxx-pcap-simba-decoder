#ifndef COMMON_H
#define COMMON_H
#include <cstdint>

enum class MDFlagsSet : uint64_t {
    Day = 0x1,
    IOC = 0x2,
    OTC = 0x4,
    EndOfTransaction = 0x1000,
    PassiveCrossOrderCancel = 0x2000,
    FOK = 0x80000,
    Replace = 0x100000,
    Cancel = 0x200000,
    MassCancel = 0x400000,
    NegotiatedOrder = 0x4000000,
    MultiLegOrder = 0x8000000,
    CrossTrade = 0x20000000,
    NegotiatedOrderRef = 0x80000000,
    COD = 0x100000000,
    SyntheticOrder = 0x200000000000,
    RFS = 0x400000000000,
    SyntheticPassive = 0x200000000000000,
    BookOrCancel = 0x1000000000000000
};

enum class MDUpdateAction : uint8_t {
    New = 0,
    Change = 1,
    Delete = 2,
};

enum class MDEntryType : char {
    Bid = '0',
    Offer = '1',
    EmptyBook = 'J',
};

struct GroupSize {
    uint16_t blockLength;
    uint8_t numInGroup;

    static consteval std::size_t size() {
        return sizeof(blockLength) + sizeof(numInGroup);
    };
};

struct Decimal5 {
    int64_t mantissa;
    double operator()() const {
        return mantissa * 1e-5;
    }
    static consteval std::size_t size() {
        return sizeof(mantissa);
    }
};

struct Decimal5NULL {
    int64_t mantissa;
    static constexpr int64_t NULL_VALUE = 9223372036854775807;
    double operator()() const {
        return mantissa == NULL_VALUE ? 0 : mantissa * 1e-5;
    }

    static consteval std::size_t size() {
        return sizeof(mantissa);
    }    
};
#endif // COMMON_H