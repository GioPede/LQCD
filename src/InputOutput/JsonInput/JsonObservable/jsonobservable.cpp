#include "InputOutput/JsonInput/json.hpp"
#include "InputOutput/JsonInput/jsoninput.h"
#include "InputOutput/JsonInput/JsonObservable/jsonobservable.h"
#include "Observables/JsonInput/jsonuserobservables.h"
#include "Apps/app.h"
#include "Observables/observablelist.h"
#include <iostream>

namespace LatticeIO {
    namespace JsonInput {
        void validateObservables(){
            // check observables
            if(jsonFile["App"]["type"] == "GFR" || jsonFile["App"]["type"] == "FLOW"){
                if(jsonFile.count("Observables") == 0){
                    std::cerr << "LatticeQCD Error: no Observables specified\n";
                    exit(1);
                }
                if(jsonFile.count("Observables") != 1){
                    std::cerr << "LatticeQCD Error: too many Observables tag specified\n";
                    exit(1);
                }
            }
            if(jsonFile["Observables"].size() == 0){
                std::cerr << "LatticeQCD Error: no Observables specified\n";
                exit(1);
            }

            // check observables
            for(auto& obs : jsonFile["Observables"]){
                if(   !validateCoreObservables(obs)
                   && !validateUserObservables(obs)
                   ) {
                    std::cerr << "LatticeQCD Error: unknown Observable "
                              << obs << "\n";
                    exit(1);
                }

            }
        }

        bool validateCoreObservables(std::string obs){
            if(obs == "plaquette"){
                return true;
            }
            else if(obs == "energydensity"){
                return true;
            }
            else if(obs == "topologicalcharge"){
                return true;
            }
            return false;
        }

        void registerObservables(App*& app){
            for(auto& obs : jsonFile["Observables"]){
                registerCoreObservables(app, obs);
                registerUserObservables(app, obs);
            }
        }

        void registerCoreObservables(App*& app, std::string obs){
            if(obs == "plaquette"){
                app->addObservable(new Plaquette());
            }
            else if(obs == "energydensity"){
                app->addObservable(new EnergyDensity());
            }
            else if(obs == "topologicalcharge"){
                app->addObservable(new TopologicalCharge());
            }
        }

    } // end JsonInput
} // end LatticeIO
