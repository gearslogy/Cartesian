//
// Created by admin on 2020/8/10.
//
#define CGAL_NO_GMP 1
#include "BindCGAL_Point_Functions.h"
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include "BindCGAL_DefineType.h"
#include "CartesianLog.h"




// check attribute is created()
#define CHECK_POINT_ATTRIB_STATUS(STATUS,ATTRIB_NAME) \
if(!STATUS){\
CARTESIAN_CORE_ERROR("Can not create attrib:{0},{1}, Maybe it's already created ", ATTRIB_NAME, __LINE__);\
}


// Register lambda function and body and register to lua
#define DEFINE_ADD_POINT_ATTRIB_FUNCTION(FUNCTION_NAME,ATTRIB_VALUE_TYPE) \
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const ATTRIB_VALUE_TYPE& default) {\
    PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE> attmap;\
    bool created;\
    boost::tie(attmap, created) = mesh.add_property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE>(name, default);\
    return created;\
}


// Register lambda function,get point attribute , point number index by int type
#define DEFINE_GET_POINT_ATTRIB_PTNUM_FUNCTION(FUNCTION_NAME, ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name,const int &ptnum ) {\
    PRE_TYPE::Vertex_descriptor vd(ptnum);\
	PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE> attribMap; \
    bool found;\
    boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE>(name);\
    if (found) {\
        return attribMap[vd];\
	}\
	CARTESIAN_CORE_ERROR("Error get attribute:{0},line:{1},funciton:{2}", name, __LINE__, __FUNCTION__);\
	return -1.2345f;\
};
// Register lambda function,get point attribute , point number index by int type
#define DEFINE_GET_POINT_ATTRIB_VERTEX_DESCRIPTOR_FUNCTION(FUNCTION_NAME, ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const PRE_TYPE::Vertex_descriptor& vd) {\
    PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE> attribMap; \
    bool found; \
    boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE>(name); \
    if (found) {\
        return attribMap[vd]; \
	}\
	CARTESIAN_CORE_ERROR("Error get attribute:{0},line:{1},funciton:{2}", name, __LINE__, __FUNCTION__); \
	return -1.2345f; \
};


// Register lambda function,set point attribute , point number by Vertex_descriptor
#define DEFINE_SET_POINT_ARRITB_PTNUM_FUNCTION(FUNCTION_NAME,SET_ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const int& vd, const SET_ATTRIB_VALUE_TYPE& value)->void {\
	PRE_TYPE::Vertex_descriptor vtex(vd);\
	PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, SET_ATTRIB_VALUE_TYPE> attribMap;\
	bool found;\
	boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, SET_ATTRIB_VALUE_TYPE>(name);\
	if (found) {\
		attribMap[vtex] = value;\
		return;\
	}\
	CARTESIAN_CORE_ERROR("Error set not existed attribute:{0}, line:{1}, function:{2}", name, __LINE__, __FUNCTION__);\
};
// Register lambda function,set point attribute , point number by Vertex_descriptor
#define DEFINE_SET_POINT_ARRITB_VERTEX_DESCRIPTOR_FUNCTION(FUNCTION_NAME,SET_ATTRIB_VALUE_TYPE)\
auto FUNCTION_NAME = [](PRE_TYPE::Mesh& mesh, const std::string& name, const PRE_TYPE::Vertex_descriptor& vd, const SET_ATTRIB_VALUE_TYPE& value)->void {\
	PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, SET_ATTRIB_VALUE_TYPE> attribMap;\
	bool found;\
	boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, SET_ATTRIB_VALUE_TYPE>(name);\
	if (found) {\
		attribMap[vd] = value;\
		return;\
	}\
	CARTESIAN_CORE_ERROR("Error set not existed attribute:{0}, line:{1}, function:{2}", name, __LINE__, __FUNCTION__);\
};

// Register to lua
#define REGISTER_POINT_LUA_FUNCTION(FUNCTION_NAME, FUNCTION) lua->set_function(FUNCTION_NAME,FUNCTION);
#define REGISTER_POINT_LUA_OVERLOAD_FUNCTION(FUNCTION_NAME, ...) lua->set_function(FUNCTION_NAME,sol::overload(__VA_ARGS__));


namespace Cartesian{




    void BindCGAL_Point_Functions::bind(sol::state *lua) {

        auto npts = [](const PRE_TYPE::Mesh& m) {return m.num_vertices(); };
        REGISTER_POINT_LUA_FUNCTION("npoints", npts);

        // ---------------------- Get all point attribute names --------------------------------------
        auto getPointsAttribNames = [](const PRE_TYPE::Mesh& mesh){
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            return sol::as_table(props);
        };
        REGISTER_POINT_LUA_FUNCTION("pointattribnames", getPointsAttribNames);


        // ---------------------- Check a point attribute is exist --------------------------------------
        auto hasPointAttrib= [](const PRE_TYPE::Mesh & mesh, const std::string &name)->bool{
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            for(auto &p: props){
                if(p == name) return true;
            }
            return false;
        };
        REGISTER_POINT_LUA_FUNCTION("haspointattrib", hasPointAttrib);

       



        // --------------------------------------------------------------  string attribute operation --------------------------------------------------------------------
		
       

        // --------------------------------------------------------------  string attribute operation --------------------------------------------------------------------







        // --------------------------------------------------------------  float attribute operation --------------------------------------------------------------------
        // add attribute
        DEFINE_ADD_POINT_ATTRIB_FUNCTION(add_float_pointattrib, float);
        REGISTER_POINT_LUA_FUNCTION("add_float_pointattrib", add_float_pointattrib);

        // get attribute
        DEFINE_GET_POINT_ATTRIB_PTNUM_FUNCTION(get_float_pointattrib_ptnum, float);
        DEFINE_GET_POINT_ATTRIB_VERTEX_DESCRIPTOR_FUNCTION(get_float_pointattrib, float);
        REGISTER_POINT_LUA_OVERLOAD_FUNCTION("get_float_pointattrib", get_float_pointattrib,get_float_pointattrib_ptnum);

        // float set
        DEFINE_SET_POINT_ARRITB_PTNUM_FUNCTION(set_float_pointattrib_ptnum, float);
        DEFINE_SET_POINT_ARRITB_VERTEX_DESCRIPTOR_FUNCTION(set_float_pointattrib, float);
        REGISTER_POINT_LUA_OVERLOAD_FUNCTION("set_float_pointattrib", set_float_pointattrib, set_float_pointattrib_ptnum);
        // --------------------------------------------------------------  float attribute operation --------------------------------------------------------------------
    }



}