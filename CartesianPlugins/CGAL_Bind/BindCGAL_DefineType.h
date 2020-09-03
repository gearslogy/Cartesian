//
// Created by admin on 2020/8/10.
//

#ifndef BUILDINGCTSLUAJIT_BINDCGAL_DEFINETYPE_H
#define BUILDINGCTSLUAJIT_BINDCGAL_DEFINETYPE_H
#define CGAL_NO_GMP 1
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CGAL/subdivision_method_3.h>

namespace Cartesian{
     class PRE_TYPE{
     public:
        // define base mesh
        //typedef CGAL::Simple_cartesian<double> K;
        typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
        //typedef CGAL::Exact_predicates_exact_constructions_kernel K;
        typedef CGAL::Surface_mesh<K::Point_3> Mesh;

        typedef K::Vector_3 Vector;

        // define base vertex & face
        typedef Mesh::Vertex_index Vertex_descriptor;
        typedef Mesh::Face_index Face_descriptor;
        typedef Mesh::edge_index Edge_descriptor;
        typedef Mesh::halfedge_index Halfedge_descriptor;

        // iterator
        typedef Mesh::face_iterator face_iterator;
        typedef Mesh::vertex_iterator vertex_iterator;
        

    };

}









#endif //BUILDINGCTSLUAJIT_BINDCGAL_DEFINETYPE_H
