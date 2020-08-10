//
// Created by admin on 2020/8/6.
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BindGLM_MatrixTable.h"
#include "GLM_Vec_Helper.h"
#include "GLM_Matrix_Helper.h"
#include <iostream>
namespace Cartesian{




    // ------------------------------- Matrix table * Matrix table -------------------------------------
    template <int DIM>
    auto mat_mul_mat(const sol::lua_table &mat1, const sol::lua_table &mat2){


        std::vector<float> values;
        if(mat1.size() != mat2.size() || mat1.empty() || mat2.empty())
        {
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, t_matrix_mul_matrix(table_mat,table_mat) table_mat * table_mat should has same size\n";
            return sol::as_table(values);
        }

        if(!mat1.valid() || !mat2.valid()){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, t_matrix_mul_matrix(table_mat,table_mat) table_mat * table_mat arg not table\n";
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

    // ------------------------------- Matrix table * vector table -------------------------------------
    template <int DIM>
    auto mat_mul_vec(const sol::lua_table &mat, const sol::lua_table &vec){

        std::vector<float> values;
        if(mat.empty()|| vec.empty())
        {
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, t_matrix_mul_vec(table_mat,table_vec) table_mat * table_vec is empty\n";
            return sol::as_table(values);
        }

        if(!mat.valid() || !vec.valid()){
            std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, t_matrix_mul_vec(table_mat,table_vec) table_mat * table_vec arg not table\n";
            return sol::as_table(values);
        }

        if(DIM == 2){
            auto rhmat = GLM_Matrix_Helper::table_to_mat2(mat);
            auto rhvec = GLM_Vec_Helper::table_to_vec2(vec);
            auto mat_mul_vec = rhmat * rhvec;
            return GLM_Vec_Helper::vec2_to_table(mat_mul_vec);
        }
        if(DIM == 3){
            auto rhmat = GLM_Matrix_Helper::table_to_mat3(mat);
            auto rhvec = GLM_Vec_Helper::table_to_vec3(vec);
            auto mat_mul_vec = rhmat * rhvec;
            return GLM_Vec_Helper::vec3_to_table(mat_mul_vec);
        }
        if(DIM == 4){
            auto rhmat = GLM_Matrix_Helper::table_to_mat4(mat);
            auto rhvec = GLM_Vec_Helper::table_to_vec4(vec);
            auto mat_mul_vec = rhmat * rhvec;
            return GLM_Vec_Helper::vec4_to_table(mat_mul_vec);
        }

    }



    // --------------------- convert flat table to matrix ---------------------------
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

        // matrix_table * matrix_table -> vector_table
        lua->set_function("t_matrix_mul_vector4", mat_mul_vec<4>);       // mat4 * vec4
        lua->set_function("t_matrix3_mul_vector", mat_mul_vec<3>);       // mat3 * vec3
        lua->set_function("t_matrix2_mul_vector2", mat_mul_vec<2>);      // mat2 * vec2


        // asmatrix*() function, accept all table to matrix, only support flat table!
        lua->set_function("table_to_matrix2",asmatrix2);
        lua->set_function("table_to_matrix3",asmatrix3);
        lua->set_function("table_to_matrix",asmatrix4);


        // and same time register table_to_vec * functions
        lua->set_function("table_to_vector2",GLM_Vec_Helper::table_to_vec2);
        lua->set_function("table_to_vector",GLM_Vec_Helper::table_to_vec3);
        lua->set_function("table_to_vector4",GLM_Vec_Helper::table_to_vec4);


        // ------------------------ ident table function --------------------------
        lua->set_function("table_ident3",[](){return GLM_Matrix_Helper::mat2_to_table(glm::mat3(1.0f)) ;} );
        lua->set_function("table_ident",[](){return GLM_Matrix_Helper::mat4_to_table(glm::mat4(1.0f)) ;});
        lua->set_function("table_ident2",[](){return GLM_Matrix_Helper::mat3_to_table(glm::mat2(1.0f)) ;});


    }
}