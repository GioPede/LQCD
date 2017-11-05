#pragma once
#include <iostream>
#include <fstream>
#include "InputOutput/JsonInput/json.hpp"
#include "InputOutput/JsonInput/jsonitems.h"

namespace LatticeIO {
    namespace JsonInput {

        // JsonFile
        extern nlohmann::json jsonFile;

        // Main Functions
		void parseInput(int argn, char* argv[]);
        void initApp(class App*&);

    } // end JsonInput
} // end LatticeIO
