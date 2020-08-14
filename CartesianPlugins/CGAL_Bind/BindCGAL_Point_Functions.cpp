//
// Created by admin on 2020/8/10.
//
#include "CGAL_Attrib_Helper.h"
#include "BindCGAL_Point_Functions.h"

namespace Cartesian{


	using std::string;

    void BindCGAL_Point_Functions::bind(sol::state *lua) {

        auto npts = [](const PRE_TYPE::Mesh& m) {return m.num_vertices(); };
        REGISTER_LUA_FUNCTION(npoints, npts);

        // ---------------------- Get all point attribute names --------------------------------------
        auto getPointsAttribNames = [](const PRE_TYPE::Mesh& mesh){
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            return sol::as_table(props);
        };
        REGISTER_LUA_FUNCTION(pointattribnames, getPointsAttribNames);


        // ---------------------- Check a point attribute is exist --------------------------------------
        auto hasPointAttrib= [](const PRE_TYPE::Mesh & mesh, const std::string &name)->bool{
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            for(auto &p: props){
                if(p == name) return true;
            }
            return false;
        };
        REGISTER_LUA_FUNCTION(haspointattrib, hasPointAttrib);

       

		// --------------------------------------------------------------  int attribute operation --------------------------------------------------------------------
		
		DEFINE_ADD_GET_SET_ATTRIB(PRE_TYPE::Vertex_descriptor, point, int);
		DEFINE_ADD_GET_SET_ATTRIB(PRE_TYPE::Vertex_descriptor, point, float);
		DEFINE_ADD_GET_SET_ATTRIB(PRE_TYPE::Vertex_descriptor, point, string);
		DEFINE_ADD_GET_SET_TABLE_ATTRIB(PRE_TYPE::Vertex_descriptor, point);

		DEFINE_ADD_GET_SET_GLM_VEC2_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
		DEFINE_ADD_GET_SET_GLM_VEC3_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
		DEFINE_ADD_GET_SET_GLM_VEC4_ATTRIB(PRE_TYPE::Vertex_descriptor, point);

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
	




        // Register get tables values
        auto func = GetAttribValues<PRE_TYPE::Vertex_descriptor, float>::get;
        REGISTER_LUA_FUNCTION(get_float_pointattribvalues, func);

		auto func2 = GetAttribValues<PRE_TYPE::Vertex_descriptor, int>::get;
		REGISTER_LUA_FUNCTION(get_int_pointattribvalues, func2);

		auto func3 = GetAttribValues<PRE_TYPE::Vertex_descriptor, std::string>::get;
		REGISTER_LUA_FUNCTION(get_string_pointattribvalues, func3);

		auto get_vec2_values = GetAttribValuesT::get;
		REGISTER_LUA_FUNCTION(get_vector2_pointattribvalues, get_vec2_values);



		auto funcTemp = GetAttribValues<PRE_TYPE::Vertex_descriptor, sol::lua_table>::get;
		REGISTER_LUA_FUNCTION(get_table_pointattribvalues, funcTemp);


    }


   

}