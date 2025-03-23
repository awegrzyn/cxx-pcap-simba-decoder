#include "simba.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iomanip>

namespace protocols {

// Constructor
SimbaSpectra::SimbaSpectra(std::span<const std::byte> udpData) 
    : mUdpData(udpData) {
}


// Implementation of parse method - similar to reference implementation
std::expected<bool, SimbaSpectra::Error> SimbaSpectra::parse() {


}
} // namespace protocols