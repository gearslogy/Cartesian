//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_MATRIX_H
#define BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_MATRIX_H
#include <string>
#include <sstream>
namespace Cartesian{


    // mat + mat
    template <typename T>
    inline T mat_add_mat(const T&rhmat1, const T&rhmat2){
        return rhmat1 + rhmat2;
    }
    template <typename T>
    inline T mat_add_float(const T&rhmat1, const float&v){
        return rhmat1 + v;
    }

    // mat - mat
    template <typename T>
    inline T mat_sub_mat(const T&rhmat1, const T&rhmat2){
        return rhmat1 - rhmat2;
    }
    template <typename T>
    inline T mat_sub_float(const T&rhmat1, const float&v){
        return rhmat1 - v;
    }

    // mat * mat
    template <typename T>
    inline T mat_mul_mat(const T&rhmat1, const T&rhmat2){
        return rhmat1*rhmat2;
    }
    template <typename T>
    inline T mat_mul_float(const T&rhmat1, const float&rh){
        return rhmat1*rh;
    }
    // mat * vector
    template <typename matType, typename vecType>
    inline vecType mat_mul_vec(const matType&mat, const vecType&vec){
        return mat*vec;
    }


    inline std::string mat2_to_string(const glm::mat2 &mat){
        auto col1 = mat[0];
        auto col2 = mat[1];
        std::stringstream stream;
        stream.flush();
        stream << col1.x << "," << col2.x << std::endl;
        stream << col1.y << "," << col2.y << std::endl;
        return stream.str();
    }

    inline std::string mat3_to_string(const glm::mat4 &mat){
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


    inline std::string mat4_to_string(const glm::mat4 &mat){
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







}

















#endif //BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_MATRIX_H
