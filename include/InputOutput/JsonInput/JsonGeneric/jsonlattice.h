#pragma once
#include "InputOutput/JsonInput/json.hpp"
#include <array>

namespace LatticeIO {
    namespace JsonInput {
        // Validate Lattice Sizes
        void validateLatticeInput();
        void validateSubLatticeInput();
        extern std::array<int, 4> latticeSize;
        extern std::array<int, 4> subLatticeSize;
    } // end JsonInput
} // end LatticeIO
