#include <iostream>
#include "lqcd.h"
#include "Observables/JsonInput/jsonuserobservables.h"

namespace LatticeIO {
    namespace JsonInput {
        // EXAMPLE FUNCTION FOR INPUT VALIDATION OF USER-DEFINED OBSERVABLE
        // Usage:
        // - Add the string tag of your observable in the if test in the following
        //   function.  Call the function to validate the single instance
        bool validateUserObservables(std::string obs){
            // check myplaquette
            if(obs == "myplaquette"){
                return true;
            }
            return false;

            // Comment this line if a user-defined observable is provided
            //return true;
        }

        void registerUserObservables(App* app, std::string obs){
            // check myplaquette
            if(obs == "myplaquette"){
                app->addObservable(new Plaquette());
            }
        }

    } // end JsonInput
} // end LatticeIO
