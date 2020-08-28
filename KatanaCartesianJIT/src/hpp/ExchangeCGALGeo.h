#pragma once


#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__

//#define SOL_NO_EXCEPTIONS 1
#include <FnGeolib/op/FnGeolibOp.h>
#include "BindCGAL_DefineType.h"
#include <vector>
#include "CartesianLog.h"

#define MAKE_ATTRIB_MAP(ATTRIB_NAME,ATTRIB_SCOPE,ATTRIB_TYPE,ATTRIB_HANDLE,ATTRIB_TUPLESIZE)\
AttribMap ATTRIB_MAP;\
ATTRIB_MAP.name = ATTRIB_NAME;\
ATTRIB_MAP.scope = ATTRIB_SCOPE;\
ATTRIB_MAP.type = ATTRIB_TYPE;\
ATTRIB_MAP.handle = ATTRIB_HANDLE;\
ATTRIB_MAP.tupleSize = ATTRIB_TUPLESIZE;


/**
         * @brief add attribute for cgal mesh
         * @param MESH cgal mesh
         * @param ATTRIB_NAME the attribute name
         * @param GEO_SCOPE cgal vertex or face or hedge or edge
         * @param ATTRIB_VALUE_TYPE type of att
         * @param DEFAULT_ATTRIB_VALUE default value
         * @return
*/
#define CREATE_CGAL_ATTRIB(MESH, ATTRIB_NAME, GEO_SCOPE, ATTRIB_VALUE_TYPE, DEFAULT_ATTRIB_VALUE)\
PRE_TYPE::Mesh::Property_map<GEO_SCOPE, ATTRIB_VALUE_TYPE > CGAL_CREATED_ATTRIB_MAP;\
bool created;\
boost::tie(CGAL_CREATED_ATTRIB_MAP, created) = MESH.add_property_map<GEO_SCOPE, ATTRIB_VALUE_TYPE>(ATTRIB_NAME, DEFAULT_ATTRIB_VALUE);




// find attribute 
#define FIND_CGAL_ATTRIBUTE(MESH, ATTRIB_NAME, GEO_SCOPE, ATTRIB_VALUE_TYPE)\
PRE_TYPE::Mesh::Property_map<GEO_SCOPE,ATTRIB_VALUE_TYPE> CGAL_FOUND_ATTRIB_MAP;\
bool found;\
boost::tie(CGAL_FOUND_ATTRIB_MAP, found) = MESH.property_map<GEO_SCOPE, ATTRIB_VALUE_TYPE>(ATTRIB_NAME);



namespace Cartesian {

    struct AttribMap {
        std::string name;                       // attrib name
        std::string scope;                      // face or vertex?
        unsigned int type;                      // int float string double?
        FnAttribute::Attribute handle;          // handle for attribute values

        // but i find tupleSize useless. just check attribute's value.size()/3 == npts that's vector!
        // recording it only for respecting.
        unsigned int tupleSize;                 // 1-> float, 3 -> vector, 4 -> vector4
    };




    void BuildSurfaceMeshFromKatana(PRE_TYPE::Mesh& meshToBuild, Foundry::Katana::GeolibCookInterface& iface);



    template <typename GEO_SCOPE>
    class GetCGALScopeNum {
    public:
        static int getNum(const PRE_TYPE::Mesh& mesh) {
            return 0;
        }
    };

    template <>
    class GetCGALScopeNum<PRE_TYPE::Vertex_descriptor> {
    public:
        static int getNum(const PRE_TYPE::Mesh& mesh) {
            return mesh.num_vertices();
        }
    };

    template <>
    class GetCGALScopeNum<PRE_TYPE::Face_descriptor> {
    public:
        static int getNum(const PRE_TYPE::Mesh& mesh) {
            return mesh.num_faces();
        }
    };

    template <>
    class GetCGALScopeNum<PRE_TYPE::Edge_descriptor> {
    public:
        static int getNum(const PRE_TYPE::Mesh& mesh) {
            return mesh.num_edges();
        }
    };

    template <>
    class GetCGALScopeNum<PRE_TYPE::Halfedge_descriptor> {
    public:
        static int getNum(const PRE_TYPE::Mesh& mesh) {
            return mesh.num_halfedges();
        }
    };

    template <typename GEO_SCOPE >
    class AttributeGetSet {

