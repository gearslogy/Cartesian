//
// Created by admin on 2020/8/10.
//

#pragma once
#define CGAL_NO_GMP 1
#include "CartesianLog.h"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BindCGAL_DefineType.h"
#include "../GLM_Bind/GLM_Matrix_Helper.h"
#include "../GLM_Bind/GLM_Vec_Helper.h"
#include <vector>

// Get attribute was founded()
#define CHECK_ATTRIB_CREATED_STATUS(STATUS,ATTRIB_NAME, ATTRIB_VALUE_TYPE) \
if(!STATUS){\
CARTESIAN_CORE_ERROR("Can not create attrib:{0},{1}, Maybe it's already created with this type: {2}", ATTRIB_NAME, __LINE__,#ATTRIB_VALUE_TYPE);\
}


// check attribute was created()
#define CHECK_ATTRIB_FOUNDED_STATUS(STATUS,ATTRIB_NAME, ATTRIB_VALUE_TYPE) \
if(!STATUS){\
CARTESIAN_CORE_ERROR("Can not find attrib:{0},{1}, Maybe it's already created with this type: {2}", ATTRIB_NAME, __LINE__,#ATTRIB_VALUE_TYPE);\
}






// Register lambda function and body and register to lua
#define DEFINE_ADD_ATTRIB_FUNCTION(SCOPE_TYPE, FUNCTION_NAME, ATTRIB_VALUE_TYPE) \
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const ATTRIB_VALUE_TYPE& default) {\
    PRE_TYPE::Mesh::Property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE> attmap;\
    bool created;\
    boost::tie(attmap, created) = mesh.add_property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE>(name, default);\
    CHECK_ATTRIB_CREATED_STATUS(created,name,ATTRIB_VALUE_TYPE);\
    return created;\
}


// Register lambda function,get attribute , point number index by int type
#define DEFINE_GET_ATTRIB_PTNUM_FUNCTION(SCOPE_TYPE,FUNCTION_NAME, ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name,const int &ptnum ) {\
    SCOPE_TYPE vd(ptnum);\
	PRE_TYPE::Mesh::Property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE> attribMap; \
    bool found;\
    boost::tie(attribMap, found) = mesh.property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE>(name);\
    if (found) {\
        return attribMap[vd];\
	}\
	CARTESIAN_CORE_ERROR("Error get attribute:{0},line:{1},funciton:{2}", name, __LINE__, __FUNCTION__);\
	return ATTRIB_VALUE_TYPE();\
};
// Register lambda function,get attribute , point number index by int type
#define DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(SCOPE_TYPE,FUNCTION_NAME, ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const SCOPE_TYPE& vd) {\
    PRE_TYPE::Mesh::Property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE> attribMap; \
    bool found; \
    boost::tie(attribMap, found) = mesh.property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE>(name); \
    if (found) {\
        return attribMap[vd]; \
	}\
	CHECK_ATTRIB_FOUNDED_STATUS(found,name,ATTRIB_VALUE_TYPE);\
	return ATTRIB_VALUE_TYPE(); \
};


// Register lambda function,set attribute , point number by Vertex_descriptor
#define DEFINE_SET_ARRITB_PTNUM_FUNCTION(SCOPE_TYPE,FUNCTION_NAME,SET_ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const int& vd, const SET_ATTRIB_VALUE_TYPE& value)->void {\
	SCOPE_TYPE vtex(vd);\
	PRE_TYPE::Mesh::Property_map<SCOPE_TYPE, SET_ATTRIB_VALUE_TYPE> attribMap;\
	bool found;\
	boost::tie(attribMap, found) = mesh.property_map<SCOPE_TYPE, SET_ATTRIB_VALUE_TYPE>(name);\
	if (found) {\
		attribMap[vtex] = value;\
		return;\
	}\
	CARTESIAN_CORE_ERROR("Error set not existed attribute:{0}, line:{1}, function:{2}", name, __LINE__, __FUNCTION__);\
};
// Register lambda function,set attribute , point number by Vertex_descriptor
#define DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(SCOPE_TYPE, FUNCTION_NAME,SET_ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const SCOPE_TYPE& vd, const SET_ATTRIB_VALUE_TYPE& value)->void {\
	PRE_TYPE::Mesh::Property_map<SCOPE_TYPE, SET_ATTRIB_VALUE_TYPE> attribMap;\
	bool found;\
	boost::tie(attribMap, found) = mesh.property_map<SCOPE_TYPE, SET_ATTRIB_VALUE_TYPE>(name);\
	if (found) {\
		attribMap[vd] = value;\
		return;\
	}\
	CARTESIAN_CORE_ERROR("Error set not existed attribute:{0}, line:{1}, function:{2}", name, __LINE__, __FUNCTION__);\
};



