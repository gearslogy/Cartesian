//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_GLM_MATRIX_HELPER_H
#define BUILDINGCTSLUAJIT_GLM_MATRIX_HELPER_H
#include <string>
#include <sstream>
#include "sol.hpp"
namespace Cartesian{

    class GLM_Matrix_Helper{
    public:
        template <typename T, int DIM>
        static float getMatrix(const T &mat, int col, int row) {
            if(col>DIM || row>DIM){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, col,row <=2\n";
                return 0;
            }
            return mat[col][row];
        };

        template <typename T, int DIM>
        static void setMatrix(T &mat, int col, int row, const float val){
            if(col>DIM || row>DIM){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, col,row <=2\n";
                return ;
            }
            mat[col][row] = val;
        };



        static inline glm::mat2 table_to_mat2(const sol::lua_table &table){
            glm::vec2 col1 (table.get<float>(1),table.get<float>(2));
            glm::vec2 col2 (table.get<float>(3),table.get<float>(4));
            glm::mat2 mat(col1,col2);
            return mat;
        }


        static inline glm::mat3 table_to_mat3(const sol::lua_table &table){
            glm::vec3 col1 (table.get<float>(1),table.get<float>(2),table.get<float>(3));
            glm::vec3 col2 (table.get<float>(4),table.get<float>(5),table.get<float>(6));
            glm::vec3 col3 (table.get<float>(7),table.get<float>(8),table.get<float>(9));
            glm::mat3 mat(col1,col2,col3);
            return mat;
        }


        static inline glm::mat4 table_to_mat4(const sol::lua_table &table){
            glm::vec4 col1 (table.get<float>(1),table.get<float>(2),table.get<float>(3),table.get<float>(4));
            glm::vec4 col2 (table.get<float>(5),table.get<float>(6),table.get<float>(7),table.get<float>(8));
            glm::vec4 col3 (table.get<float>(9),table.get<float>(10),table.get<float>(11),table.get<float>(12));
            glm::vec4 col4 (table.get<float>(13),table.get<float>(14),table.get<float>(15),table.get<float>(16));
            glm::mat4 mat(col1,col2,col3,col4);
            return mat;
        }


    };
}

















#endif //BUILDINGCTSLUAJIT_GLM_MATRIX_HELPER_H
