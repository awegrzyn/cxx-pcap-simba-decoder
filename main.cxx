#include "pcap/pcap_parser.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <pcap_file>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::filesystem::path filepath(filename);
    
    pcap::Parser parser;
    auto open_result = parser.open(filepath);
    
    if (!open_result) {
        std::cerr << "Error: " << pcap::error_message(open_result.error()) << std::endl;
        return 1;
    }

    std::cout << "PCAP version: " << parser.get_file_header().version_major << "." 
              << parser.get_file_header().version_minor << std::endl;
    std::cout << "Timestamp precision: " 
              << (parser.get_file_header().is_nanosecond() ? "nanosecond" : "microsecond") << std::endl;

    size_t count = 0;
    while (true) {
        auto record_result = parser.read_next_record();
        if (!record_result) {
            if (record_result.error() == pcap::Error::UnexpectedEndOfFile) {
                // End of file reached
                break;
            }
            std::cerr << "Error: " << pcap::error_message(record_result.error()) << std::endl;
            return 1;
        }
        
        const auto& record = record_result.value();
        count++;
        
        std::cout << "Record #" << count 
                  << " - time: " << std::fixed << std::setprecision(6) 
                  << record.timestamp(parser.get_file_header().is_nanosecond())
                  << ", length: " << record.header.incl_len << " bytes" << std::endl;
    }
    
    std::cout << "Total records: " << count << std::endl;
    return 0;
}
