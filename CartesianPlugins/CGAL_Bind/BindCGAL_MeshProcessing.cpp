#include "BindCGAL_MeshProcessing.h"
#include "BindCGAL_DefineType.h"
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>
#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CartesianLog.h"

namespace Cartesian {

    void BindCGAL_MeshProcessing::bind(sol::state* lua) {

        // triangulate face
        auto triangulate_faces = [](PRE_TYPE::Mesh &mesh) {
            CGAL::Polygon_mesh_processing::triangulate_faces(mesh);
        };
        lua->set_function("triangulate",triangulate_faces);
        
        // computer normal save to CGAL_N
        auto compute_normal = [](PRE_TYPE::Mesh& mesh) {
            // 
           
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, PRE_TYPE::Vector > CGAL_N_CREATE_MAP;
            bool created;
            boost::tie(CGAL_N_CREATE_MAP, created) = mesh.add_property_map < PRE_TYPE::Vertex_descriptor, PRE_TYPE::Vector >("CGAL_N", CGAL::NULL_VECTOR);
            CGAL::Polygon_mesh_processing::compute_vertex_normals(mesh, CGAL_N_CREATE_MAP);

            // to CGAL_N glm::vec3
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, glm::vec3 > CGAL_N_GLM_CREATE_MAP;
            bool found = false;
            boost::tie(CGAL_N_GLM_CREATE_MAP, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, glm::vec3>("CGAL_N");
            if (found) {
                for (auto& vd : mesh.vertices())
                    CGAL_N_GLM_CREATE_MAP[vd] = glm::vec3(CGAL_N_CREATE_MAP[vd].x(), CGAL_N_CREATE_MAP[vd].y(), CGAL_N_CREATE_MAP[vd].z());
            }
            else {
                bool created;
                boost::tie(CGAL_N_GLM_CREATE_MAP, created) = mesh.add_property_map < PRE_TYPE::Vertex_descriptor, glm::vec3 >("CGAL_N", glm::vec3(0) );
                for (auto& vd : mesh.vertices())
                    CGAL_N_GLM_CREATE_MAP[vd] = glm::vec3(CGAL_N_CREATE_MAP[vd].x(), CGAL_N_CREATE_MAP[vd].y(), CGAL_N_CREATE_MAP[vd].z());
            }


            // remove CGAL Vector CGAL_N type
            mesh.remove_property_map(CGAL_N_CREATE_MAP);

        };
        lua->set_function("compute_normal",compute_normal);

        // normal reverse
        auto reverse_normal = [](PRE_TYPE::Mesh& mesh) {
            // to CGAL_N glm::vec3
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, glm::vec3 > CGAL_N_GLM_CREATE_MAP;
            bool found = false;
            boost::tie(CGAL_N_GLM_CREATE_MAP, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, glm::vec3>("CGAL_N");
            if (found) {
                for (auto& vd : mesh.vertices())
                    CGAL_N_GLM_CREATE_MAP[vd] = -CGAL_N_GLM_CREATE_MAP[vd];
            }
            else {
                CARTESIAN_CORE_ERROR("can not reverse vertex normal, not exist CGAL_N");
            }
        };
        lua->set_function("reverse_normal", reverse_normal);


        auto bounding_min = [](const PRE_TYPE::Mesh &mesh) {
            auto bbox = CGAL::Polygon_mesh_processing::bbox(mesh);
            auto xmin = bbox.xmin();
            auto ymin = bbox.ymin();
            auto zmin = bbox.zmin();
            return glm::vec3(xmin, ymin, zmin);
        };
        lua->set_function("getbbox_min", bounding_min);
        auto bounding_max = [](const PRE_TYPE::Mesh &mesh) {
            auto bbox = CGAL::Polygon_mesh_processing::bbox(mesh);
            auto xmax = bbox.xmax();
            auto ymax = bbox.ymax();
            auto zmax = bbox.zmax();
            return glm::vec3(xmax, ymax, zmax);
        };
        lua->set_function("getbbox_max", bounding_max);

        // https://doc.cgal.org/latest/Subdivision_method_3/index.html#sectionSubIntro
        auto subdmesh = [](PRE_TYPE::Mesh& mesh, int iterations, int type) {
            if (type == 0)
            {
                auto pointmap = get(CGAL::vertex_point, mesh);
  
                CGAL::Subdivision_method_3::CatmullClark_subdivision(mesh, CGAL::parameters::vertex_point_map(pointmap));
                CGAL::Subdivision_method_3::CatmullClark_subdivision(mesh, CGAL::parameters::number_of_iterations(iterations) );
                
            }
            else if(type == 1)
                CGAL::Subdivision_method_3::Loop_subdivision(mesh, CGAL::parameters::number_of_iterations(iterations));
            else {
                CARTESIAN_CORE_ERROR("error subd type, type==1:CatmullClark,type==2:Loop_subdivision");
            }
        };
        lua->set_function("subdmesh", subdmesh);

    }
}
