//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_MATRIX_H
#define BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_MATRIX_H
#include <string>
#include <sstream>
#include "sol.hpp"
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

    inline std::string mat3_to_string(const glm::mat3 &mat){
        auto col1 = mat[0];
        auto col2 = mat[1];
        auto col3 = mat[2];
        std::stringstream stream;
        stream.flush();
        stream << col1.x << "," << col2.x << "," << col3.x << std::endl;
        stream << col1.y << "," << col2.y << "," << col3.y << std::endl;
        stream << col1.z << "," << col2.z << "," << col3.z << std::endl;
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

    template <typename T, int DIM>
    float getMatrix(const T &mat, int col, int row) {
        if(col>DIM || row>DIM){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, col,row <=2\n";
            return 0;
        }
        return mat[col][row];
    };

    template <typename T, int DIM>
    void setMatrix(T &mat, int col, int row, const float val){
        if(col>DIM || row>DIM){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, col,row <=2\n";
            return ;
        }
        mat[col][row] = val;
    };



    inline glm::mat2 table_to_mat2(const sol::lua_table &table){
        glm::vec2 col1 (table.get<float>(1),table.get<float>(2));
        glm::vec2 col2 (table.get<float>(3),table.get<float>(4));
        glm::mat2 mat(col1,col2);
        return mat;
    }


    inline glm::mat3 table_to_mat3(const sol::lua_table &table){
        glm::vec3 col1 (table.get<float>(1),table.get<float>(2),table.get<float>(3));
        glm::vec3 col2 (table.get<float>(4),table.get<float>(5),table.get<float>(6));
        glm::vec3 col3 (table.get<float>(7),table.get<float>(8),table.get<float>(9));
        glm::mat3 mat(col1,col2,col3);
        return mat;
    }


    inline glm::mat4 table_to_mat4(const sol::lua_table &table){
        glm::vec4 col1 (table.get<float>(1),table.get<float>(2),table.get<float>(3),table.get<float>(4));
        glm::vec4 col2 (table.get<float>(5),table.get<float>(6),table.get<float>(7),table.get<float>(8));
        glm::vec4 col3 (table.get<float>(9),table.get<float>(10),table.get<float>(11),table.get<float>(12));
        glm::vec4 col4 (table.get<float>(13),table.get<float>(14),table.get<float>(15),table.get<float>(16));
        glm::mat4 mat(col1,col2,col3,col4);
        return mat;
    }



}

















#endif //BUILDINGCTSLUAJIT_BINDGLM_PUBLIC_MATRIX_H
