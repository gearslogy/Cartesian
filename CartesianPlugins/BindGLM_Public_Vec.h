//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
#define BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
#include <string>
#include "sol.hpp"
namespace Cartesian{
    // ----------------------- vector3 register to lua:vector---------------------------------
    // +
    template <typename T>
    auto vec_add_vec = [](const T &o1, const T &o2){
        return o1 + o2;
    };

    template <typename T>
    auto vec_add_float = [](const T &o1, const float &o2){
        return o1 + o2;
    };
    // -
    template <typename T>
    auto vec_sub_vec = [](const T &o1, const T &o2){
        return o1 - o2;
    };

    template <typename T>
    auto vec_sub_float = [](const T &o1, const float &o2){
        return o1 - o2;
    };

    // *
    template <typename T>
    auto vec_mul_vec = [](const T &o1, const T &o2){
        return o1 * o2;
    };

    template <typename T>
    auto vec_mul_float = [](const T &o1, const float &o2){
        return o1 * o2;
    };
    // /
    template <typename T>
    auto vec_div_vec = [](const T &o1, const T &o2){
        return o1 / o2;
    };

    template <typename T>
    auto vec_div_float = [](const T &o1, const float &o2) {
        return o1 / o2;
    };

    // =
    template <typename T>
    auto vec_equal_vec = []( T &o1,  const T &o2) -> bool{
        return o1 == o2;
    };

    // vector to string , lua can direct print(glm::vec*)
    std::string vec2ToString(const glm::vec2 &vec){
        std::string ret =  std::to_string(vec.x) + std::string(",") + std::to_string(vec.y) ;
        return ret;
    }
    std::string vec3ToString(const glm::vec3 &vec){
        std::string ret =  std::to_string(vec.x) + std::string(",") + std::to_string(vec.y) + std::string(",") + std::to_string(vec.z);
        return ret;
    }
    std::string vec4ToString(const glm::vec4 &vec){
        std::string ret =  std::to_string(vec.x) + std::string(",") + std::to_string(vec.y) + std::string(",") + std::to_string(vec.z)+ std::string(",") + std::to_string(vec.w);
        return ret;
    }

    // normalize vector
    template <typename T>
    T normalize_vector(const T&vec){
        return glm::normalize(vec);
    }

    // cross vector
    template <typename T>
    T cross_vector(const T&vec1,const T&vec2){
        return glm::cross(vec1,vec2);
    }
    // dot vector
    template <typename T>
    float dot_vector(const T&vec1, const T&vec2){
        return glm::dot(vec1,vec2);
    }

    glm::vec2 table_to_vec2(const sol::lua_table &table){
        return glm::vec2(table.get<float>(1),table.get<float>(2));
    }

    glm::vec3 table_to_vec3(const sol::lua_table &table){
        return glm::vec3(table.get<float>(1),table.get<float>(2),table.get<float>(3));
    }
    glm::vec4 table_to_vec4(const sol::lua_table &table){
        return glm::vec4(table.get<float>(1),
                table.get<float>(2),
                table.get<float>(3),
                table.get<float>(4));
    }


}

#endif //BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_H
