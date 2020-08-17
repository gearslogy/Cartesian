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


// find attribute 
#define READY_FIND_ATTRIBUTE(SCOPE_TYPE,ATTRIB_NAME,ATTRIB_VALUE_TYPE)\
PRE_TYPE::Mesh::Property_map<SCOPE_TYPE,ATTRIB_VALUE_TYPE> attribMap;\
bool found;\
boost::tie(attribMap, found) = mesh.property_map<SCOPE_TYPE, ATTRIB_NAME>(ATTRIB_NAME);\
CHECK_ATTRIB_FOUNDED_STATUS(found, ATTRIB_NAME, ATTRIB_VALUE_TYPE);



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


// if you write DEFINE_ADD_GET_SET_GLM_MAT2_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
//		GEN lua function:add_matrix2_pointattrib( mesh,"name",ident2() )
//      GEN lua function:set_matrix2_pointattrib( mesh,"name",0 )
//      GEN lua function:get_matrix2_pointattrib( mesh,"name",0 , newmatrix2)
#define DEFINE_ADD_GET_SET_GLM_MAT2_ATTRIB(GEOMETRY_SCOPE_TYPE, FUNCTION_SCOPE_NAME)\
	/*add matrix2*/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_matrix2_##FUNCTION_SCOPE_NAME##attrib, glm::mat2);\
	REGISTER_LUA_FUNCTION(add_matrix2_##FUNCTION_SCOPE_NAME##attrib, add_matrix2_##FUNCTION_SCOPE_NAME##attrib);\
	/*get matrix2*/\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_matrix2_##FUNCTION_SCOPE_NAME##attrib, glm::mat2);\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, get_matrix2_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::mat2);\
	REGISTER_LUA_OVERLOAD_FUNCTION(get_matrix2_##FUNCTION_SCOPE_NAME##attrib, get_matrix2_##FUNCTION_SCOPE_NAME##attrib, get_matrix2_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/*get matrix2*/\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_matrix2_##FUNCTION_SCOPE_NAME##attrib, glm::mat2);\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_matrix2_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::mat2);\
	REGISTER_LUA_OVERLOAD_FUNCTION(set_matrix2_##FUNCTION_SCOPE_NAME##attrib, set_matrix2_##FUNCTION_SCOPE_NAME##attrib, set_matrix2_##FUNCTION_SCOPE_NAME##attrib_ptnum);



// if you write DEFINE_ADD_GET_SET_GLM_MAT3_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
//		GEN lua function:add_matrix_pointattrib(mesh,"name",ident3())
//      GEN lua function:set_matrix_pointattrib( mesh,"name",0 )
//      GEN lua function:get_matrix_pointattrib( mesh,"name",0 ,newmatrix3)
#define DEFINE_ADD_GET_SET_GLM_MAT3_ATTRIB(GEOMETRY_SCOPE_TYPE, FUNCTION_SCOPE_NAME)\
	/*add matrix3*/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_matrix3_##FUNCTION_SCOPE_NAME##attrib, glm::mat3);\
	REGISTER_LUA_FUNCTION(add_matrix3_##FUNCTION_SCOPE_NAME##attrib, add_matrix3_##FUNCTION_SCOPE_NAME##attrib);\
	/*get matrix3*/\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_matrix3_##FUNCTION_SCOPE_NAME##attrib, glm::mat3);\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, get_matrix3_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::mat3);\
	REGISTER_LUA_OVERLOAD_FUNCTION(get_matrix3_##FUNCTION_SCOPE_NAME##attrib, get_matrix3_##FUNCTION_SCOPE_NAME##attrib, get_matrix3_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/*get matrix3*/\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_matrix3_##FUNCTION_SCOPE_NAME##attrib, glm::mat3);\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_matrix3_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::mat3);\
	REGISTER_LUA_OVERLOAD_FUNCTION(set_matrix3_##FUNCTION_SCOPE_NAME##attrib, set_matrix3_##FUNCTION_SCOPE_NAME##attrib, set_matrix3_##FUNCTION_SCOPE_NAME##attrib_ptnum);


// if you write DEFINE_ADD_GET_SET_GLM_MAT4_ATTRIB(PRE_TYPE::Vertex_descriptor, point);
//		GEN lua function:add_matrix_pointattrib( mesh,"name", ident())
//      GEN lua function:set_matrix_pointattrib( mesh,"name",0 )
//      GEN lua function:get_matrix_pointattrib( mesh,"name",0 , newmatrix4 )
#define DEFINE_ADD_GET_SET_GLM_MAT4_ATTRIB(GEOMETRY_SCOPE_TYPE, FUNCTION_SCOPE_NAME)\
	/*add matrix*/\
	DEFINE_ADD_ATTRIB_FUNCTION(GEOMETRY_SCOPE_TYPE, add_matrix_##FUNCTION_SCOPE_NAME##attrib, glm::mat4);\
	REGISTER_LUA_FUNCTION(add_matrix_##FUNCTION_SCOPE_NAME##attrib, add_matrix_##FUNCTION_SCOPE_NAME##attrib);\
	/*get matrix*/\
	DEFINE_GET_ATTRIB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, get_matrix_##FUNCTION_SCOPE_NAME##attrib, glm::mat4);\
	DEFINE_GET_ATTRIB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, get_matrix_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::mat4);\
	REGISTER_LUA_OVERLOAD_FUNCTION(get_matrix_##FUNCTION_SCOPE_NAME##attrib, get_matrix_##FUNCTION_SCOPE_NAME##attrib, get_matrix_##FUNCTION_SCOPE_NAME##attrib_ptnum);\
	/*get matrix*/\
	DEFINE_SET_ARRITB_DESCRIPTOR_FUNCTION(GEOMETRY_SCOPE_TYPE, set_matrix_##FUNCTION_SCOPE_NAME##attrib, glm::mat4);\
	DEFINE_SET_ARRITB_PTNUM_FUNCTION(GEOMETRY_SCOPE_TYPE, set_matrix_##FUNCTION_SCOPE_NAME##attrib_ptnum, glm::mat4);\
	REGISTER_LUA_OVERLOAD_FUNCTION(set_matrix_##FUNCTION_SCOPE_NAME##attrib, set_matrix_##FUNCTION_SCOPE_NAME##attrib, set_matrix_##FUNCTION_SCOPE_NAME##attrib_ptnum);




namespace Cartesian
{
    // function like houdini's api return tuple of table
    template <typename geoScope>
    class AccessScopeRange {
    public:
        static std::vector<geoScope> get(PRE_TYPE::Mesh& mesh) {
            std::vector<geoScope> listIDs;
            return listIDs;
        }
    };

    template<>
    class AccessScopeRange<PRE_TYPE::Vertex_descriptor> {
    public:
        static std::vector<PRE_TYPE::Vertex_descriptor> get(PRE_TYPE::Mesh& mesh)
        {
            std::vector<PRE_TYPE::Vertex_descriptor> listIDs;
            for (PRE_TYPE::Vertex_descriptor& id : mesh.vertices())
                listIDs.emplace_back(id);
            return listIDs;
        }
    };
    template<>
    class AccessScopeRange<PRE_TYPE::Face_descriptor> {
    public:
        static std::vector<PRE_TYPE::Face_descriptor> get(PRE_TYPE::Mesh& mesh) {
            std::vector<PRE_TYPE::Face_descriptor> listIDs;
            for (PRE_TYPE::Face_descriptor& id : mesh.faces())
                listIDs.emplace_back(id);
            return listIDs;
        }
    };

    template<>
    class AccessScopeRange<PRE_TYPE::Edge_descriptor> {
    public:
        static std::vector<PRE_TYPE::Edge_descriptor> get(PRE_TYPE::Mesh& mesh) {
            std::vector<PRE_TYPE::Edge_descriptor> listIDs;
            for (PRE_TYPE::Edge_descriptor& id : mesh.edges())
                listIDs.emplace_back(id);
            return listIDs;
        }
    };

    template<>
    class AccessScopeRange<PRE_TYPE::Halfedge_descriptor> {
    public:
        static std::vector<PRE_TYPE::Halfedge_descriptor> get(PRE_TYPE::Mesh& mesh) {
            std::vector<PRE_TYPE::Halfedge_descriptor> listIDs;
            for (PRE_TYPE::Halfedge_descriptor& id : mesh.halfedges())
                listIDs.emplace_back(id);
            return listIDs;
        }
    };



    // For General vertex/face/edge/hedge , for this element type:float/int/string
    template <typename geoScope, typename atribValueType>
    class GetAttribValues
    {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
            std::vector <atribValueType> values;
            PRE_TYPE::Mesh::Property_map<geoScope, atribValueType> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, atribValueType>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, atribValueType);
            if (!found)
                return sol::as_table(values);
            auto range = AccessScopeRange<geoScope>::get(mesh);
            for (geoScope vd : range) {
                values.push_back(attribMap[vd]);
            }


            return sol::as_table(values);
        }

    };

    // if element attribute value type is : lua table
    template <typename geoScope>
    class GetAttribValues<geoScope, sol::lua_table>
    {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
            std::vector <sol::lua_table> values;
            PRE_TYPE::Mesh::Property_map<geoScope, sol::lua_table> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, sol::lua_table>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, sol::lua_table);
            for (geoScope& vd : AccessScopeRange<geoScope>::get(mesh)) {
                values.push_back(attribMap[vd]);
            }
            return sol::as_table(values);
        }
    };


    // For General glm::vec2 type, return a tables,{ {x1,y1}, {x2,y2} ...}
    template <typename geoScope>
    class GetAttribValues<geoScope, glm::vec2> {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName, sol::this_state this_lua) {

            sol::state_view lua(this_lua);
            sol::table values = lua.create_table();
            PRE_TYPE::Mesh::Property_map<geoScope, glm::vec2> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, glm::vec2>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::vec2);

            if (!found) {
                //return sol::as_table(values);
                return values;
            }
            auto range = AccessScopeRange<geoScope>::get(mesh);
            for (geoScope& vd : range) {
                auto val = attribMap[vd];
                values.add(GLM_Vec_Helper::vec2_to_table(attribMap[vd]));
            }
            return values;
        }

    };


    // For General glm::vec3 type, return a tables,{ {x1,y1,z1}, {x2,y2,z2}.}
    template <typename geoScope>
    class GetAttribValues<geoScope, glm::vec3> {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName, sol::this_state this_lua) {

            sol::state_view lua(this_lua);
            sol::table values = lua.create_table();

            PRE_TYPE::Mesh::Property_map<geoScope, glm::vec3> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, glm::vec3>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::vec3);

            if (!found) {
                //return sol::as_table(values);
                return values;
            }

            for (geoScope& vd : AccessScopeRange<geoScope>::get(mesh)) {
                auto val = attribMap[vd];
                values.add(GLM_Vec_Helper::vec3_to_table(attribMap[vd]));
            }
            return values;

        }

    };

    // For General glm::vec4 type, return a tables,{ {x1,y1,z1,w1}, {x2,y2,z2,w2} ...}
    template <typename geoScope>
    class GetAttribValues<geoScope, glm::vec4> {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName, sol::this_state this_lua) {

            sol::state_view lua(this_lua);
            sol::table values = lua.create_table();

            PRE_TYPE::Mesh::Property_map<geoScope, glm::vec4> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, glm::vec4>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::vec4);

            if (!found) {
                //return sol::as_table(values);
                return values;
            }

            for (geoScope& vd : AccessScopeRange<geoScope>::get(mesh)) {
                auto val = attribMap[vd];
                values.add(GLM_Vec_Helper::vec4_to_table(attribMap[vd]));
            }
            return values;

        }

    };


    // For General glm::mat2 type, return a tables,{ {col1.x,col1.y,col2.x,col2.y}, {...} ...}
    template <typename geoScope>
    class GetAttribValues<geoScope, glm::mat2> {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName, sol::this_state this_lua) {

            sol::state_view lua(this_lua);
            sol::table values = lua.create_table();

            PRE_TYPE::Mesh::Property_map<geoScope, glm::mat2> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, glm::mat2>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::mat2);

            if (!found) {
                return values;
            }

            for (geoScope vd : AccessScopeRange<geoScope>::get(mesh)) {
                auto val = attribMap[vd];
                values.add(GLM_Matrix_Helper::mat2_to_table(attribMap[vd]));
            }
            return values;
        }

    };

    // For General glm::mat3 type, return a tables,{ {9 elements}, {...} ...}
    template <typename geoScope>
    class GetAttribValues<geoScope, glm::mat3> {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName, sol::this_state this_lua) {

            sol::state_view lua(this_lua);
            sol::table values = lua.create_table();
            PRE_TYPE::Mesh::Property_map<geoScope, glm::mat3> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, glm::mat3>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::mat3);

            if (!found) {
                return values;
            }

            for (geoScope vd : AccessScopeRange<geoScope>::get(mesh)) {
                auto val = attribMap[vd];
                values.add(GLM_Matrix_Helper::mat3_to_table(attribMap[vd]));
            }
            return values;
        }

    };

    // For General glm::mat4 type, return a tables,{ {16 elements}, {...} ...}
    template <typename geoScope>
    class GetAttribValues<geoScope, glm::mat4> {
    public:
        // Return lua table
        static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName, sol::this_state this_lua) {

            sol::state_view lua(this_lua);
            sol::table values = lua.create_table();
            PRE_TYPE::Mesh::Property_map<geoScope, glm::mat4> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, glm::mat4>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, glm::mat4);
            if (!found) {
                return values;
            }

            for (geoScope vd : AccessScopeRange<geoScope>::get(mesh)) {
                auto val = attribMap[vd];
                values.add(GLM_Matrix_Helper::mat4_to_table(attribMap[vd]));
            }
            return values;
        }

    };



    // --------------------------------- SetAttributesInterface--------------------------------------------------------
    // For General vertex/face/edge/hedge , for this element type:float/int/string
    template <typename geoScope, typename atribValueType>
    class SetAttribValues
    {
    public:
        // Return lua table
        static void set(PRE_TYPE::Mesh& mesh, const std::string& attribName, const sol::lua_table& values) {

            PRE_TYPE::Mesh::Property_map<geoScope, atribValueType> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, atribValueType>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, atribValueType);

            // can not find attribute
            if (!found)
                return;

            // check is lua table?
            if (!values.valid())
            {
                CARTESIAN_CORE_ERROR("Error set values for not exist attribute: {0}", attribName);
                return;
            }
            int luaIndex = 1;
            auto range = AccessScopeRange<geoScope>::get(mesh);
            //check size == range
            if (values.size() != range.size()) {
                CARTESIAN_CORE_ERROR("Error set values for attribute: {0}, length must {1}", attribName, range.size());
                return;
            }


            for (geoScope vd : range) {
                attribMap[vd] = values.get<atribValueType>(luaIndex);
                luaIndex++;
            }

        } // end of set function
    };

    template <typename geoScope>
    class SetAttribValues<geoScope, sol::lua_table>
    {
    public:
        // Return lua table
        static void set(PRE_TYPE::Mesh& mesh, const std::string& attribName, const sol::lua_table& values) {

            PRE_TYPE::Mesh::Property_map<geoScope, sol::lua_table> attribMap;
            bool found;
            boost::tie(attribMap, found) = mesh.property_map<geoScope, sol::lua_table>(attribName);
            CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, sol::lua_table);

            // can not find attribute
            if (!found)
                return;

            // check is lua table?
            if (!values.valid())
            {
                CARTESIAN_CORE_ERROR("Error set values for not exist attribute: {0}", attribName);
                return;
            }
            int luaIndex = 1;
            auto range = AccessScopeRange<geoScope>::get(mesh);
            //check size == range
            if (values.size() != range.size()) {
                CARTESIAN_CORE_ERROR("Error set values for attribute: {0}, length must {1}", attribName, range.size());
                return;
            }


            for (geoScope vd : range) {
                attribMap[vd] = values.get<sol::lua_table>(luaIndex);
                luaIndex++;
            }

        } // end of set function
    };















}// end of name space 
