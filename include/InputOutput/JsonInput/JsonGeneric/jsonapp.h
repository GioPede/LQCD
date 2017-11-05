#pragma once
#include "InputOutput/JsonInput/json.hpp"
#include "Apps/app.h"

namespace LatticeIO {
    namespace JsonInput {
        // Validate App
        void validateAppType();
        void validatePGFG();
        void validateGFR();
        void validateFLOW();

        // Register App
        void registerApp(App*& app);
        void registerGFR(App*& app);
        void registerPGFG(App*& app);
        void registerFLOW(App*& app);

    } // end JsonInput
} // end LatticeIO
