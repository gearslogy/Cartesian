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
#define DEFINE_ADD_ATTRIB_FUNCTION(SCOPE_TYPE,FUNCTION_NAME,ATTRIB_VALUE_TYPE) \
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const ATTRIB_VALUE_TYPE& default) {\
    PRE_TYPE::Mesh::Property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE> attmap;\
    bool created;\
    boost::tie(attmap, created) = mesh.add_property_map<SCOPE_TYPE, ATTRIB_VALUE_TYPE>(name, default);\
    CHECK_ATTRIB_CREATED_STATUS(created,name,ATTRIB_VALUE_TYPE);\
    return created;\
}


// Register lambda function,get point attribute , point number index by int type
#define DEFINE_GET_ATTRIB_ID_FUNCTION(SCOPE_TYPE,FUNCTION_NAME, ATTRIB_VALUE_TYPE)\
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
// Register lambda function,get point attribute , point number index by int type
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


// Register lambda function,set point attribute , point number by Vertex_descriptor
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
// Register lambda function,set point attribute , point number by Vertex_descriptor
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
#define REGISTER_LUA_FUNCTION(FUNCTION_NAME, FUNCTION) lua->set_function(FUNCTION_NAME,FUNCTION);
#define REGISTER_LUA_OVERLOAD_FUNCTION(FUNCTION_NAME, ...) lua->set_function(FUNCTION_NAME,sol::overload(__VA_ARGS__));



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


	
	// For General vertex lua table type, let you write : get_table_values(m,"foo"), return table
	template <typename scope, typename tableValueType>
	class GetTableAttribValues
	{
	public:
		// Return lua table
		static auto get(PRE_TYPE::Mesh& mesh, const std::string& attribName) {
			std::vector<tableValueType> values; 
		
			PRE_TYPE::Mesh::Property_map<scope, sol::lua_table> attribMap;
			bool found;
			boost::tie(attribMap, found) = mesh.property_map<scope, sol::lua_table>(attribName);
			CHECK_ATTRIB_FOUNDED_STATUS(found, attribName, tableValueType);

			if (found) {
				for (scope vd : mesh.vertices()) {
					auto per_vertex_table = attribMap[vd];
					for (auto i = 0; i < per_vertex_table.size(); i++) {
						values.push_back(per_vertex_table.get<tableValueType>(i+1));
					}
				}
			}
			return sol::as_table(values);
		}
	};
	

}// end of name space 
