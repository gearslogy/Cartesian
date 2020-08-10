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


#define PREPARE_ADD_POINT_ATTRIB(MESH_REF,ATTRIB_NAME,ATTRIB_VALUE_TYPE,DEFAULT_VALUE) PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE> attribMap;\
bool created;\
boost::tie(attribMap, created) = MESH_REF.add_property_map<PRE_TYPE::Vertex_descriptor,ATTRIB_VALUE_TYPE>(ATTRIB_NAME, DEFAULT_VALUE);

#define PREPARE_GET_POINT_ATTRIB(MESH_REF,ATTRIB_NAME,ATTRIB_VALUE_TYPE) PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE> attribMap;\
bool found;\
boost::tie(attribMap, found)=  MESH_REF.property_map<PRE_TYPE::Vertex_descriptor, ATTRIB_VALUE_TYPE>(ATTRIB_NAME);


namespace Cartesian{




    void BindCGAL_Point_Functions::bind(sol::state *lua) {

        auto npts = [](const PRE_TYPE::Mesh& m) {return m.num_vertices(); };
        lua->set_function("npoints", npts);


        // ---------------------- Get all point attribute names --------------------------------------
        auto getPointsAttribNames = [](const PRE_TYPE::Mesh& mesh){
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            return sol::as_table(props);
        };
        lua->set_function("pointattribnames", getPointsAttribNames);


        // ---------------------- Check a point attribute is exist --------------------------------------
        auto hasPointAttrib= [](const PRE_TYPE::Mesh & mesh, const std::string &name)->bool{
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            for(auto &p: props){
                if(p == name) return true;
            }
            return false;
        };

        lua->set_function("haspointattrib", hasPointAttrib);

       


        // ---------------------- get/add/set attrib ------------------------------------------------

        //  ------- float add -----------
		auto add_float_pointattrib = [](PRE_TYPE::Mesh& mesh, const std::string& name, const float& default) ->bool{
            PREPARE_ADD_POINT_ATTRIB(mesh, name, float, 0.0f);
			return created;
		};
        lua->set_function("add_float_pointattrib", add_float_pointattrib);



        //  ------- float get -----------
        auto get_float_pointattrib = [](PRE_TYPE::Mesh& mesh, const std::string& name, const PRE_TYPE::Vertex_descriptor &vd)->float {
            PREPARE_GET_POINT_ATTRIB(mesh, name,float);
            if (found) {
                return attribMap[vd];
            }
			CARTESIAN_CORE_ERROR("Error get attribute:{0},{1},{2}", name, __LINE__, __FUNCTION__);
            return -1.2345f;
        };

		auto get_float_pointattrib_ptnum = [](PRE_TYPE::Mesh& mesh, const std::string& name, const int& ptnum)->float {
            PRE_TYPE::Vertex_descriptor vd(ptnum);
			PREPARE_GET_POINT_ATTRIB(mesh, name, float);
			if (found) {
				return attribMap[vd];
			}
			CARTESIAN_CORE_ERROR("Error get attribute:{0},{1},{2}", name, __LINE__, __FUNCTION__);
			return -1.2345f;
		};
        lua->set_function("get_float_pointattrib", sol::overload(get_float_pointattrib,get_float_pointattrib_ptnum));

        //  ------- float set -----------
        auto set_float_pointattrib = [](PRE_TYPE::Mesh& mesh, const std::string& name, const PRE_TYPE::Vertex_descriptor &vd, const float& value) ->void{
            PREPARE_GET_POINT_ATTRIB(mesh, name, float);
            if (found) {
                attribMap[vd] = value;
                return;
            }
			CARTESIAN_CORE_ERROR("Error set attribute:{0},{1},{2}", name, __LINE__, __FUNCTION__);
        };
        // overload set funciton
		auto set_float_pointattrib_ptnum = [](PRE_TYPE::Mesh& mesh, const std::string& name, const int& ptnum, const float& value) ->void {
			PRE_TYPE::Vertex_descriptor vd(ptnum);
            PREPARE_GET_POINT_ATTRIB(mesh, name, float);
			boost::tie(attribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, float>(name);
			if (found) {
				attribMap[vd] = value;
                return;
			}
			CARTESIAN_CORE_ERROR("Error set attribute:{0},{1},{2}", name, __LINE__, __FUNCTION__);
		};
        lua->set_function("set_float_pointattrib", sol::overload(set_float_pointattrib, set_float_pointattrib_ptnum));

    }



}