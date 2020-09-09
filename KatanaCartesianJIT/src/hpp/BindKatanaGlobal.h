//
// Created by admin on 2020/8/3.
//

#ifndef BUILDINGCTSLUAJIT_BINDKATANAGLOBAL_H
#define BUILDINGCTSLUAJIT_BINDKATANAGLOBAL_H

#undef interface
#include <FnGeolib/op/FnGeolibOp.h>
#include "sol.hpp"

namespace Cartesian{

    class BindGlobalVars{
    public:
        static void bind(Foundry::Katana::GeolibCookInterface& interface, sol::state* lua);
    };



}






#endif //BUILDINGCTSLUAJIT_BINDKATANAGLOBAL_H
