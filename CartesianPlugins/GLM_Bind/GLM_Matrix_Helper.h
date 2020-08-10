//
// Created by admin on 2020/8/6.
//

#ifndef BUILDINGCTSLUAJIT_GLM_MATRIX_HELPER_H
#define BUILDINGCTSLUAJIT_GLM_MATRIX_HELPER_H
#include <string>
#include <sstream>
#include "sol.hpp"
#include <vector>
namespace Cartesian{

    class GLM_Matrix_Helper{
    public:
        template <typename T, int DIM>
        static float getMatrix(const T &mat, int col, int row) {
            if(col>DIM || row>DIM){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, getmatrix(mat,col,row) col,row <=2\n";
                return 0;
            }
            return mat[col][row];
        };

        template <typename T, int DIM>
        static void setMatrix(T &mat, int col, int row, const float val){
            if(col>DIM || row>DIM){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, setmatrix(mat,col,row,val) col,row <=2\n";
                return ;
            }
            mat[col][row] = val;
        };


        static inline auto mat2_to_table(const glm::mat2 &mat){
            std::vector<float> values; // num should 4
            // insert col1
            values.emplace_back(mat[0].x);
            values.emplace_back(mat[0].y);
            // insert col2
            values.emplace_back(mat[1].x);
            values.emplace_back(mat[1].y);
            return sol::as_table(values);
        }

        static inline auto mat3_to_table(const glm::mat3 &mat)
        {
            std::vector<float> values; // num should 4
            // insert col1
            values.emplace_back(mat[0].x);
            values.emplace_back(mat[0].y);
            values.emplace_back(mat[0].z);
            // insert col2
            values.emplace_back(mat[1].x);
            values.emplace_back(mat[1].y);
            values.emplace_back(mat[1].z);
            // insert col3
            values.emplace_back(mat[2].x);
            values.emplace_back(mat[2].y);
            values.emplace_back(mat[2].z);
            return sol::as_table(values);
        }

        static inline auto mat4_to_table(const glm::mat4 &mat){
            std::vector<float> values; // num should 4
            // insert col1
            values.emplace_back(mat[0].x);
            values.emplace_back(mat[0].y);
            values.emplace_back(mat[0].z);
            values.emplace_back(mat[0].w);
            // insert col2
            values.emplace_back(mat[1].x);
            values.emplace_back(mat[1].y);
            values.emplace_back(mat[1].z);
            values.emplace_back(mat[1].w);
            // insert col3
            values.emplace_back(mat[2].x);
            values.emplace_back(mat[2].y);
            values.emplace_back(mat[2].z);
            values.emplace_back(mat[2].w);
            //insert col4
            values.emplace_back(mat[3].x);
            values.emplace_back(mat[3].y);
            values.emplace_back(mat[3].z);
            values.emplace_back(mat[3].w);

            return sol::as_table(values);
        }


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
