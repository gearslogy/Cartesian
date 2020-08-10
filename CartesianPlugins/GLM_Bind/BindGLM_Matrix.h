//
// Created by admin on 2020/8/4.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_MATRIX_H
#define BUILDINGCTSLUAJIT_BINDGLM_MATRIX_H

#include "sol.hpp"

namespace Cartesian {
    class BindGLM_Matrix {
    public:
        static void bind(sol::state *lua);
    };
}

#endif //BUILDINGCTSLUAJIT_BINDGLM_MATRIX_H
