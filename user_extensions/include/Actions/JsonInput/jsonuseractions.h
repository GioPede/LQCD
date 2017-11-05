#pragma once
#include "InputOutput/JsonInput/json.hpp"

namespace LatticeIO {
    namespace JsonInput {
        // Collective functions
        bool validateUserActions();
        void registerUserActions(class App* app);

        // Input validation functions
        void validateMyPureGauge();
    } // end JsonInput
} // end LatticeIO
