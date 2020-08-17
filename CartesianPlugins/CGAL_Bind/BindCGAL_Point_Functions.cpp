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


        // Also you can use below code
        /*
        // add attribute
       //DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Vertex_descriptor, add_int_pointattrib, int);
       //REGISTER_LUA_FUNCTION("add_int_pointattrib", add_int_pointattrib);

        // get attribute
        DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Vertex_descriptor,get_int_pointattrib_ptnum, int);
        DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor,get_int_pointattrib,int);
        REGISTER_LUA_OVERLOAD_FUNCTION("get_int_pointattrib", get_int_pointattrib, get_int_pointattrib_ptnum);

        // set attribute
        DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Vertex_descriptor,set_int_pointattrib_ptnum, int);
        DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor,set_int_pointattrib, int);
        REGISTER_LUA_OVERLOAD_FUNCTION("set_int_pointattrib", set_int_pointattrib, set_int_pointattrib_ptnum);

        // --------------------------------------------------------------  int attribute operation --------------------------------------------------------------------



        // --------------------------------------------------------------  string attribute operation --------------------------------------------------------------------
         // add attribute
        DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Vertex_descriptor,add_string_pointattrib, std::string);
        REGISTER_LUA_FUNCTION("add_string_pointattrib", add_string_pointattrib);

        // get attribute
        DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Vertex_descriptor, get_string_pointattrib_ptnum, std::string);
        DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, get_string_pointattrib, std::string);
        REGISTER_LUA_OVERLOAD_FUNCTION( "get_string_pointattrib", get_string_pointattrib, get_string_pointattrib_ptnum);

        // set attribute
        DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Vertex_descriptor, set_string_pointattrib_ptnum, std::string);
        DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, set_string_pointattrib, std::string);
        REGISTER_LUA_OVERLOAD_FUNCTION( "set_string_pointattrib", set_string_pointattrib, set_string_pointattrib_ptnum);

        // --------------------------------------------------------------  string attribute operation --------------------------------------------------------------------




        // --------------------------------------------------------------  float attribute operation --------------------------------------------------------------------
        // add attribute
        DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Vertex_descriptor, add_float_pointattrib, float);
        REGISTER_LUA_FUNCTION("add_float_pointattrib", add_float_pointattrib);

        // get attribute
        DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Vertex_descriptor, get_float_pointattrib_ptnum, float);
        DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, get_float_pointattrib, float);
        REGISTER_LUA_OVERLOAD_FUNCTION("get_float_pointattrib", get_float_pointattrib,get_float_pointattrib_ptnum);

        // set attribute
        DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Vertex_descriptor, set_float_pointattrib_ptnum, float);
        DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, set_float_pointattrib, float);
        REGISTER_LUA_OVERLOAD_FUNCTION("set_float_pointattrib", set_float_pointattrib, set_float_pointattrib_ptnum);
        // --------------------------------------------------------------  float attribute operation --------------------------------------------------------------------


         // -------------------------------------------------------------- lua table attribute operation --------------------------------------------------------------------
        // add attribute
        DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Vertex_descriptor, add_table_pointattrib, sol::lua_table);
        REGISTER_LUA_FUNCTION(add_table_pointattrib, add_table_pointattrib);

        // get attribute
        DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Vertex_descriptor, get_table_pointattrib_ptnum, sol::lua_table);
        DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, get_table_pointattrib, sol::lua_table);
        REGISTER_LUA_OVERLOAD_FUNCTION(get_table_pointattrib, get_table_pointattrib, get_table_pointattrib_ptnum);

        // set attribute
        DEFINE_SET_ARRITB_PTNUM_FUNCTION(PRE_TYPE::Vertex_descriptor, set_table_pointattrib_ptnum, sol::lua_table);
        DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, set_table_pointattrib, sol::lua_table);
        REGISTER_LUA_OVERLOAD_FUNCTION(set_table_pointattrib, set_table_pointattrib, set_table_pointattrib_ptnum);
        // -------------------------------------------------------------- lua table attribute operation --------------------------------------------------------------------
        */


        //// Register can add/set/get vector vector3 vector4 attribute
        ///*add vector2*/
        //DEFINE_ADD_ATTRIB_FUNCTION(PRE_TYPE::Vertex_descriptor, add_vector2_pointattrib, glm::vec2);
        //REGISTER_LUA_FUNCTION(add_vector2_pointattrib, add_vector2_pointattrib);
        ///*get vector2*/
        //DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, get_vector2_pointattrib,glm::vec2);
        //DEFINE_GET_ATTRIB_ID_FUNCTION(PRE_TYPE::Vertex_descriptor, get_vector2_pointattrib_ptnum, glm::vec2);
        //REGISTER_LUA_FUNCTION(get_vector2_pointattrib, get_vector2_pointattrib, get_vector2_pointattrib_ptnum);
        ///*get vector2*/
        //DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, set_vector2_pointattrib, glm::vec2);
        //DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(PRE_TYPE::Vertex_descriptor, set_vector2_pointattrib_ptnum, glm::vec2);
        //REGISTER_LUA_FUNCTION(set_vector2_pointattrib, set_vector2_pointattrib, set_vector2_pointattrib_ptnum);



        // ------------------------------ Get point values ---------------------------------------------------------------

        // Register get all vertices attribute values of float, return tables values
        auto func = GetAttribValues<PRE_TYPE::Vertex_descriptor, float >::get;
        REGISTER_LUA_FUNCTION(get_float_pointattribvalues, func);
        // Register get all vertices attribute values of int, return tables values
        auto func2 = GetAttribValues<PRE_TYPE::Vertex_descriptor, int >::get;
        REGISTER_LUA_FUNCTION(get_int_pointattribvalues, func2);
        // Register get all vertices attribute values of string, return tables values
        auto func3 = GetAttribValues<PRE_TYPE::Vertex_descriptor, std::string>::get;
        REGISTER_LUA_FUNCTION(get_string_pointattribvalues, func3);
        // Register get all vertices attribute values of table, return tables values
        auto func4 = GetAttribValues<PRE_TYPE::Vertex_descriptor, sol::lua_table>::get;
        REGISTER_LUA_FUNCTION(get_table_pointattribvalues, func4);

        // Register get all vertices attribute values of vector2, return tables values
        auto get_vec2_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec2>::get;
        REGISTER_LUA_FUNCTION(get_vector2_pointattribvalues, get_vec2_values);
        // Register get all vertices attribute values of vector, return tables values
        auto get_vec3_values = GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec3>::get;
        REGISTER_LUA_FUNCTION(get_vector_pointattribvalues, get_vec3_values);
        // Register get all vertices attribute values of vector3, return tables values
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
        auto set_float = SetAttribValues<PRE_TYPE::Vertex_descriptor, float >::set;
        REGISTER_LUA_FUNCTION(set_float_pointattribvalues, set_float);
        // Register set all vertices attribute values of int
        auto set_int = SetAttribValues<PRE_TYPE::Vertex_descriptor, int >::set;
        REGISTER_LUA_FUNCTION(set_int_pointattribvalues, set_int);
        // Register set all vertices attribute values of string
        auto set_string = SetAttribValues<PRE_TYPE::Vertex_descriptor, std::string>::set;
        REGISTER_LUA_FUNCTION(set_string_pointattribvalues, set_string);
        // Register set all vertices attribute values of table
        auto set_luatable = SetAttribValues<PRE_TYPE::Vertex_descriptor, sol::lua_table>::set;
        REGISTER_LUA_FUNCTION(set_table_pointattribvalues, set_luatable);

    }




}