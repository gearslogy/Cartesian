//
// Created by admin on 2020/8/10.
//

#ifndef BUILDINGCTSLUAJIT_BINDCGAL_POINT_FUNCTIONS_H
#define BUILDINGCTSLUAJIT_BINDCGAL_POINT_FUNCTIONS_H


#define CGAL_NO_GMP 1
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include "BindCGAL_DefineType.h"
#include "sol.hpp"




namespace Cartesian {
   

    class BindCGAL_Point_Functions {
    public:
        static void bind(sol::state *lua);
    };



}


#endif //BUILDINGCTSLUAJIT_BINDCGAL_POINT_FUNCTIONS_H
