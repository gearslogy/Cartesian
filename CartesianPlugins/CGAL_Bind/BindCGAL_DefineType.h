//
// Created by admin on 2020/8/10.
//

#ifndef BUILDINGCTSLUAJIT_BINDCGAL_DEFINETYPE_H
#define BUILDINGCTSLUAJIT_BINDCGAL_DEFINETYPE_H
#define CGAL_NO_GMP 1
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

namespace Cartesian{
     class PRE_TYPE{
     public:
        // define base mesh
        typedef CGAL::Simple_cartesian<double> K;
        typedef CGAL::Surface_mesh<K::Point_3> Mesh;

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