// Register to lua
#define REGISTER_LUA_FUNCTION(FUNCTION_NAME, FUNCTION) lua->set_function(#FUNCTION_NAME,FUNCTION);
#define REGISTER_LUA_OVERLOAD_FUNCTION(FUNCTION_NAME, ...) lua->set_function(#FUNCTION_NAME,sol::overload(__VA_ARGS__));




// Register add/get/set function for lua for any type
// arg1: PRE_TYPE::Vertex_descritor
// arg2: point/prim/edge/hedge
// arg3: float,int, std::string
// gen function : add_int_pointattrib(mesh,"name",default_value)
// gen function : set_int_pointattrib(mesh,"name",0,set_value)
// gen function : get_int_pointattrib(mesh,"name",0)
#define DEFINE_ADD_GET_SET_ATTRIB(GEOMETRY_SCOPE_TYPE,FUNCTION_SCOPE_NAME, ATTRIBUTE_DATA_TYPE)\
	/*Regsiter the add function*/\
	/**/\
	/**/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, ATTRIBUTE_DATA_TYPE); \
	REGISTER_LUA_FUNCTION(##add_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib,add_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib)\
	/**/\
	/**/\
	/*get*/\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE,get_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib_ptnum, ATTRIBUTE_DATA_TYPE);\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, ATTRIBUTE_DATA_TYPE);\
	REGISTER_LUA_OVERLOAD_FUNCTION(##get_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, get_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, get_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/**/\
	/**/\
	/* set */\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib_ptnum, ATTRIBUTE_DATA_TYPE);\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, ATTRIBUTE_DATA_TYPE);\
	REGISTER_LUA_OVERLOAD_FUNCTION(##set_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, set_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib, set_##ATTRIBUTE_DATA_TYPE##_##FUNCTION_SCOPE_NAME##attrib_ptnum);






// Register add/get/set function for lua for any type
// arg1: PRE_TYPE::Vertex_descritor
// arg2: point/prim/edge/hedge
// arg3: must write : table!
// gen function : add_table_pointattrib(mesh,"name",{})
// gen function : set_table_pointattrib(mesh,"name",0,{1,2,3,4})
// gen function : get_table_pointattrib(mesh,"name",0)
#define DEFINE_ADD_GET_SET_TABLE_ATTRIB(GEOMETRY_SCOPE_TYPE,FUNCTION_SCOPE_NAME)\
	/*Regsiter the add function*/\
	/**/\
	/**/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_table_##FUNCTION_SCOPE_NAME##attrib, sol::lua_table); \
	REGISTER_LUA_FUNCTION(##add_table_##FUNCTION_SCOPE_NAME##attrib,add_table_##FUNCTION_SCOPE_NAME##attrib)\
	/**/\
	/**/\
	/*get*/\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE,get_table_##FUNCTION_SCOPE_NAME##attrib_ptnum, sol::lua_table);\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_table_##FUNCTION_SCOPE_NAME##attrib, sol::lua_table);\
	REGISTER_LUA_OVERLOAD_FUNCTION(##get_table_##FUNCTION_SCOPE_NAME##attrib, get_table_##FUNCTION_SCOPE_NAME##attrib, get_table_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/**/\
	/**/\
	/* set */\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_table_##FUNCTION_SCOPE_NAME##attrib_ptnum, sol::lua_table);\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_table_##FUNCTION_SCOPE_NAME##attrib, sol::lua_table);\
	REGISTER_LUA_OVERLOAD_FUNCTION(##set_table_##FUNCTION_SCOPE_NAME##attrib, set_table_##FUNCTION_SCOPE_NAME##attrib, set_table_##FUNCTION_SCOPE_NAME##attrib_ptnum);


// Gen lua function: add_vector2_pointattrib(mesh,"name",set(0,1))
// if you write DEFINE_ADD_GET_SET_GLM_VEC2_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
//		GEN lua function:add_vector2_pointattrib( mesh,"name",set(0,1))
//      GEN lua function:get_vector2_pointattrib( mesh,"name",0 )
//      GEN lua function:set_vector2_pointattrib( mesh,"name",0 ,set(0,1))
// --------------------------------------------------------------------------------
// if you write DEFINE_ADD_GET_SET_GLM_VEC2_ATTRIB(PRE_TYPE::Face_descriptor, prim);
//      GEN lua function:add_vector2_primattrib( mesh,"name",set(0,1))
//      GEN lua function:get_vector2_primattrib( mesh,"name",0 )
//      GEN lua function:set_vector2_primattrib( mesh,"name",0 ,set(0,1))

#define DEFINE_ADD_GET_SET_GLM_VEC2_ATTRIB(GEOMETRY_SCOPE_TYPE, FUNCTION_SCOPE_NAME)\
	/*add vector2*/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_vector2_##FUNCTION_SCOPE_NAME##attrib, glm::vec2);\
	REGISTER_LUA_FUNCTION(add_vector2_##FUNCTION_SCOPE_NAME##attrib, add_vector2_##FUNCTION_SCOPE_NAME##attrib);\
	/*get vector2*/\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, get_vector2_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::vec2);\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_vector2_##FUNCTION_SCOPE_NAME##attrib, glm::vec2);\
	REGISTER_LUA_OVERLOAD_FUNCTION(get_vector2_##FUNCTION_SCOPE_NAME##attrib, get_vector2_##FUNCTION_SCOPE_NAME##attrib, get_vector2_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/*set vector2*/\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_vector2_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::vec2);\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_vector2_##FUNCTION_SCOPE_NAME##attrib, glm::vec2);\
	REGISTER_LUA_OVERLOAD_FUNCTION(set_vector2_##FUNCTION_SCOPE_NAME##attrib, set_vector2_##FUNCTION_SCOPE_NAME##attrib, set_vector2_##FUNCTION_SCOPE_NAME##attrib_ptnum);


// if you write DEFINE_ADD_GET_SET_GLM_VEC3_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
//		GEN lua function:add_vector_pointattrib( mesh,"name",set(0,1,0))
//      GEN lua function:get_vector_pointattrib( mesh,"name",0 )
//      GEN lua function:set_vector_pointattrib( mesh,"name",0 ,set(1,0,0))
// --------------------------------------------------------------------------------
// if you write DEFINE_ADD_GET_SET_GLM_VEC3_ATTRIB(PRE_TYPE::Face_descriptor, prim);
//      GEN lua function:add_vector_primattrib( mesh,"name",set(0,1,0))
//      GEN lua function:get_vector_primattrib( mesh,"name",0 )
//      GEN lua function:set_vector_primattrib( mesh,"name",0 ,set(1,1,0))
#define DEFINE_ADD_GET_SET_GLM_VEC3_ATTRIB(GEOMETRY_SCOPE_TYPE, FUNCTION_SCOPE_NAME)\
	/*add vector2*/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_vector_##FUNCTION_SCOPE_NAME##attrib, glm::vec3);\
	REGISTER_LUA_FUNCTION(add_vector_##FUNCTION_SCOPE_NAME##attrib, add_vector_##FUNCTION_SCOPE_NAME##attrib);\
	/*get vector2*/\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_vector_##FUNCTION_SCOPE_NAME##attrib, glm::vec3);\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, get_vector_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::vec3);\
	REGISTER_LUA_OVERLOAD_FUNCTION(get_vector_##FUNCTION_SCOPE_NAME##attrib, get_vector_##FUNCTION_SCOPE_NAME##attrib, get_vector_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/*get vector2*/\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_vector_##FUNCTION_SCOPE_NAME##attrib, glm::vec3);\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_vector_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::vec3);\
	REGISTER_LUA_OVERLOAD_FUNCTION(set_vector_##FUNCTION_SCOPE_NAME##attrib, set_vector_##FUNCTION_SCOPE_NAME##attrib, set_vector_##FUNCTION_SCOPE_NAME##attrib_ptnum);


// Gen lua function: add_vector4_pointattrib(mesh,"name",set(0,1,0,1))
// if you write DEFINE_ADD_GET_SET_GLM_VEC4_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
//		GEN lua function:add_vector4_pointattrib( mesh,"name",set(0,1,0,1))
//      GEN lua function:get_vector4_pointattrib( mesh,"name",0 )
//      GEN lua function:set_vector4_pointattrib( mesh,"name",0 ,set(0,1,1,2))
// --------------------------------------------------------------------------------
// if you write DEFINE_ADD_GET_SET_GLM_VEC4_ATTRIB(PRE_TYPE::Face_descriptor, prim);
//      GEN lua function:add_vector4_primattrib( mesh,"name",set(0,1,1,1))
//      GEN lua function:get_vector4_primattrib( mesh,"name",0 )
//      GEN lua function:set_vector4_primattrib( mesh,"name",0 ,set(0,1,0,1))
#define DEFINE_ADD_GET_SET_GLM_VEC4_ATTRIB(GEOMETRY_SCOPE_TYPE, FUNCTION_SCOPE_NAME)\
	/*add vector2*/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_vector4_##FUNCTION_SCOPE_NAME##attrib, glm::vec4);\
	REGISTER_LUA_FUNCTION(add_vector4_##FUNCTION_SCOPE_NAME##attrib, add_vector4_##FUNCTION_SCOPE_NAME##attrib);\
	/*get vector2*/\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_vector4_##FUNCTION_SCOPE_NAME##attrib, glm::vec4);\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, get_vector4_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::vec4);\
	REGISTER_LUA_OVERLOAD_FUNCTION(get_vector4_##FUNCTION_SCOPE_NAME##attrib, get_vector4_##FUNCTION_SCOPE_NAME##attrib, get_vector4_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/*get vector2*/\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_vector4_##FUNCTION_SCOPE_NAME##attrib, glm::vec4);\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_vector4_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::vec4);\
	REGISTER_LUA_OVERLOAD_FUNCTION(set_vector4_##FUNCTION_SCOPE_NAME##attrib, set_vector4_##FUNCTION_SCOPE_NAME##attrib, set_vector4_##FUNCTION_SCOPE_NAME##attrib_ptnum);







namespace Cartesian
{
	// function like houdini's api return tuple of table
	template <typename geoScope, typename T>
	class GetAttribValues
	{
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName){
			std::vector <float> values;
			return sol::as_table(values);
		}

	};

	// For General vertex 
	template <typename T>
	class GetAttribValues<PRE_TYPE::Vertex_descriptor,T>
	{
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName){
			std::vector <T> values;
			PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, T> attribMap;
			bool found;
			boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, T>(attribName);
			CHECK_ATTRIB_FOUNDED_STATUS(found,attribName, T);
			if (found) {
				for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices())
				{
					values.push_back(attribMap[vd]);
				}
			}
			return sol::as_table(values);
		}

	};


	// For General vertex glm::vec2 type, return a flat tables,{x1,y1, x1,y1 ...}
	template <>
	class GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec2> {
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
			std::vector <float> values;
			PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, glm::vec2> attribMap;
			bool found;
			boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, glm::vec2>(attribName);
			CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::vec2);
			if (!found) {
				return sol::as_table(values);
			}

			for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices()) {
				auto val = attribMap[vd];
				values.push_back(val.x);
				values.push_back(val.y);
			}
			return sol::as_table(values);
		}
	};



	// For General vertex glm::vec3 type, return flat tables {x1,y1,z1, x1,y1,z1 ...}
	template <>
	class GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec3>{
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
			std::vector <float> values;
			PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, glm::vec3> attribMap;
			bool found;
			boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, glm::vec3>(attribName);
			CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::vec3);
			if (!found) {
				return sol::as_table(values);
			}

			for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices()) {
				auto val = attribMap[vd];
				values.push_back(val.x);
				values.push_back(val.y);
				values.push_back(val.z);
			}
			return sol::as_table(values);
		}
	};


	// For General vertex glm::vec4 type  return a flat tables,{x1,y1,z1,w1, x1,y1,z1,w1 ...}
	template <>
	class GetAttribValues<PRE_TYPE::Vertex_descriptor, glm::vec4>{
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
			std::vector <float> values;
			PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, glm::vec4> attribMap;
			bool found;
			boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, glm::vec4>(attribName);
			CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::vec4);
			if (!found) {
				return sol::as_table(values);
			}

			for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices()) {
				auto val = attribMap[vd];
				values.push_back(val.x);
				values.push_back(val.y);
				values.push_back(val.z);
				values.push_back(val.w);
			}
			return sol::as_table(values);
		}
	};



	template <>
	class GetAttribValues<PRE_TYPE::Vertex_descriptor, sol::lua_table>
	{
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
			std::vector <sol::lua_table> values;
			PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, sol::lua_table> attribMap;
			bool found;
			boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, sol::lua_table>(attribName);
			CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, sol::lua_table);
			if (found) {
				for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices()) {
					// attribMap[vd] is sol::lua_table
					values.push_back(attribMap[vd]); // add a table to a table, lua:{ {},{},{}... }
				}
			}
			return sol::as_table(values);
		}
	};



}// end of name space 
