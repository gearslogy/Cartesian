//
// Created by admin on 2020/8/1.
//

#include "CartesianEntryPoint.h"
#include "sol.hpp"
#include <cmath>
#include "GLM_Bind/BindGLM_Vec.h"
#include "GLM_Bind/BindGLM_Matrix.h"
#include "GLM_Bind/BindGLM_PublicFunction.h"
#include "GLM_Bind/BindGLM_MatrixTable.h"
#include "CGAL_Bind/BindCGAL.h"
#include "CGAL_Bind/BindCGAL_Point_Functions.h"
using namespace std;

void newOp(void *lua){
    auto *luaPtr = static_cast<sol::state *> (lua);
    luaPtr->set_function("sin",static_cast<float(*) (float)> (&sin));
    luaPtr->set_function("cos",static_cast<float(*) (float)> (&cos));
    luaPtr->set_function("acos",static_cast<float(*) (float)> (&acos));
    luaPtr->set_function("asin",static_cast<float(*) (float)> (&asin));
    luaPtr->set_function("pow",static_cast<double(*) (double,double)> (&pow));
    luaPtr->set_function("ceil",static_cast<double(*) (double)> (&ceil));
    luaPtr->set_function("floor",static_cast<double(*) (double)> (&floor));

    Cartesian::BindGLM_Vec::bind(luaPtr);
    Cartesian::BindGLM_Matrix::bind(luaPtr);
    Cartesian::BindGLM_PublicFunction::bind(luaPtr);
    Cartesian::BindGLM_MatrixTable::bind(luaPtr);
    Cartesian::BindCGAL::bind(luaPtr);
    Cartesian::BindCGAL_Point_Functions::bind(luaPtr);
}
