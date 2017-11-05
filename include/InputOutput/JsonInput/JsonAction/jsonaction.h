#pragma once
#include "InputOutput/JsonInput/json.hpp"
#include "Apps/app.h"

namespace LatticeIO {
    namespace JsonInput {
        // Generic action validation
        void validateAction();
        bool validateCoreActions();

        // Core implemented actions
        void validatePureGauge();

        // Generic action resitration
        void registerAction(App*& app);
        void registerCoreActions(App*& app);

        // Core implemented actions
        void registerPureGauge(App*& app);

    } // end JsonInput
} // end LatticeIO
