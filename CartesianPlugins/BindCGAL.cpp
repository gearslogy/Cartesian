//
// Created by admin on 2020/8/4.
//

#include "BindCGAL.h"


#define CGAL_NO_GMP 1

#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
typedef Mesh::Vertex_index vertex_descriptor;
typedef Mesh::Face_index face_descriptor;




void Cartesian::BindCGAL::bind(sol::state *lua) {





}
