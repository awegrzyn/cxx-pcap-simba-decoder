#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include <memory>
#include <expected>

// Use packed structs for binary protocol parsing
#pragma pack(push, 1)

// Forward declare the test class
namespace Test {
    class SimbaParserTest_SingleOrderUpdate_Test;
}

namespace protocols {

class SimbaSpectra {
    friend class Test::SimbaParserTest_SingleOrderUpdate_Test;
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
private:
    struct MarketDataHeader {
        uint32_t MsgSeqNum;
        uint16_t MsgSize;
        uint16_t MsgFlags;
        uint64_t SendingTime;
        bool IsFragmented() const noexcept { return MsgFlags & 0x1; }
        bool IsStartOfSnapshot() const noexcept { return MsgFlags & 0x2; }
        bool IsEndOfSnapshot() const noexcept { return MsgFlags & 0x4; }
        bool IsIncremental() const noexcept { return MsgFlags & 0x8; }
        bool IsPossDupFlag() const noexcept { return MsgFlags & 0x10; }

        static consteval std::size_t size() {
            return sizeof(MsgSeqNum) + sizeof(MsgSize) + sizeof(MsgFlags) + sizeof(SendingTime);
        }
    };

    struct IncrementalPacketHeader {
        uint64_t TransactTime;
        uint32_t ExchangeTradingSessionID;

        static consteval std::size_t size(){
            return sizeof(TransactTime) + sizeof(ExchangeTradingSessionID);
        }
        static consteval uint32_t getExchangeTradingSessionIDNull() {
            return 4294967295;
        }
    };

    // SBE Message Header - matches reference
    struct SbeMessageHeader {
        uint16_t BlockLength;
        uint16_t TemplateId;
        uint16_t SchemaId;
        uint16_t Version;

        static consteval std::size_t size() {
            return sizeof(BlockLength) + sizeof(TemplateId) + sizeof(SchemaId) + sizeof(Version);
        }
    };

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
        int64_t operator()() const {
            return mantissa * 1e-5;
        }
        static consteval std::size_t size() {
            return sizeof(mantissa);
        }
    };

    struct Decimal5NULL {
        int64_t mantissa;
        static constexpr int64_t MAX_VALUE = 9223372036854775806;
        static constexpr int64_t NULL_VALUE = 9223372036854775807;
        static consteval size_t size() {
            return sizeof(mantissa);
        }    
        static consteval double exponent() {
            return 1e-5;
        }
    };

    // TemplateID=15
    struct OrderUpdate {
        int64_t MDEntryID;
        Decimal5 MDEntryPx;
        int64_t MDEntrySize;
        MDFlagsSet MDFlags;
        uint64_t MDFlags2 ;
        int32_t SecurityID;
        uint32_t RptSeq;
        MDUpdateAction MDUpdateAction_;
        MDEntryType MDEntryType_;

        static consteval std::size_t size() {
            return sizeof(MDEntryID) + sizeof(MDEntryPx.mantissa) + sizeof(MDEntrySize) + sizeof(MDFlags) + sizeof(MDFlags2) + sizeof(SecurityID) + sizeof(RptSeq) + sizeof(MDUpdateAction) + sizeof(MDEntryType);
        }
        static consteval uint16_t templateId() {
            return 15;
        }
    };

    // TemplateID=16
    struct OrderExecution {
        int64_t MDEntryID;
        Decimal5NULL MDEntryPx;
        int64_t MDEntrySize;
        Decimal5 LastPx;
        int64_t LastQty;
        int64_t TradeID;
        MDFlagsSet MDFlags;
        uint64_t MDFlags2;
        int32_t SecurityID;
        uint32_t RptSeq;
        MDUpdateAction MDUpdateAction_;
        MDEntryType MDEntryType_;

        static consteval std::size_t size() {
            return sizeof(MDEntryID) + sizeof(MDEntryPx.mantissa) + sizeof(MDEntrySize) + sizeof(LastPx.mantissa) + sizeof(LastQty) + sizeof(TradeID) + sizeof(MDFlags) + sizeof(MDFlags2) + sizeof(SecurityID) + sizeof(RptSeq) + sizeof(MDUpdateAction) + sizeof(MDEntryType);
        }
        static consteval uint16_t templateId() {
            return 16;
        }
    };

    // TemplateID=17
    struct OrderBookSnapshot {
        int32_t SecurityID;
        uint32_t LastMsgSeqNumProcessed;
        uint32_t RptSeq;
        uint32_t ExchangeTradingSessionID;
        GroupSize NoMDEntries;

        struct Entry {
            int64_t MDEntryID;
            uint64_t TransactTime;
            Decimal5NULL MDEntryPx;
            int64_t MDEntrySize;
            int64_t TradeID;
            MDFlagsSet MDFlags;
            uint64_t MDFlags2;
            MDEntryType MDEntryType_;

            static consteval std::size_t size() {
                return sizeof(MDEntryID) + sizeof(TransactTime) + sizeof(MDEntryPx.mantissa) + sizeof(MDEntrySize) + sizeof(TradeID) + sizeof(MDFlags) + sizeof(MDFlags2) + sizeof(MDEntryType);
            }
        };
        std::vector<Entry> entries;
        static consteval std::size_t size() {
            return sizeof(SecurityID) + sizeof(LastMsgSeqNumProcessed) + sizeof(RptSeq) + sizeof(ExchangeTradingSessionID) + GroupSize::size();
        }
        static consteval uint16_t templateId() {
            return 17;
        }
    };
public:
    SimbaSpectra(std::span<const std::byte> udpData);
    std::expected<bool, Error> parse();
    const std::vector<OrderUpdate>& getOrderUpdates() const noexcept { return mOrderUpdates; }
private:
    MarketDataHeader parseMarketDataHeader() const;
    IncrementalPacketHeader parseIncrementalPacketHeader() const;
    SbeMessageHeader parseSbeMessageHeader() const;
    OrderUpdate parseOrderUpdate() const;
    OrderExecution parseOrderExecution() const;
    OrderBookSnapshot parseOrderBookSnapshot() const;

    mutable std::size_t mParsingOffset;
    std::span<const std::byte> mUdpData;
    std::vector<OrderUpdate> mOrderUpdates;
    std::vector<OrderExecution> mOrderExecutions;
    std::vector<OrderBookSnapshot> mOrderBookSnapshots;
};

} // namespace protocols

#pragma pack(pop)