#include <iostream>
#include "lqcd.h"
#include "Actions/JsonInput/jsonuseractions.h"

namespace LatticeIO {
    namespace JsonInput {
        // EXAMPLE FUNCTION FOR INPUT VALIDATION OF USER-DEFINED ACTIONS
        // Usage:
        // - Add the string tag of your action in the if test in the following
        //   function.  Call the function to validate the single instance
        bool validateUserActions(){
        //    // check mypuregauge
        //    if(jsonFile["Action"]["type"] == "mypuregauge"){
        //        validateMyPureGauge();
        //        return true;
        //    }
        //    return false;

            // Comment this line if a user-defined action is provided
            return true;
        }

        void registerUserActions(App* app){
            if(jsonFile["Action"]["type"] == "mypuregauge"){
                app->setAction(new PureGauge(
                                      double(jsonFile["Action"]["params"]["beata"])));
            }
        }

        void validateMyPureGauge(){
            // check mypuregauge
            if(    jsonFile["Action"]["params"].size() != 1
                || jsonFile["Action"]["params"]["beata"].size() != 1){
               std::cerr << "LatticeQCD Error: wrong \"puregauge\" action specification\n"
                             "Correct format:"
                             "\"Action\" : {\n"
                             "   \"type\" : \"mypuregauge\", \n"
                             "   \"params\" : { \n"
                             "       \"beata\" : beta, \n"
                             "   }\n"
                             "}\n";
                exit(1);
            }
        }
    } // end JsonInput
} // end LatticeIO
