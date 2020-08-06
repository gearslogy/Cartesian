//
// Created by admin on 2020/8/4.
//


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

#include "BindGLM_Public_Matrix.h"
#include "BindGLM_Matrix.h"

namespace Cartesian{

    void BindGLM_Matrix::bind(sol::state *lua) {

        // ----------------------- matrix22 ---------------------------------
        sol::usertype<glm::mat2> matrix2 = (*lua).new_usertype<glm::mat2>(
                "matrix2" , // class type
                sol::constructors<glm::mat2(),
                glm::mat2(const float&),
                glm::mat2(glm::vec2, glm::vec2)>(),
                sol::meta_function::addition,        sol::overload(mat_add_mat<glm::mat2>,mat_add_float<glm::mat2>),
                sol::meta_function::subtraction,     sol::overload(mat_sub_mat<glm::mat2>,mat_sub_float<glm::mat2>),
                sol::meta_function::multiplication,  sol::overload(mat_mul_mat<glm::mat2>,mat_mul_float<glm::mat2>,mat_mul_vec<glm::mat2,glm::vec2>),
                sol::meta_function::to_string,       mat2_to_string
        ); // end of user data



        // ----------------------- matrix33 ---------------------------------
        sol::usertype<glm::mat3> matrix3 = (*lua).new_usertype<glm::mat3>(
                "matrix3" , // class type
                sol::constructors<glm::mat3(),glm::mat3(const float &)>(), // matrix3.new() matrix3.new(1)
                sol::meta_function::addition,        sol::overload(mat_add_mat<glm::mat3>,mat_add_float<glm::mat3>),
                sol::meta_function::subtraction,     sol::overload(mat_sub_mat<glm::mat3>,mat_sub_float<glm::mat3>),
                sol::meta_function::multiplication,  sol::overload(mat_mul_mat<glm::mat3>,mat_mul_float<glm::mat3>,mat_mul_vec<glm::mat3,glm::vec3>),
                sol::meta_function::to_string,       mat3_to_string
        ); // end of user data


        // ----------------------- matrix44 ---------------------------------
        sol::usertype<glm::mat4> matrix4 = (*lua).new_usertype<glm::mat4>(
                "matrix" , // class type
                sol::constructors<glm::mat4(),glm::mat4(const float&)>(),
                sol::meta_function::addition,        sol::overload(mat_add_mat<glm::mat4>,mat_add_float<glm::mat4>),
                sol::meta_function::subtraction,     sol::overload(mat_sub_mat<glm::mat4>,mat_sub_float<glm::mat4>),
                sol::meta_function::multiplication,  sol::overload(mat_mul_mat<glm::mat4>,mat_mul_float<glm::mat4>,mat_mul_vec<glm::mat4,glm::vec4>),
                sol::meta_function::to_string,       mat4_to_string
        ); // end of user data







    }


}