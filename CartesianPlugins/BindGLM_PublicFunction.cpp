//
// Created by admin on 2020/8/6.
//



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLM_Matrix_Helper.h"
#include "GLM_Vec_Helper.h"
#include "BindGLM_PublicFunction.h"
#include <iostream>

namespace Cartesian{
    // vector to table
    auto vec2_to_table(const glm::vec2 &vec){
        std::vector<float> vals;
        vals.emplace_back(vec.x);
        vals.emplace_back(vec.y);
        return sol::as_table(vals);
    }
    auto vec3_to_table(const glm::vec3 &vec){
        std::vector<float> vals;
        vals.emplace_back(vec.x);
        vals.emplace_back(vec.y);
        vals.emplace_back(vec.z);
        return sol::as_table(vals);
    }
    auto vec4_to_table(const glm::vec4 &vec){
        std::vector<float> vals;
        vals.emplace_back(vec.x);
        vals.emplace_back(vec.y);
        vals.emplace_back(vec.z);
        vals.emplace_back(vec.w);
        return sol::as_table(vals);
    }



    template <typename T>
    T transpose(const T&mat){
        return glm::transpose(mat);
    }

    template <typename T>
    T inverse(const T&mat){
        return glm::transpose(mat);
    }



    void BindGLM_PublicFunction::bind(sol::state *lua) {
        // ------------------------ houdini set() function -----------------------------------------
        // vec2
        auto set2 = [](const float &x,const float &y)->glm::vec2{
            return glm::vec2(x,y);
        };
        // vec3
        auto set3 = [](const float &x,const float &y,const float &z)->glm::vec3{
            return glm::vec3(x,y,z);
        };
        // vec4
        auto set4 = [](const float &x,const float &y,const float &z,const float &w)->glm::vec4{
            return glm::vec4(x,y,z,w);
        };
        // mat2 : local m = {vector2.new(1,0),vector2.new(0,1)}
        auto set5 = [](const glm::vec2 &col1, const glm::vec2 &col2){
            return glm::mat2(col1,col2);
        };
        // mat2 from table's : local mat2 = {{1,2},{3,4}}
        auto set5_1 = [](const sol::lua_table &table1, const sol::lua_table &table2){
            if(table1.size() != 2 || table2.size() != 2){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, you should use -> local mat2 = {{1,2},{4,5},{7,8}}\n";
                return glm::mat2(1.0f);
            }
            glm::vec2 col1 (table1.get<float>(1), table1.get<float>(2));
            glm::vec2 col2 (table2.get<float>(1), table2.get<float>(2));
            return glm::mat2(col1,col2);
        };


        // mat3
        auto set6 = [](const glm::vec3 &col1, const glm::vec3 &col2, const glm::vec3 &col3){
            return glm::mat3(col1,col2,col3);
        };

        // mat3 from table's : local mat2 = {{1,2,3},{4,5,6},{7,8,9}}
        auto set6_1 = [](const sol::lua_table &table1, const sol::lua_table &table2, const sol::lua_table &table3){
            if(table1.size() != 3 || table2.size() != 3 || table2.size() != 3){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, you should use -> local mat2 = {{1,2,3},{4,5,6},{7,8,9}}\n";
                return glm::mat3(1.0f);
            }
            glm::vec3 col1 (table1.get<float>(1), table1.get<float>(2), table1.get<float>(3));
            glm::vec3 col2 (table2.get<float>(1), table2.get<float>(2),table2.get<float>(3));
            glm::vec3 col3 (table3.get<float>(1), table3.get<float>(2),table3.get<float>(3));
            return glm::mat3(col1,col2,col3);
        };


        // mat4
        auto set7 = [](const glm::vec4 &col1,
                const glm::vec4 &col2,
                const glm::vec4 &col3,
                const glm::vec4 &col4){
            return glm::mat4(col1,col2,col3,col4);
        };
        // mat3 from table's : local mat2 = {{1,2,3,4},{4,5,6,7},{7,8,9,10}}
        auto set7_1 = [](const sol::lua_table &table1, const sol::lua_table &table2, const sol::lua_table &table3,const sol::lua_table &table4){
            if(table1.size() != 4 || table2.size() != 4 || table2.size() != 4){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, you should use -> local mat2 = {{1,2,3,4},{4,5,6,7},{7,8,9,10},{10,11,12,13}}\n";
                return glm::mat4(1.0f);
            }
            glm::vec4 col1 (table1.get<float>(1), table1.get<float>(2),table1.get<float>(3),table1.get<float>(4));
            glm::vec4 col2 (table2.get<float>(1), table2.get<float>(2),table2.get<float>(3),table2.get<float>(4));
            glm::vec4 col3 (table3.get<float>(1), table3.get<float>(2),table3.get<float>(3),table3.get<float>(4));
            glm::vec4 col4 (table4.get<float>(1), table4.get<float>(2),table4.get<float>(3),table4.get<float>(4));
            return glm::mat4(col1,col2,col3,col4);
        };


        lua->set_function("set",sol::overload(set2,set3,set4,
                set5,set5_1,
                set6,set6_1,
                set7,set7_1));


        // -------------------------- convert vector/mat to lua table ------------------------
        lua->set_function("astable", sol::overload(
                vec2_to_table,
                vec3_to_table,
                vec4_to_table,
                GLM_Matrix_Helper::mat2_to_table,
                GLM_Matrix_Helper::mat3_to_table,
                GLM_Matrix_Helper::mat4_to_table) );

        // ----------------------------- perspective return mat4 ----------------------------------------
        lua->set_function("persp",glm::perspective<float>);



        // ------------------------------- look at --------------------------------------------------
        auto lookat= [](const glm::vec3 & eye, const glm::vec3 &center, const glm::vec3 &up)->glm::mat4{
            return glm::lookAt(eye,center,up);
        };
        auto lookat_table = [](const sol::table &eye,const sol::table &center,const sol::table &up){
            auto condition = eye.valid() && center.valid() && up.valid();
            if(!condition){
                return glm::mat4(1.0f);
            }
            if(eye.size() != 3 || center.size() !=3 || up.size() !=3){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, lookat(table,table,table) must have 3 args\n";
                return glm::mat4(1.0f);
            }

            glm::vec3 eye_pos (eye.get<float>(1),eye.get<float>(2),eye.get<float>(3));
            glm::vec3 center_pos (center.get<float>(1),center.get<float>(2),center.get<float>(3));
            glm::vec3 up_dir (up.get<float>(1),up.get<float>(2),up.get<float>(3));
            return glm::lookAt(eye_pos,center_pos,up_dir);
        };
        lua->set_function("lookat",sol::overload(lookat,lookat_table));

        // -------------------------------- Transpose ----------------------------------------------------
        lua->set_function("transpose",sol::overload(transpose<glm::mat2>,
                        transpose<glm::mat3>,
                        transpose<glm::mat4>));

        // -------------------------------- inverse ----------------------------------------------------
        lua->set_function("invert",sol::overload(inverse<glm::mat2>,
                                                    inverse<glm::mat3>,
                                                    inverse<glm::mat4>));

        // -------------------------------- transform ----------------------------------------------------
        auto transform = [](const glm::mat4 &trans,const glm::vec3 &pos){
            return glm::translate(trans,pos);
        };

        auto transform1 = [](const sol::lua_table &trans,const sol::lua_table &pos){
            if(trans.size() != 16){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, transform(table_trans,table_pos),table_trans should has 16 elements\n";
                return glm::mat4(1);
            }
            if(trans.size() != 3){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, rotate(table_trans,table_pos),table_pos table should has 3 elements\n";
                return glm::mat4(1);
            }
            auto mat = GLM_Matrix_Helper::table_to_mat4(trans);
            auto p   = GLM_Vec_Helper::table_to_vec3(pos);
            return glm::translate(mat,p);
        };
        lua->set_function("transform",sol::overload(transform,transform1));


        // -------------------------------- scale ----------------------------------------------------
        auto scale = [](const glm::mat4 &scale,const glm::vec3 &sv)->glm::mat4{
            return glm::scale(scale,sv);
        };
        auto scale1 = [](const sol::lua_table &scale,const sol::lua_table &sv)->glm::mat4{
            if(scale.size() != 16){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, transform(table_scale,table_pos),table_scale should has 16 elements\n";
                return glm::mat4(1);
            }
            if(sv.size() != 3){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, rotate(table_scale,table_pos),table_pos table should has 3 elements\n";
                return glm::mat4(1);
            }
            auto mat = GLM_Matrix_Helper::table_to_mat4(scale);
            auto s   = GLM_Vec_Helper::table_to_vec3(sv);
            return glm::scale(mat,s);
        };
        lua->set_function("scale",sol::overload(scale,scale1));

        // -------------------------------- rotate ----------------------------------------------------
        auto rotate = [](const glm::mat4 &rot,const float &angle, const glm::vec3 &axis)->glm::mat4{
            return glm::rotate(rot,angle,axis);
        };

        // rotate({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}, 10, {0,1,0} )
        auto rotate1 = [](const sol::lua_table &rot, const float &angle, const sol::lua_table &axis){
            if(rot.size() != 16){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, rotate(table_rot,float_angle,table_axis),table_rot should has 16 elements\n";
                return glm::mat4(1);
            }
            if(axis.size() != 3){
                std::cout << "CARTESIAN::PLUGIN::BIND::ERROR, rotate(table_rot,float_angle,table_axis),table_axis table should has 3 elements\n";
                return glm::mat4(1);
            }
            glm::mat4 rotMat4 = GLM_Matrix_Helper::table_to_mat4(rot);
            glm::vec3 axisVec3= GLM_Vec_Helper::table_to_vec3(axis);
            return glm::rotate(rotMat4,angle,axisVec3);
        };
        lua->set_function("rotate",sol::overload(rotate,rotate1));

        // radians
        lua->set_function("radians",glm::radians<double>);
        // degrees
        lua->set_function("angle",glm::degrees<double>);




    }
}