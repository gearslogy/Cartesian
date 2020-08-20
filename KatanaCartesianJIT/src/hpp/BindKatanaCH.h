//
// Created by admin on 2020/8/4.
//

#ifndef BUILDINGCTSLUAJIT_BINDKATACH_H
#define BUILDINGCTSLUAJIT_BINDKATACH_H


#include "CartesianLog.h"
#include "sol.hpp"


#undef interface
#undef max
#undef min
#undef GetCurrentTime
#include <FnGeolib/op/FnGeolibOp.h>
#undef interface

namespace Cartesian {

    class BindKatanaCH {
    public:
        static void bind(Foundry::Katana::GeolibCookInterface& Geolibface, const std::shared_ptr<sol::state>& lua);    
    };
}
#endif //BUILDINGCTSLUAJIT_BINDKATACH_H
