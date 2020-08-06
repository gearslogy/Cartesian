//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
#define BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
#include <string>
#include "sol.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Cartesian{
    class GLM_Vec_Helper{
    public:
        static inline glm::vec2 table_to_vec2(const sol::lua_table &table){
            return glm::vec2(table.get<float>(1),table.get<float>(2));
        }

        static inline glm::vec3 table_to_vec3(const sol::lua_table &table){
            return glm::vec3(table.get<float>(1),table.get<float>(2),table.get<float>(3));
        }
        static inline glm::vec4 table_to_vec4(const sol::lua_table &table){
            return glm::vec4(table.get<float>(1),
                    table.get<float>(2),
                    table.get<float>(3),
                    table.get<float>(4));
        }
    };

}

#endif //BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
