#include "pcap/parser.h"
#include "protocols/ethernet.h"
#include "protocols/ipv4.h"
#include "protocols/udp.h"
#include "protocols/simba.h"
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
    parser.open(filepath);

    std::cout << "PCAP version: " << parser.getFileHeader().version_major << "." 
              << parser.getFileHeader().version_minor << std::endl;

    size_t count = 0;
    while (true) {
        auto record_result = parser.readNextRecord();
        if (!record_result) {
            if (record_result.error() == pcap::Error::UnexpectedEndOfFile) {
                break;
            }
            std::cerr << "Error !" << std::endl;
            return 1;
        }

        const pcap::Record record = record_result.value();
        count++;

        protocols::Ethernet frame(record.getData());
        auto ethernetResult = frame.parse();
        if (!ethernetResult) {
            std::cerr << "Error parsing Ethernet frame: " << static_cast<int>(ethernetResult.error()) << std::endl;
            return 1;
        }
        protocols::Ipv4 ipv4(frame.getPayload());
        auto ipv4Result = ipv4.parse();
        if (!ipv4Result) {
            std::cerr << "Error parsing IPv4 packet: " << static_cast<int>(ipv4Result.error()) << std::endl;
            return 1;
        }
        protocols::Udp udp(ipv4.payload());
        auto udpResult = udp.parse();
        if (!udpResult) {
            std::cerr << "Error parsing UDP packet: " << static_cast<int>(udpResult.error()) << std::endl;
            return 1;
        }

        protocols::SimbaSpectra simba(udp.getPayload());
        auto simbaResult = simba.parse();
        if (!simbaResult) {
            std::cerr << "Error parsing SIMBA SPECTRA message: " << static_cast<int>(simbaResult.error()) << std::endl;
            return 1;
        }
        std::cout << simba.toJson() << std::endl;
    }
    std::cout << "Total records: " << count << std::endl;
    return 0;
}