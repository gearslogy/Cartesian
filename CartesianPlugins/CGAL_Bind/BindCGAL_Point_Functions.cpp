//
// Created by admin on 2020/8/10.
//
#include "CGAL_Attrib_Helper.h"
#include "BindCGAL_Point_Functions.h"

namespace Cartesian {


    using std::string;

    void BindCGAL_Point_Functions::bind(sol::state* lua) {

        auto npts = [](const PRE_TYPE::Mesh& m) {return m.num_vertices(); };
        REGISTER_LUA_FUNCTION(npoints, npts);

        // ---------------------- Get all point attribute names --------------------------------------
        auto getPointsAttribNames = [](const PRE_TYPE::Mesh& mesh) {
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            return sol::as_table(props);
        };
        REGISTER_LUA_FUNCTION(pointattribnames, getPointsAttribNames);


        // ---------------------- Check a point attribute is exist --------------------------------------
        auto hasPointAttrib = [](const PRE_TYPE::Mesh& mesh, const std::string& name)->bool {
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            for (auto& p : props) {
                if (p == name) return true;
            }
            return false;
        };
        REGISTER_LUA_FUNCTION(haspointattrib, hasPointAttrib);



        // --------------------------------------------------------------  int attribute operation --------------------------------------------------------------------

        // Register add_int_pointattrib(mesh, "name", 0 )
        // Register set_int_pointattrib(mesh, "name", id, 0 )
        // Register get_int_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_ATTRIB(PRE_TYPE::Vertex_descriptor, point, int);



        // Register add_float_pointattrib(mesh, "name", set(0,0) )
        // Register set_float_pointattrib(mesh, "name", id, set(0,0) )
        // Register get_float_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_ATTRIB(PRE_TYPE::Vertex_descriptor, point, float);


        // Register add_string_pointattrib(mesh, "name", set(0,0) )
        // Register set_string_pointattrib(mesh, "name", id, set(0,0) )
        // Register get_string_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_ATTRIB(PRE_TYPE::Vertex_descriptor, point, string);


        // Register add_vector2_pointattrib(mesh, "name", set(0,0) )
        // Register set_vector2_pointattrib(mesh, "name", id, set(0,0) )
        // Register get_vector2_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_TABLE_ATTRIB(PRE_TYPE::Vertex_descriptor, point);

        // Register add_vector2_pointattrib(mesh, "name", set(0,0) )
        // Register set_vector2_pointattrib(mesh, "name", id, set(0,0) )
        // Register get_vector2_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_GLM_VEC2_ATTRIB(PRE_TYPE::Vertex_descriptor, point);

        // Register add_vector3_pointattrib(mesh, "name", set(0,0,0) )
        // Register set_vector3_pointattrib(mesh, "name", id, set(0,0,0) )
        // Register get_vector3_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_GLM_VEC3_ATTRIB(PRE_TYPE::Vertex_descriptor, point);

        // Register add_vector4_pointattrib(mesh, "name", set(0,0,0,0) )
        // Register set_vector4_pointattrib(mesh, "name", id, set(0,0,0,0) )
        // Register get_vector4_pointattrib(mesh, "name", id )
        DEFINE_ADD_GET_SET_GLM_VEC4_ATTRIB(PRE_TYPE::Vertex_descriptor, point);


        //GEN lua function:add_matrix2_pointattrib( mesh,"name",ident2() )
        //GEN lua function:set_matrix2_pointattrib( mesh,"name",0 )
        //GEN lua function:get_matrix2_pointattrib( mesh,"name",0 , newmatrix2)
        DEFINE_ADD_GET_SET_GLM_MAT2_ATTRIB(PRE_TYPE::Vertex_descriptor, point);




        //GEN lua function:add_matrix3_pointattrib( mesh,"name",ident3() )
        //GEN lua function:set_matrix3_pointattrib( mesh,"name",0 )
        //GEN lua function:get_matrix3_pointattrib( mesh,"name",0 , newmatrix3)
        DEFINE_ADD_GET_SET_GLM_MAT3_ATTRIB(PRE_TYPE::Vertex_descriptor, point);


        //GEN lua function:add_matrix_pointattrib( mesh,"name",ident() )
        //GEN lua function:set_matrix_pointattrib( mesh,"name",0 )
        //GEN lua function:get_matrix_pointattrib( mesh,"name",0 , newmatrix)
        DEFINE_ADD_GET_SET_GLM_MAT4_ATTRIB(PRE_TYPE::Vertex_descriptor, point);


        // ------------------------------ Get point values ---------------------------------------------------------------

        // Register get all vertices attribute values of float, return tables values
        auto get_float_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, float >::get;
        REGISTER_LUA_FUNCTION(get_float_pointattribvalues, get_float_values);


        // Register get all vertices attribute values of int, return tables values
        auto get_int_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, int >::get;
        REGISTER_LUA_FUNCTION(get_int_pointattribvalues, get_int_values);


        // Register get all vertices attribute values of string, return tables values
        auto get_string_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, std::string>::get;
        REGISTER_LUA_FUNCTION(get_string_pointattribvalues, get_string_values);


        // Register get all vertices attribute values of table, return tables values
        auto get_table_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, sol::lua_table>::get;
        REGISTER_LUA_FUNCTION(get_table_pointattribvalues, get_table_values);


        // Register get all vertices attribute values of vector2, return tables values
        // set_vector2_pointattribvalues(m,"foo_vec2", { {1,2},{1,2},{1,2} })	
        auto get_vec2_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec2>::get;
        REGISTER_LUA_FUNCTION(get_vector2_pointattribvalues, get_vec2_values);


        // Register get all vertices attribute values of vector, return tables values
        // set_vector_pointattribvalues(m,"foo_vec3", { {1,2,3},{3,4,5},{6,7,8} })	
        auto get_vec3_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec3>::get;
        REGISTER_LUA_FUNCTION(get_vector_pointattribvalues, get_vec3_values);


        // Register get all vertices attribute values of vector3, return tables values
        // for 3 points
        // set_vector4_pointattribvalues(m,"foo_vec4", { {1,2,3,4},{1,2,3,4},{1,2,3,4} })	
        auto get_vec4_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec4>::get;
        REGISTER_LUA_FUNCTION(get_vector4_pointattribvalues, get_vec4_values);


        // Register get all vertices attribute values of matrix2, return tables values{{mat2},{mat2} ,{.} ..}
        auto get_mat2_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::mat2>::get;
        REGISTER_LUA_FUNCTION(get_matrix2_pointattribvalues, get_mat2_values);

        // Register get all vertices attribute values of matrix3, return tables values{{mat2},{mat2} ,{.} ..}
        auto get_mat3_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::mat3>::get;
        REGISTER_LUA_FUNCTION(get_matrix3_pointattribvalues, get_mat3_values);

        // Register get all vertices attribute values of matrix, return tables values{{mat2},{mat2} ,{.} ..}
        auto get_mat4_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::mat4>::get;
        REGISTER_LUA_FUNCTION(get_matrix_pointattribvalues, get_mat4_values);



        // ------------------------------ set point values ---------------------------------------------------------------
        // Register set all vertices attribute values of float
        auto set_float_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, float >::set;
        REGISTER_LUA_FUNCTION(set_float_pointattribvalues, set_float_values);
        // Register set all vertices attribute values of int
        auto set_int_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, int >::set;
        REGISTER_LUA_FUNCTION(set_int_pointattribvalues, set_int_values);
        // Register set all vertices attribute values of string
        auto set_string_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, std::string>::set;
        REGISTER_LUA_FUNCTION(set_string_pointattribvalues, set_string_values);
        // Register set all vertices attribute values of table
        auto set_luatable_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, sol::lua_table>::set;
        REGISTER_LUA_FUNCTION(set_table_pointattribvalues, set_luatable_values);
        // Register set all vertices attribute values of vec2
        auto set_vec2_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec2 >::set;
        REGISTER_LUA_FUNCTION(set_vector2_pointattribvalues, set_vec2_values);
        // Register set all vertices attribute values of vec3
        auto set_vec3_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec3 >::set;
        REGISTER_LUA_FUNCTION(set_vector_pointattribvalues, set_vec3_values);

        // Register set all vertices attribute values of vec4
        auto set_vec4_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec4 >::set;
        REGISTER_LUA_FUNCTION(set_vector4_pointattribvalues, set_vec4_values);


        // Register set all vertices attribute values of mat2
        // for 3 points
        //    local mat2_pt0 = astable(ident2() * 100)
        //    local mat2_pt1 = astable(ident2() * 50)
        //    local mat2_pt2 = astable(ident2() * 10)
        //    set_matrix2_pointattribvalues(m, "foo_matrix2", { mat2_pt0, mat2_pt1, mat2_pt2 })
        auto set_mat2_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, glm::mat2 >::set;
        REGISTER_LUA_FUNCTION(set_matrix2_pointattribvalues, set_mat2_values);

        // Register set all vertices attribute values of mat3
        // for 3 points
        //    local mat3_pt0 = astable(ident3() * 100)
        //    local mat3_pt1 = astable(ident3() * 50)
        //    local mat3_pt2 = astable(ident3() * 10)
        //    set_matrix3_pointattribvalues(m, "foo_matrix3", { mat3_pt0, mat3_pt1, mat3_pt2 })
        auto set_mat3_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, glm::mat3 >::set;
        REGISTER_LUA_FUNCTION(set_matrix3_pointattribvalues, set_mat3_values);


        // Register set all vertices attribute values of mat4
        // for 3 points
        //    local mat4_pt0 = astable(ident() * 100)
        //    local mat4_pt1 = astable(ident() * 50)
        //    local mat4_pt2 = astable(ident() * 10)
        //    set_matrix4_pointattribvalues(m, "foo_matrix", { mat4_pt0, mat4_pt1, mat4_pt2 })

        auto set_mat4_values = SetAttribValues<PRE_TYPE::Vertex_descriptor, glm::mat4 >::set;
        REGISTER_LUA_FUNCTION(set_matrix4_pointattribvalues, set_mat4_values);

    }




}