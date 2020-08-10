//
// Created by admin on 2020/8/10.
//

#ifndef BUILDINGCTSLUAJIT_CGAL_POINTATTRIB_HELPER_H
#define BUILDINGCTSLUAJIT_CGAL_POINTATTRIB_HELPER_H
#define CGAL_NO_GMP 1


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Cartesian
{

    template <typename T1,typename T2>
    bool addpointattrib(T1 & mesh, const std::string &name, const T2& defvalue);


}

#endif //BUILDINGCTSLUAJIT_CGAL_POINTATTRIB_HELPER_H
