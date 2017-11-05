#include "InputOutput/JsonInput/json.hpp"
#include "InputOutput/JsonInput/jsoninput.h"
#include "InputOutput/JsonInput/JsonAction/jsonaction.h"
#include "Actions/JsonInput/jsonuseractions.h"
#include "Apps/app.h"
#include "Actions/actionlist.h"
#include <iostream>

namespace LatticeIO {
    namespace JsonInput {
        void validateAction(){
            // check action
            if(jsonFile["App"]["type"] == "PGFG" || jsonFile["App"]["type"] == "FLOW"){
                if(jsonFile.count("Action") == 0){
                    std::cerr << "LatticeQCD Error: no Action specified\n";
                    exit(1);
                }
                if(jsonFile.count("Action") != 1){
                    std::cerr << "LatticeQCD Error: too many Action specified\n";
                    exit(1);
                }

                // check for action specification
                if(    jsonFile["Action"]["type"].size() != 1
                    || jsonFile["Action"]["params"].size() == 0){
                    std::cerr << "LatticeQCD Error: wrong Action specification\n"
                                 "Correct format is:\n"
                                 "\"Action\" : {\n"
                                 "   \"type\" : actionType, \n"
                                 "   \"params\" : { \n"
                                 "       \"param1\" : param1, \n"
                                 "       \"param2\" : param2, \n"
                                 "       ... \n"
                                 "   }\n"
                                 "}\n";
                    exit(1);
                }

                // check core actions
                if(   !validateCoreActions()
                   && !validateUserActions()
                   ) {
                    std::cerr << "LatticeQCD Error: unknown Action "
                              << jsonFile["Action"]["type"] << "\n";
                    exit(1);
                }

            }
        }

        bool validateCoreActions(){
            // check puregauge
            if(jsonFile["Action"]["type"] == "puregauge"){
                validatePureGauge();
                return true;
            }
            return false;
        }

        void validatePureGauge(){
            // check puregauge
            if(    jsonFile["Action"]["params"].size() != 1
                || jsonFile["Action"]["params"]["beta"].size() != 1){
               std::cerr << "LatticeQCD Error: wrong \"puregauge\" action specification\n"
                             "Correct format:"
                             "\"Action\" : {\n"
                             "   \"type\" : \"puregauge\", \n"
                             "   \"params\" : { \n"
                             "       \"beta\" : beta \n"
                             "   }\n"
                             "}\n";
                exit(1);
            }
        }

        void registerAction(class App*& app){
            registerCoreActions(app);
            registerUserActions(app);
        }

        void registerCoreActions(class App*& app){
            if(jsonFile["Action"]["type"] == "puregauge")
                registerPureGauge(app);
        }
        // Core implemented actions
        void registerPureGauge(class App*& app){
            double beta = jsonFile["Action"]["params"]["beta"];
            app->setAction(new PureGauge(beta));
        }

    } // end JsonInput
} // end LatticeIO
