//
// Created by admin on 2020/8/4.
//

#include "BindGLM_Vec.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace  std;


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
        std::string ret =  to_string(vec.x) + std::string(",") + to_string(vec.y) ;
        return ret;
    }
    std::string vec3ToString(const glm::vec3 &vec){
        std::string ret =  to_string(vec.x) + std::string(",") + to_string(vec.y) + std::string(",") + std::to_string(vec.z);
        return ret;
    }
    std::string vec4ToString(const glm::vec4 &vec){
        std::string ret =  to_string(vec.x) + std::string(",") + to_string(vec.y) + std::string(",") + std::to_string(vec.z)+ std::string(",") + std::to_string(vec.w);
        return ret;
    }



    void BindGLM_Vec::bind(sol::state * lua) {

        // ----------------------- vector3 ---------------------------------
        sol::usertype<glm::vec2> vector2 = (*lua).new_usertype<glm::vec2>(
                "vector" , // class type
                sol::constructors<glm::vec2(),glm::vec2(float,float)>(),
                sol::meta_function::addition,        sol::overload(vec_add_vec<glm::vec2>,vec_add_float<glm::vec2>),
                sol::meta_function::subtraction,     sol::overload(vec_sub_vec<glm::vec2>,vec_sub_float<glm::vec2>),
                sol::meta_function::multiplication,  sol::overload(vec_mul_vec<glm::vec2>,vec_mul_float<glm::vec2>),
                sol::meta_function::division,        sol::overload(vec_div_vec<glm::vec2>,vec_div_float<glm::vec2>),
                sol::meta_function::equal_to,        vec_equal_vec<glm::vec2>,
                sol::meta_function::to_string,       vec2ToString
        ); // end of user data

        vector2["x"] = &glm::vec2::x;
        vector2["y"] = &glm::vec2::y;
        vector2["length"] = &glm::vec2::length;





        // ----------------------- vector3 ---------------------------------
        sol::usertype<glm::vec3> vector3 = (*lua).new_usertype<glm::vec3>(
                "vector" , // class type
                sol::constructors<glm::vec3(),glm::vec3(float,float,float)>(),
                        sol::meta_function::addition,        sol::overload(vec_add_vec<glm::vec3>,vec_add_float<glm::vec3>),
                        sol::meta_function::subtraction,     sol::overload(vec_sub_vec<glm::vec3>,vec_sub_float<glm::vec3>),
                        sol::meta_function::multiplication,  sol::overload(vec_mul_vec<glm::vec3>,vec_mul_float<glm::vec3>),
                        sol::meta_function::division,        sol::overload(vec_div_vec<glm::vec3>,vec_div_float<glm::vec3>),
                        sol::meta_function::equal_to,        vec_equal_vec<glm::vec3>,
                        sol::meta_function::to_string,       vec3ToString
                        ); // end of user data

        vector3["x"] = &glm::vec3::x;
        vector3["y"] = &glm::vec3::y;
        vector3["z"] = &glm::vec3::z;
        vector3["length"] = &glm::vec3::length;


        // ----------------------- vector4 ---------------------------------
        sol::usertype<glm::vec4> vector4 = (*lua).new_usertype<glm::vec4>(
                "vector4" , // class type
                sol::constructors<glm::vec4(),glm::vec4(float,float,float,float)>(),
                sol::meta_function::addition,        sol::overload(vec_add_vec<glm::vec4>,vec_add_float<glm::vec4>),
                sol::meta_function::subtraction,     sol::overload(vec_sub_vec<glm::vec4>,vec_sub_float<glm::vec4>),
                sol::meta_function::multiplication,  sol::overload(vec_mul_vec<glm::vec4>,vec_mul_float<glm::vec4>),
                sol::meta_function::division,        sol::overload(vec_div_vec<glm::vec4>,vec_div_float<glm::vec4>),
                sol::meta_function::equal_to,        vec_equal_vec<glm::vec4>,
                sol::meta_function::to_string,        vec4ToString
        ); // end of user data

        vector4["x"] = &glm::vec4::x;
        vector4["y"] = &glm::vec4::y;
        vector4["z"] = &glm::vec4::z;
        vector4["w"] = &glm::vec4::w;
        vector4["length"] = &glm::vec4::length;


        // length
        auto len = [](const glm::vec3 &vec3)->float {
            return glm::length(vec3);
        };
        lua->set_function("length",len );

        // normalize
        auto norm = [](const glm::vec3 &vec3)->glm::vec3 {
            return glm::normalize(vec3);
        };
        lua->set_function("normalize",norm );

        // houdini set() function
        auto set2 = [](const float &x,const float &y)->glm::vec2{
                    return glm::vec2(x,y);
        };
        auto set3 = [](const float &x,const float &y,const float &z)->glm::vec3{
            return glm::vec3(x,y,z);
        };
        auto set4 = [](const float &x,const float &y,const float &z,const float &w)->glm::vec4{
            return glm::vec4(x,y,z,w);
        };

        lua->set_function("set",sol::overload(set2,set3,set4));

    }
}

