//
// Created by admin on 2020/8/6.
//



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    auto mat2_to_table(const glm::mat2 &mat){
        std::vector<float> values; // num should 4
        // insert col1
        values.emplace_back(mat[0].x);
        values.emplace_back(mat[0].y);
        // insert col2
        values.emplace_back(mat[1].x);
        values.emplace_back(mat[1].y);
        return sol::as_table(values);
    }

    auto mat3_to_table(const glm::mat3 &mat){
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

    auto mat4_to_table(const glm::mat4 &mat){
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
        // mat2
        auto set5 = [](const glm::vec2 &col1, const glm::vec2 &col2){
            return glm::mat2(col1,col2);
        };
        // mat3
        auto set6 = [](const glm::vec3 &col1, const glm::vec3 &col2, const glm::vec3 &col3){
            return glm::mat3(col1,col2,col3);
        };
        // mat4
        auto set7 = [](const glm::vec4 &col1,
                const glm::vec4 &col2,
                const glm::vec4 &col3,
                const glm::vec4 &col4){
            return glm::mat4(col1,col2,col3,col4);
        };


        lua->set_function("set",sol::overload(set2,set3,set4,
                set5,set6,set7));


        // -------------------------- convert vector/mat to lua table ------------------------
        lua->set_function("astable", sol::overload(
                vec2_to_table,
                vec3_to_table,
                vec4_to_table,
                mat2_to_table,
                mat3_to_table,
                mat4_to_table) );

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


    }
}