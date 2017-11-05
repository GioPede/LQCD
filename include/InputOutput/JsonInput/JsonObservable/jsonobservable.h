#pragma once
#include "InputOutput/JsonInput/json.hpp"
#include "Apps/app.h"

namespace LatticeIO {
    namespace JsonInput {
        // Generic observable validation
        void validateObservables();
        bool validateCoreObservables(std::string obsType);

        // Core implemented observables
        void validatePlaquette();

        // Generic observable registration
        void registerObservables(App*& app);
        void registerCoreObservables(App*& app, std::string obsType);

    } // end JsonInput
} // end LatticeIO
