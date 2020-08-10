//
// Created by admin on 2020/8/10.
//

#ifndef BUILDINGCTSLUAJIT_BINDCGAL_POINT_FUNCTIONS_H
#define BUILDINGCTSLUAJIT_BINDCGAL_POINT_FUNCTIONS_H

#include "sol.hpp"

namespace Cartesian {
    class BindCGAL_Point_Functions {
    public:
        static void bind(sol::state *lua);
    };
}

#endif //BUILDINGCTSLUAJIT_BINDCGAL_POINT_FUNCTIONS_H
