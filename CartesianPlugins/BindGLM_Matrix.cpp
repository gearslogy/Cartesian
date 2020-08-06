//
// Created by admin on 2020/8/4.
//

#include "BindGLM_Matrix.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

namespace Cartesian{
    // mat + mat
    template <typename T>
    T mat_add_mat(const T&rhmat1, const T&rhmat2){
        return rhmat1 + rhmat2;
    }
    template <typename T>
    T mat_add_float(const T&rhmat1, const float&v){
        return rhmat1 + v;
    }

    // mat - mat
    template <typename T>
    T mat_sub_mat(const T&rhmat1, const T&rhmat2){
        return rhmat1 - rhmat2;
    }
    template <typename T>
    T mat_sub_float(const T&rhmat1, const float&v){
        return rhmat1 - v;
    }

    // mat * mat
    template <typename T>
    T mat_mul_mat(const T&rhmat1, const T&rhmat2){
        return rhmat1*rhmat2;
    }
    template <typename T>
    T mat_mul_float(const T&rhmat1, const float&rh){
        return rhmat1*rh;
    }


    std::string mat2_to_string(const glm::mat2 &mat){
        auto col1 = mat[0];
        auto col2 = mat[1];
        std::stringstream stream;
        stream.flush();
        stream << col1.x << "," << col2.x << std::endl;
        stream << col1.y << "," << col2.y << std::endl;
        return stream.str();
    }

    std::string mat3_to_string(const glm::mat4 &mat){
        auto col1 = mat[0];
        auto col2 = mat[1];
        auto col3 = mat[2];
        std::stringstream stream;
        stream.flush();
        stream << col1.x << "," << col2.x << "," << col3.x << std::endl;
        stream << col1.y << "," << col2.y << "," << col3.y << std::endl;
        stream << col1.z << "," << col2.z << "," << col3.z << std::endl;
        stream << col1.w << "," << col2.w << "," << col3.w << std::endl;
        return stream.str();
    }


    std::string mat4_to_string(const glm::mat4 &mat){
        auto col1 = mat[0];
        auto col2 = mat[1];
        auto col3 = mat[2];
        auto col4 = mat[3];
        std::stringstream stream;
        stream.flush();
        stream << col1.x << "," << col2.x << "," << col3.x << "," << col4.x << std::endl;
        stream << col1.y << "," << col2.y << "," << col3.y << "," << col4.y << std::endl;
        stream << col1.z << "," << col2.z << "," << col3.z << "," << col4.z << std::endl;
        stream << col1.w << "," << col2.w << "," << col3.w << "," << col4.w << std::endl;
        return stream.str();
    }



    void BindGLM_Matrix::bind(sol::state *lua) {


        // ----------------------- matrix22 ---------------------------------
        sol::usertype<glm::mat2> matrix2 = (*lua).new_usertype<glm::mat2>(
                "matrix2" , // class type
                sol::constructors<glm::mat2(),
                glm::mat2(const float&),
                glm::mat2(glm::vec2, glm::vec2)>(),
                sol::meta_function::addition,        sol::overload(mat_add_mat<glm::mat2>,mat_add_float<glm::mat2>),
                sol::meta_function::subtraction,     sol::overload(mat_sub_mat<glm::mat2>,mat_sub_float<glm::mat2>),
                sol::meta_function::multiplication,  sol::overload(mat_mul_mat<glm::mat2>,mat_mul_float<glm::mat2>),
                sol::meta_function::to_string,       mat2_to_string
        ); // end of user data


        // ----------------------- matrix33 ---------------------------------
        sol::usertype<glm::mat3> matrix3 = (*lua).new_usertype<glm::mat3>(
                "matrix3" , // class type
                sol::constructors<glm::mat3(),glm::mat3(const float &)>(), // matrix3.new() matrix3.new(1)
                sol::meta_function::addition,        sol::overload(mat_add_mat<glm::mat3>,mat_add_float<glm::mat3>),
                sol::meta_function::subtraction,     sol::overload(mat_sub_mat<glm::mat3>,mat_sub_float<glm::mat3>),
                sol::meta_function::multiplication,  sol::overload(mat_mul_mat<glm::mat3>,mat_mul_float<glm::mat3>),
                sol::meta_function::to_string,       mat3_to_string
        ); // end of user data


        // ----------------------- matrix44 ---------------------------------
        sol::usertype<glm::mat4> matrix4 = (*lua).new_usertype<glm::mat4>(
                "matrix" , // class type
                sol::constructors<glm::mat4(),glm::mat4(const float&)>(),
                sol::meta_function::addition,        sol::overload(mat_add_mat<glm::mat4>,mat_add_float<glm::mat4>),
                sol::meta_function::subtraction,     sol::overload(mat_sub_mat<glm::mat4>,mat_sub_float<glm::mat4>),
                sol::meta_function::multiplication,  sol::overload(mat_mul_mat<glm::mat4>,mat_mul_float<glm::mat4>),
                sol::meta_function::to_string,       mat4_to_string
        ); // end of user data







    }


}