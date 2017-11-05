#include "InputOutput/JsonInput/json.hpp"
#include "InputOutput/JsonInput/jsoninput.h"
#include "InputOutput/JsonInput/jsonitems.h"
#include "Apps/app.h"
#include "Apps/applist.h"
#include "ParallelTools/parallel.h"
#include <iostream>

namespace LatticeIO {
    namespace JsonInput {
        void validateAppType(){
            // check app specification
            if(jsonFile["App"]["type"].size() == 0){
                std::cerr << "LatticeQCD Error: no App specified\n";
                exit(1);
            }
            if(jsonFile["App"]["type"].size() != 1){
                std::cerr << "LatticeQCD Error: wrong App specification\n"
                             "Correct format is:\n"
                             "\"App\" : {\n"
                             "   \"type\" : \"appType\", \n"
                             "   \"params\" : {        (optional)\n"
                             "       \"param1\" : param1, \n"
                             "       \"param2\" : param2, \n"
                             "       ... \n"
                             "   }\n"
                             "}\n";
                exit(1);
            }

            if(jsonFile["App"]["type"] == "GFR")
                validateGFR();
            else if (jsonFile["App"]["type"] == "FLOW")
                validateFLOW();
            else if (jsonFile["App"]["type"] == "PGFG")
                validatePGFG();
            else{
                std::cerr << "LatticeQCD Error: unknown App "
                          << jsonFile["App"]["type"]
                          << "\n";
                exit(1);
            }
        }

        void validateGFR(){

        }

        void validateFLOW(){
            if(    jsonFile["App"]["params"].size() != 2
                || jsonFile["App"]["params"]["epsilon"].size() != 1
                || jsonFile["App"]["params"]["tauFinal"].size() != 1){
               std::cerr << "LatticeQCD Error: wrong \"FLOW\" app specification\n"
                             "Correct format:"
                             "\"App\" : {\n"
                             "   \"type\" : \"FLOW\", \n"
                             "   \"params\" : { \n"
                             "       \"epsilon\" : epsilon, \n"
                             "       \"tauFinal\" : tauFinal \n"
                             "   }\n"
                             "}\n";
                exit(1);
            }
        }

        void validatePGFG(){
            if(    jsonFile["App"]["params"].size() != 5
                || jsonFile["App"]["params"]["startType"].size() != 1
                || jsonFile["App"]["params"]["MCSteps"].size() != 1
                || jsonFile["App"]["params"]["thermSteps"].size() != 1
                || jsonFile["App"]["params"]["SU3eps"].size() != 1
                || (  jsonFile["App"]["params"]["NConf"].size()
                    + jsonFile["App"]["params"]["NCorr"].size() != 1)
               ){
               std::cerr << "LatticeQCD Error: wrong \"PGFG\" app specification\n"
                             "Correct format:"
                             "\"App\" : {\n"
                             "   \"type\" : \"PGFG\", \n"
                             "   \"params\" : { \n"
                             "       \"MCSteps\"   :  MCSteps, \n"
                             "       \"themSteps\" :  thermSteps\n"
                             "       \"startType\" :  startType, \n"
                             "       \"NConf\"     : NConf,   OR    \"NCorr\" : NCorr, \n"
                             "       \"SU3eps\"    : SU3eps \n"
                             "   }\n"
                             "}\n";
                exit(1);
            }
        }

        void registerApp(App*& app){
            Parallel::createGeometry(latticeSize, subLatticeSize);
            if(jsonFile["App"]["type"] == "PGFG")
                registerPGFG(app);
            else if(jsonFile["App"]["type"] == "FLOW")
                registerFLOW(app);
            else if(jsonFile["App"]["type"] == "GFR")
                registerGFR(app);

            app->createLattice(subLatticeSize);
        }

        void registerPGFG(App*& app){
            if(jsonFile["App"]["params"]["NConf"].size() == 1)
                app = new GaugeFieldFactory(
                            jsonFile["App"]["params"]["MCSteps"],
                            jsonFile["App"]["params"]["thermSteps"],
                            jsonFile["App"]["params"]["NConf"],
                            jsonFile["App"]["params"]["SU3eps"],
                            jsonFile["App"]["params"]["startType"]);
            else
                app = new GaugeFieldFactory(
                            jsonFile["App"]["params"]["MCSteps"],
                            jsonFile["App"]["params"]["thermSteps"],
                            int(jsonFile["App"]["params"]["MCSteps"]) / int(jsonFile["App"]["params"]["NCorr"]),
                            jsonFile["App"]["params"]["SU3eps"],
                            jsonFile["App"]["params"]["startType"]);
        }

        void registerGFR(App*& app){
            app = new GaugeFieldReader();
        }

        void registerFLOW(App*& app){
            app = new WilsonFlow(
                        jsonFile["App"]["params"]["tauFinal"],
                        jsonFile["App"]["params"]["epsilon"]);
        }
    } // end JsonInput
} // end LatticeIO
