#pragma once
#include "InputOutput/JsonInput/json.hpp"

namespace LatticeIO {
    namespace JsonInput {
        bool validateUserObservables(std::string obs);
        void registerUserObservables(class App* app, std::string obs);
    } // end JsonInput
} // end LatticeIO
