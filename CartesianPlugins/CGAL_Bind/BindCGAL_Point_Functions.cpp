//
// Created by admin on 2020/8/10.
//
#define CGAL_NO_GMP 1
#include "BindCGAL_Point_Functions.h"
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include "BindCGAL_DefineType.h"

namespace Cartesian{




    void BindCGAL_Point_Functions::bind(sol::state *lua) {

        // ---------------------- Get all point attribute names --------------------------------------
        auto GetPointsAttribNames = [](const PRE_TYPE::Mesh& mesh){
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            return sol::as_table(props);
        };

        // ---------------------- Check a point attribute is exist --------------------------------------
        auto hasPointAttrib= [](const PRE_TYPE::Mesh & mesh, const std::string &name)->bool{
            std::vector<std::string> props = mesh.properties<PRE_TYPE::Vertex_descriptor>();
            for(auto &p: props){
                if(p == name) return true;
            }
            return false;
        };


    }



}