    public:
        // for int attribute
        static void createAndSet_INT_CGALAttrib(PRE_TYPE::Mesh& mesh, const AttribMap& attribmap, const float& time) {
            auto meshnpts = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);
            FnAttribute::IntAttribute refValueAttrib = static_cast<FnAttribute::IntAttribute> (attribmap.handle);
            auto data = refValueAttrib.getNearestSample(time);
            if (data.size() / 3 == meshnpts)                    // first we check the data length is equal 3 time than our cgal mesh points. if true, that's a vector attribute 
            {
                CARTESIAN_CORE_INFO("create int attribute , create as vector, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec3, glm::vec3(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);

                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec3);
                if (found) {
                    for (auto i = 0; i < data.size() / 3; i++) {
                        auto x = data[i * 3 + 0];
                        auto y = data[i * 3 + 1];
                        auto z = data[i * 3 + 2];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec3(x, y, z);
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }

            }
            if (data.size() == meshnpts) // not vector, not vector2,not matrix
            {

                CARTESIAN_CORE_INFO("create int attribute , create as int, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, int, 0);
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);


                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, int);
                if (found) {
                    for (auto i = 0; i < data.size(); i++) {
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = data[i];
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }
        }

        static void createAndSet_FLT_CGALAttrib(PRE_TYPE::Mesh& mesh, const AttribMap& attribmap, const float& time) {
            auto meshnpts = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);
            FnAttribute::FloatAttribute refValueAttrib = static_cast<FnAttribute::FloatAttribute> (attribmap.handle);
            auto data = refValueAttrib.getNearestSample(time);                    // first we check the data length is equal 3 time than our cgal mesh points. if true, that's a vector attribute 

            if (data.size() / 3 == meshnpts) {
                CARTESIAN_CORE_INFO("create float attribute , create as vector, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec3, glm::vec3(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec3);
                if (found) {
                    for (auto i = 0; i < data.size() / 3; i++) {
                        auto x = data[i * 3 + 0];
                        auto y = data[i * 3 + 1];
                        auto z = data[i * 3 + 2];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec3(x, y, z);
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }
            if (data.size() == meshnpts) {// not vector, not vector2,not matrix
                CARTESIAN_CORE_INFO("create float attribute , create as float, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, float, 0);
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);

                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, float);
                if (found) {
                    for (auto i = 0; i < data.size(); i++) {
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = data[i];
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }

            }

        }// end of create and set float attrib

        static void createAndSet_DOUBLE_CGALAttrib(PRE_TYPE::Mesh& mesh, const AttribMap& attribmap, const float& time) {
            auto meshnpts = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);
            FnAttribute::DoubleAttribute refValueAttrib = static_cast<FnAttribute::DoubleAttribute> (attribmap.handle);
            auto data = refValueAttrib.getNearestSample(time);

            if (data.size() / 3 == meshnpts) {                    // first we check the data length is equal 3 time than our cgal mesh points. if true, that's a vector attribute 
                CARTESIAN_CORE_INFO("create double attribute , create as vector, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec3, glm::vec3(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec3);
                if (found) {
                    for (auto i = 0; i < data.size() / 3; i++) {
                        auto x = data[i * 3 + 0];
                        auto y = data[i * 3 + 1];
                        auto z = data[i * 3 + 2];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec3(x, y, z);
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }
            if (data.size() == meshnpts) { // not vector, not vector2,not matrix
                CARTESIAN_CORE_INFO("create double attribute , create as double, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, double, 0.0);
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, double);
                if (found) {
                    for (auto i = 0; i < data.size(); i++) {
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = data[i];
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }

        } // end of create and set double attrib


        static void createAndSet_STR_CGALAttrib(PRE_TYPE::Mesh& mesh, const AttribMap& attribmap, const float& time) {
            auto meshnpts = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);
            FnAttribute::StringAttribute refValueAttrib = static_cast<FnAttribute::StringAttribute> (attribmap.handle);
            auto data = refValueAttrib.getNearestSample(time);

            if (data.size() == meshnpts) { // even though it's a int vector, we still recognize as a float vector
                CARTESIAN_CORE_INFO("create string attribute , create as string, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, std::string, " ");
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);

                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, std::string);
                if (found) {
                    for (auto i = 0; i < data.size(); i++) {
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = data[i];
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }
            else {
                CARTESIAN_CORE_ERROR("not support vector std::string attribute");
            }

        }// end of create and set std::string attrib



    };



    void SurfaceMeshToKatana(const PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface);



}