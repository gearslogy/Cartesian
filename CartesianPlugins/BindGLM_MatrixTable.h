//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_MATRIXTABLE_H
#define BUILDINGCTSLUAJIT_BINDGLM_MATRIXTABLE_H
/*
 * This file only bind matrix for:
 * mat_table * mat_table -> return mat_table
 * mat_table * vec_table -> return vec_table
 * because can improve the speed of LUAJIT
 */

#include "sol.hpp"

class BindGLM_MatrixTable {
public:
    static void bind(sol::state *state);
};


#endif //BUILDINGCTSLUAJIT_BINDGLM_MATRIXTABLE_H
