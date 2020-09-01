//
// Created by admin on 2020/8/4.
//
#define CGAL_NO_GMP 1

#include "BindCGAL.h"

// ----------------- Data exchange with glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../GLM_Bind/GLM_Matrix_Helper.h"
#include "../GLM_Bind/GLM_Vec_Helper.h"
#include "CartesianLog.h"
#include "BindCGAL_DefineType.h"


#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <fstream>


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



namespace Cartesian {

    static std::string VertexToString(const PRE_TYPE::Vertex_descriptor &vert) {
        std::string ret;
        ret += "PointIDType:";
        ret += std::to_string(vert.idx());
        ret += " ,convert to int use PointIDType:idx()\n";
        return ret;
    }

    static std::string FaceToString(const PRE_TYPE::Face_descriptor& face) {
        std::string ret;
        ret += "FaceIDType:";
        ret += std::to_string(face.idx());
        ret += " ,convert to int use FaceIDType:idx()\n";
        return ret;
    }


    static std::string MeshToString(const PRE_TYPE::Mesh& mesh) {
        std::string ret = "-----------------\n";
        ret += "NumPoints:";
        ret += std::to_string(mesh.number_of_vertices());
        ret += "\n";

        ret += "NumFaces:";
        ret += std::to_string(mesh.number_of_faces());
        ret += "\n";

        ret += "NumEdges:";
        ret += std::to_string(mesh.number_of_edges());
        ret += "\n";

        ret += "NumHalfEdges:";
        ret += std::to_string(mesh.number_of_halfedges());
        ret += "\n";

        ret += "point attribute:";
        ret += "\n{\n";
        for (auto& attriName : mesh.properties<PRE_TYPE::Vertex_descriptor>()) {
            ret += "\t";
            ret += attriName;
            ret += "\n";
        }
        ret[ret.size() - 1] = ' '; 
        ret += "\n}\n";


        ret += "prim attribute:";
        ret += "\n{\n";
        for (auto& attriName : mesh.properties<PRE_TYPE::Face_descriptor>()) {
            ret += "\t";
            ret += attriName;
            ret += "\n";
        }
        ret[ret.size() - 1] = ' ';
        ret += "\n}\n";
        ret += "-----------------\n";
        return ret;
    }

    static PRE_TYPE::Mesh MeshPlusMesh(const PRE_TYPE::Mesh& mesh1, const PRE_TYPE::Mesh &mesh2) {
        PRE_TYPE::Mesh mesh;
        mesh.join(mesh1);
        mesh.join(mesh2);
        return mesh;
    }


