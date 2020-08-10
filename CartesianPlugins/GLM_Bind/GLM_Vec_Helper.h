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
#include <vector>
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

        static inline auto vec2_to_table(const glm::vec2 &vec){
            std::vector<float> values;
            values.emplace_back(vec.x);
            values.emplace_back(vec.y);
            return sol::as_table(values);
        }

        static inline auto vec3_to_table(const glm::vec3 &vec){
            std::vector<float> values;
            values.emplace_back(vec.x);
            values.emplace_back(vec.y);
            values.emplace_back(vec.z);
            return sol::as_table(values);
        }
        static inline auto vec4_to_table(const glm::vec4 &vec){
            std::vector<float> values;
            values.emplace_back(vec.x);
            values.emplace_back(vec.y);
            values.emplace_back(vec.z);
            values.emplace_back(vec.w);
            return sol::as_table(values);
        }
    };

}

#endif //BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
