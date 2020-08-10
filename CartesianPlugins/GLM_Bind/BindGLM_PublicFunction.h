//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_PUBLICFUNCTION_H
#define BUILDINGCTSLUAJIT_BINDGLM_PUBLICFUNCTION_H

#include "sol.hpp"
namespace Cartesian{

class BindGLM_PublicFunction {
public:
    static void bind(sol::state *lua);
};

}

#endif //BUILDINGCTSLUAJIT_BINDGLM_PUBLICFUNCTION_H
