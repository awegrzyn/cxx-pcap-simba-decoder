#include "pcap_record.h"

namespace pcap {

double Record::timestamp(bool is_nanosecond) const {
    double subsecond_divisor = is_nanosecond ? 1e9 : 1e6; // 1e9 for nanoseconds, 1e6 for microseconds
    return static_cast<double>(header.ts_sec) + static_cast<double>(header.ts_subsec) / subsecond_divisor;
}

} // namespace pcap
