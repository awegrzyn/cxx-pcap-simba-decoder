#include "pcap/parser.h"
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
    
    pcap::Parser parser(filepath);

    std::cout << "PCAP version: " << parser.getFileHeader().version_major << "." 
              << parser.getFileHeader().version_minor << std::endl;

    size_t count = 0;
    while (true) {
        auto record_result = parser.readNextRecord();
        if (!record_result) {
            if (record_result.error() == pcap::Error::UnexpectedEndOfFile) {
                break;
            }
            std::cerr << "Error: " << pcap::error_message(record_result.error()) << std::endl;
            return 1;
        }
        
        const auto& record = record_result.value();
        count++;
        
        std::cout << "Record #" << count 
                  << " - time: " << std::fixed << std::setprecision(6) 
                  << record.timestamp()
                  << ", length: " << record.header.incl_len << " bytes" << std::endl;
    }
    
    std::cout << "Total records: " << count << std::endl;
    return 0;
}
