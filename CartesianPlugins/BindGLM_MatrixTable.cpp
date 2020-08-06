//
// Created by admin on 2020/8/6.
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BindGLM_MatrixTable.h"
#include "GLM_Matrix_Helper.h"
#include <iostream>
namespace Cartesian{




    template <int DIM>
    auto mat_mul_mat(const sol::lua_table &mat1, const sol::lua_table &mat2){
        std::vector<float> values;
        if(mat1.size() != mat2.size() || mat1.empty() || mat2.empty())
        {
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, table_mat * table_mat should has same size\n";
            return sol::as_table(values);
        }
        if(DIM == 2) // mat2 * mat2
        {
            auto gmat1 = GLM_Matrix_Helper::table_to_mat2(mat1);
            auto gmat2 = GLM_Matrix_Helper::table_to_mat2(mat2);
            auto mat = gmat1 * gmat2;
            return GLM_Matrix_Helper::mat2_to_table(mat);
        }
        if(DIM == 3) // mat3 * mat3
        {
            auto gmat1 = GLM_Matrix_Helper::table_to_mat3(mat1);
            auto gmat2 = GLM_Matrix_Helper::table_to_mat3(mat2);
            auto mat = gmat1 * gmat2;
            return GLM_Matrix_Helper::mat3_to_table(mat);

        }
        if(DIM == 4)
        {
            auto gmat1 = GLM_Matrix_Helper::table_to_mat4(mat1);
            auto gmat2 = GLM_Matrix_Helper::table_to_mat4(mat2);
            auto mat = gmat1 * gmat2;
            return GLM_Matrix_Helper::mat4_to_table(mat);
        }
    }

    auto asmatrix2 = [](const sol::lua_table &table)->glm::mat2{
        if(table.size() != 4){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, asmatrix2(table)  table count should be 4\n";
            return glm::mat2(1.0);
        }
        return GLM_Matrix_Helper::table_to_mat2(table);
    };

    auto asmatrix3 = [](const sol::lua_table &table)->glm::mat3{
        if(table.size() != 9){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, asmatrix3(table)  table count should be 9\n";
            return glm::mat3(1.0);
        }
        return GLM_Matrix_Helper::table_to_mat3(table);
    };

    auto asmatrix4 = [](const sol::lua_table &table)->glm::mat4{
        if(table.size() != 16){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, asmatrix3(table)  table count should be 9\n";
            return glm::mat4(1.0);
        }
        return GLM_Matrix_Helper::table_to_mat4(table);
    };


    void BindGLM_MatrixTable::bind(sol::state *lua) {
        // matrix_table * matrix_table -> matrix_table
        lua->set_function("t_matrix_mul_matrix", mat_mul_mat<4>);
        lua->set_function("t_matrix3_mul_matrix3", mat_mul_mat<3>);
        lua->set_function("t_matrix2_mul_matrix2", mat_mul_mat<2>);




        // asmatrix*() function, accept all table to matrix, only support flat table!
        lua->set_function("table_to_matrix2",asmatrix2);
        lua->set_function("table_to_matrix3",asmatrix3);
        lua->set_function("table_to_matrix",asmatrix4);
    }
}