    void BindCGAL::bind(sol::state* lua) {
        // bind vertex index
        sol::usertype<PRE_TYPE::Vertex_descriptor> vertex = (*lua).new_usertype<PRE_TYPE::Vertex_descriptor>(
            "vertex",
            sol::constructors<PRE_TYPE::Vertex_descriptor, PRE_TYPE::Vertex_descriptor(int)>(),
            sol::meta_function::to_string, VertexToString
            );
        vertex["idx"] = &PRE_TYPE::Vertex_descriptor::idx;


        // bind face index
        sol::usertype<PRE_TYPE::Face_descriptor> face = (*lua).new_usertype<PRE_TYPE::Face_descriptor>(
            "face",
            sol::constructors<PRE_TYPE::Face_descriptor(), PRE_TYPE::Face_descriptor(int)>(),
            sol::meta_function::to_string, FaceToString
            );
        face["idx"] = &PRE_TYPE::Face_descriptor::idx;


        // bind half-edge index
        sol::usertype<PRE_TYPE::Halfedge_descriptor> half_edge = (*lua).new_usertype<PRE_TYPE::Halfedge_descriptor>(
            "hedge",
            sol::constructors<PRE_TYPE::Halfedge_descriptor(), PRE_TYPE::Halfedge_descriptor(int)>()
            );
        half_edge["idx"] = &PRE_TYPE::Halfedge_descriptor::idx;


        // bind edge index
        sol::usertype<PRE_TYPE::Edge_descriptor> edge = (*lua).new_usertype<PRE_TYPE::Edge_descriptor>(
            "edge",
            sol::constructors<PRE_TYPE::Edge_descriptor(), PRE_TYPE::Edge_descriptor(int), PRE_TYPE::Edge_descriptor(PRE_TYPE::Halfedge_descriptor)>()

            );
        edge["idx"] = &PRE_TYPE::Edge_descriptor::idx;


        // bind mesh pointer
        sol::usertype<PRE_TYPE::Mesh> mesh = (*lua).new_usertype<PRE_TYPE::Mesh>(
            "mesh",
            sol::constructors<PRE_TYPE::Mesh()>(),
            sol::meta_function::to_string, MeshToString,
            sol::meta_function::addition, MeshPlusMesh
            );
        mesh["clear"] = &PRE_TYPE::Mesh::clear;
        mesh["has_garbage"] = &PRE_TYPE::Mesh::has_garbage;
        mesh["npoints"] = &PRE_TYPE::Mesh::number_of_vertices;
        mesh["nprims"] = &PRE_TYPE::Mesh::number_of_faces;
        mesh["nedges"] = &PRE_TYPE::Mesh::number_of_edges;
        mesh["nhedges"] = &PRE_TYPE::Mesh::number_of_halfedges;
        // todo addprim() need imp in DCC software,and return mesh handle.

        // mesh functions
        // addpoint(mesh, pt) : return vertex descriptor
        auto add_point = [](PRE_TYPE::Mesh& mesh, const glm::vec3& ptpos) {
            auto vd = mesh.add_vertex(PRE_TYPE::K::Point_3(ptpos.x, ptpos.y, ptpos.z));
            return vd;
        };
        auto add_point_table = [](PRE_TYPE::Mesh& mesh, const sol::lua_table& table) {
            if (table.size() != 3) {
                CARTESIAN_CORE_ERROR("addpoint(table) table length = 3");
                return PRE_TYPE::Vertex_descriptor(-1);
            }
            auto ptpos = GLM_Vec_Helper::table_to_vec3(table);
            auto vd = mesh.add_vertex(PRE_TYPE::K::Point_3(ptpos.x, ptpos.y, ptpos.z));
            return vd;
        };
        lua->set_function("addpoint", sol::overload(add_point, add_point_table));



        // ------------------ add face function , return face descriptor -------------------------------
        auto add_face_triangle = [](PRE_TYPE::Mesh& mesh, const PRE_TYPE::Vertex_descriptor& u, const PRE_TYPE::Vertex_descriptor& v, const PRE_TYPE::Vertex_descriptor& w) {
            return mesh.add_face(u, v, w);
        };
        auto add_face_quad = [](PRE_TYPE::Mesh& mesh, const PRE_TYPE::Vertex_descriptor& u, const PRE_TYPE::Vertex_descriptor& v, const PRE_TYPE::Vertex_descriptor& w, const PRE_TYPE::Vertex_descriptor& x) {
            return mesh.add_face(u, v, w, x);
        };
        lua->set_function("addface", sol::overload(add_face_triangle, add_face_quad));



        // remove point
        auto remove_point = []( PRE_TYPE::Mesh& mesh, const PRE_TYPE::Vertex_descriptor& vd) {
            mesh.remove_vertex(vd);
            /*
            auto halfedge = mesh.halfedge(vd);
            auto face = mesh.face(halfedge);
            std::cout <<"remove vert:"<< vd <<"\tremove face:" << face << std::endl;
            mesh.remove_face(face);*/
        };

        auto remove_point_ptnum = []( PRE_TYPE::Mesh& mesh, const int & ptnum) {
            PRE_TYPE::Vertex_descriptor vd(ptnum);
            //mesh.remove_vertex(vd);
            CGAL::remove_vertex(vd,mesh);
            
            /*
            auto halfedge = mesh.halfedge(vd);
            if (halfedge.is_valid()) {
                auto face = mesh.face(halfedge);
                if (face.is_valid()) {
                    std::cout << "remove vert:" << vd << "\tremove face:" << face << std::endl;
                    mesh.remove_face(face);
                }
            }*/
            
        };
        lua->set_function("removepoint", sol::overload(remove_point, remove_point_ptnum));


        // remove prim
        auto remove_prim = []( PRE_TYPE::Mesh& mesh, const PRE_TYPE::Face_descriptor& vd) {
            mesh.remove_face(vd);
        };

        auto remove_prim_ptnum = []( PRE_TYPE::Mesh& mesh, const int& ptnum) {
            mesh.remove_face(PRE_TYPE::Face_descriptor(ptnum));
        };
        lua->set_function("removeprim", sol::overload(remove_prim, remove_prim_ptnum));

       

        // -------------------- after remove prim, may be need remove unused points -------------------------------
        auto remove_unused_points = [](PRE_TYPE::Mesh& mesh) {
            for (auto iter = mesh.vertices_begin(); iter != mesh.vertices_end(); iter++)
            {
                PRE_TYPE::Vertex_descriptor vertid = *iter;
                CGAL::Vertex_around_target_circulator<PRE_TYPE::Mesh> begin(mesh.halfedge(vertid), mesh), end(begin);
                auto accum_neighbours = 0;
                while (++begin != end) {
                    accum_neighbours += 1; // add once
                }

                if (accum_neighbours == 1) {
                    mesh.remove_vertex(vertid);
                }
            }
        };
        lua->set_function("remove_unused_points", remove_unused_points);

        auto remove_unused_prims = [](PRE_TYPE::Mesh& mesh) {
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    if (!v.is_valid() || mesh.is_removed(v))
                    {
                        mesh.remove_face(f);
                        break;
                    }
                }
            }
        };
        lua->set_function("remove_unused_prims", remove_unused_points);



        // --------------- after change the geometry's topo, cgal must update geometry -----------------------
        auto update_geometry = [](PRE_TYPE::Mesh& mesh) {
            mesh.collect_garbage();
        };
        lua->set_function("topoupdate", update_geometry);

        // -------------------- neibours , return lua table------------------------------
        auto neibours = [](const PRE_TYPE::Mesh& mesh, const PRE_TYPE::Vertex_descriptor& vd) {
            std::vector <PRE_TYPE::Vertex_descriptor> vts;
            CGAL::Vertex_around_target_circulator<PRE_TYPE::Mesh> begin(mesh.halfedge(vd), mesh), end(begin);
            while (1) {
                PRE_TYPE::Vertex_descriptor vd = *begin;
                if (vd.is_valid())
                    //std::cout << " " << vd.idx();
                    vts.emplace_back(vd);
                if (++begin == end)break;
            }
            return sol::as_table(vts);
        };
        auto neibours_ptnum = [](const PRE_TYPE::Mesh& mesh, const int& ptnum) {
            std::vector <int> vts;
            CGAL::Vertex_around_target_circulator<PRE_TYPE::Mesh> begin(mesh.halfedge(PRE_TYPE::Vertex_descriptor(ptnum)), mesh), end(begin);
            while (1) {
                PRE_TYPE::Vertex_descriptor vd = *begin;
                if (vd.is_valid())
                    //std::cout << " " << vd.idx();
                    vts.emplace_back(vd.idx());
                if (++begin == end)break;
            }
            return sol::as_table(vts);
        };
        lua->set_function("neibours", sol::overload(neibours, neibours_ptnum));



        // -------------------------- pointprims, return lua table -----------------------------------------------
        auto pointprims = [](const PRE_TYPE::Mesh& mesh, const PRE_TYPE::Vertex_descriptor& vd) {
            CGAL::Face_around_target_circulator<PRE_TYPE::Mesh> begin(mesh.halfedge(vd), mesh), end(begin);
            std::vector <PRE_TYPE::Face_descriptor> vts;
            while (1) {
                PRE_TYPE::Face_descriptor fd = *begin;
                if (fd.is_valid())
                    //std::cout << " " << fd.idx();
                    vts.emplace_back(fd);
                if (++begin == end)break;
            }
            return sol::as_table(vts);
        };
        auto pointprims_ptnum = [](const PRE_TYPE::Mesh& mesh, const int& ptnum) {
            CGAL::Face_around_target_circulator<PRE_TYPE::Mesh> begin(mesh.halfedge(PRE_TYPE::Vertex_descriptor(ptnum)), mesh), end(begin);
            std::vector <int> vts;
            while (1) {
                PRE_TYPE::Face_descriptor fd = *begin;
                if (fd.is_valid())
                    //std::cout << " " << fd.idx();
                    vts.emplace_back(fd.idx() );
                if (++begin == end)break;
            }
            return sol::as_table(vts);
        };
        lua->set_function("pointprims", sol::overload(pointprims, pointprims_ptnum));


        // ------------------------------------- primpoints , return lua table-------------------------------------------
        auto primpoints = [](const PRE_TYPE::Mesh& mesh, const PRE_TYPE::Face_descriptor& face) {
            std::vector <PRE_TYPE::Vertex_descriptor> vts;
            CGAL::Vertex_around_face_circulator<PRE_TYPE::Mesh> vRoundBegin(mesh.halfedge(face), mesh), vRoundEnd(vRoundBegin);
            while (1) {
                PRE_TYPE::Vertex_descriptor vd = *vRoundBegin;
                vts.emplace_back(vd);
                if (++vRoundBegin == vRoundEnd)break;
            }
            return sol::as_table(vts);
        };
        auto primpoints_primnum = [](const PRE_TYPE::Mesh& mesh, const int &primnum) {
            std::vector <int> vts;
            CGAL::Vertex_around_face_circulator<PRE_TYPE::Mesh> vRoundBegin(mesh.halfedge(PRE_TYPE::Face_descriptor(primnum) ), mesh), vRoundEnd(vRoundBegin);
            while (1) {
                PRE_TYPE::Vertex_descriptor vd = *vRoundBegin;
                vts.emplace_back(vd.idx());
                if (++vRoundBegin == vRoundEnd)break;
            }
            return sol::as_table(vts);
        };
        lua->set_function("primpoints", sol::overload(primpoints, primpoints_primnum));



        // mesh io read
        auto read_mesh = [](PRE_TYPE::Mesh& mesh, const std::string& path) {
            std::ifstream in(path);
            in >> mesh;
        };
        lua->set_function("readmesh", read_mesh);

        // mesh io save
        auto save_mesh = [](PRE_TYPE::Mesh& mesh, const std::string& path) {
            std::ofstream out(path);
            out << mesh;
        };
        lua->set_function("savemesh", save_mesh);


    }

}
