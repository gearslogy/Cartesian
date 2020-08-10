//
// Created by admin on 2020/8/4.
//

#include "BindCGAL.h"

// ----------------- Data exchange with glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../GLM_Bind/GLM_Matrix_Helper.h"
#include "../GLM_Bind/GLM_Vec_Helper.h"
#include "CartesianLog.h"
#include "BindCGAL_DefineType.h"

#define CGAL_NO_GMP 1

#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>



// ------------ add triangle by houdini------------------------
/*
int prim = addprim(geoself(),"poly");
addpoint(geoself(), set(0,1,0) );
addpoint(geoself(), set(1,1,0) );
addpoint(geoself(), set(0,0,0) );
addvertex(geoself(), prim, 0 );
addvertex(geoself(), prim, 1 );
addvertex(geoself(), prim, 2 );
 */



namespace Cartesian{

void BindCGAL::bind(sol::state *lua) {
    // bind vertex index
    sol::usertype<PRE_TYPE::Vertex_descriptor> vertex= (*lua).new_usertype<PRE_TYPE::Vertex_descriptor>(
            "vertex",
            sol::constructors<PRE_TYPE::Vertex_descriptor ,PRE_TYPE::Vertex_descriptor(int)>()
            );
    vertex["idx"] = &PRE_TYPE::Vertex_descriptor::idx;


    // bind face index
    sol::usertype<PRE_TYPE::Face_descriptor> face= (*lua).new_usertype<PRE_TYPE::Face_descriptor>(
            "face",
            sol::constructors<PRE_TYPE::Face_descriptor(),PRE_TYPE::Face_descriptor(int)>()
    );
    face["idx"] = &PRE_TYPE::Face_descriptor::idx;


    // bind half-edge index
    sol::usertype<CGAL_halfedge_descriptor> half_edge= (*lua).new_usertype<CGAL_halfedge_descriptor>(
            "half_edge",
            sol::constructors<CGAL_halfedge_descriptor(),CGAL_halfedge_descriptor(int)>()
    );
    half_edge["idx"] = &CGAL_halfedge_descriptor::idx;


    // bind edge index
    sol::usertype<CGAL_edge_descriptor> edge= (*lua).new_usertype<CGAL_edge_descriptor>(
            "edge",
            sol::constructors<CGAL_edge_descriptor(),CGAL_edge_descriptor(int),CGAL_edge_descriptor(CGAL_halfedge_descriptor)>()
    );
    edge["idx"] = &CGAL_edge_descriptor::idx;


    // bind mesh pointer
    sol::usertype<CGAL_Mesh> mesh= (*lua).new_usertype<CGAL_Mesh>(
            "mesh"
            );


    // todo addprim() need imp in DCC software,and return mesh handle.

    // mesh functions
    // addpoint(mesh, pt) : return vertex descriptor
    auto add_point = [](CGAL_Mesh & mesh, const glm::vec3 &ptpos){
        auto vd = mesh.add_vertex(CGAL_K::Point_3(ptpos.x, ptpos.y, ptpos.z));
        return vd;
    };
    auto add_point_table = [](CGAL_Mesh & mesh, const sol::lua_table & table){
        if(table.size() != 3){
            CARTESIAN_CORE_ERROR("addpoint(table) table length = 3");
            return CGAL_vertex_descriptor(-1);
        }
        auto ptpos = GLM_Vec_Helper::table_to_vec3(table);
        auto vd = mesh.add_vertex(CGAL_K::Point_3(ptpos.x, ptpos.y, ptpos.z));
        return vd;
    };
    lua->set_function("addpoint",sol::overload(add_point,add_point_table));





}

}
