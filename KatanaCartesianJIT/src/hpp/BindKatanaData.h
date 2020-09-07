//
// Created by admin on 2020/8/4.
// Bind some general function for katana
//


#ifndef BUILDINGCTSLUAJIT_BINDKATANADATA_H
#define BUILDINGCTSLUAJIT_BINDKATANADATA_H
#undef interface
//#define SOL_NO_EXCEPTIONS 1
#include <FnGeolib/op/FnGeolibOp.h>
#include "sol.hpp"

namespace Cartesian{

    class GetAttribException {

    public:
        std::string what() {
            return "Get Attrib Error, KATANA_API";
        }

    };

    /*
     *  This function should not set to lua statck per cook !
     *
     */

    class BindKatanaFunction{
    public:
        static void bind(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua);

    };




}





#endif //BUILDINGCTSLUAJIT_BINDKATANADATA_H
