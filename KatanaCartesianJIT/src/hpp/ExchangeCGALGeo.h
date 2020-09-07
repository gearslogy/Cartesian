#pragma once




#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__

//#define SOL_NO_EXCEPTIONS 1
#include <FnGeolib/op/FnGeolibOp.h>
#include "BindCGAL_DefineType.h"
#include <vector>
#define CARTESIAN_CLOSE_LOG
#include "CartesianLog.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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


// katana float attribute set
#define SET_KATANA_FLOAT_ATTRIB(ATTRIB_NAME,RAW_DATA,DATA_SIZE,TUPLESIZE)\
FnAttribute::FloatAttribute attr(RAW_DATA, DATA_SIZE, TUPLESIZE);\
iface.setAttr(ATTRIB_NAME, attr, false);




namespace Cartesian {

    std::string getBaseTypeAsString(const int baseType);
    int getTupleSize(FnAttribute::Attribute& attrib, const int baseType);






    struct AttribMap {
        std::string name;                       // attrib name
        std::string scope;                      // face or vertex?

        // int float string double? same as katana API
        /* Katana API
        #define kFnKatAttributeTypeNull 0
        #define kFnKatAttributeTypeInt 1
        #define kFnKatAttributeTypeFloat 2
        #define kFnKatAttributeTypeDouble 3
        #define kFnKatAttributeTypeString 4
        #define kFnKatAttributeTypeGroup 5
        #define kFnKatAttributeTypeError -1
        */
        unsigned int type;



        // is value attribute ? or index&&indexedValue attribute?
        bool isIndexed = false;

        // handle for attribute values only like this attribute: emp:geometry.arbitrary.Cd.value
        FnAttribute::Attribute handle;



        // index &  indexedValue handle on for contained .index && .indexedValue -> like this: st/Texture attributes
        FnAttribute::Attribute indexHandle;
        FnAttribute::Attribute indexedValueHandle;


        // but i find tupleSize useless. just check attribute's value.size()/3 == npts that's vector!
        // recording it only for respecting.
        unsigned int tupleSize;                 // 1-> float, 3 -> vector, 4 -> vector4

    };

    template <typename T>
    struct IndexedValueType {
        IndexedValueType() {

        }
        IndexedValueType(const  T& rhs) :value(rhs) {}
        T value;

    };

    void CreateAndBuildCGAL_vertexnum(PRE_TYPE::Mesh& mesh);

    // if topo update, the vertex num order need update. emp: removepoint()
    void ReBuildCGAL_vertexnum(PRE_TYPE::Mesh& mesh);

    // if read new mesh to katana, emp: readmesh(m, "d:/test.off")
    void TryToBuildCGAL_vertexnum(PRE_TYPE::Mesh& mesh);


    template <typename KATANA_ATTRIB_TYPE, typename CGAL_ATTRIB_DATA_TYPE>
    class IndexedValueHelper {
    public:

        // this attribute repr point vertex number
        using VertexNumAttrib = PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int>;

        static void createAndSet_simpleCGALIndexedValue(PRE_TYPE::Mesh& mesh, const VertexNumAttrib& vertexnumAttrib, AttribMap& attribmap, const float time) {
            CARTESIAN_CORE_INFO("map the katana geometry.arbitrary.{0}.indexedvalue", attribmap.name);
            // Get katana indexedValue
            KATANA_ATTRIB_TYPE indexValueAttr = static_cast<KATANA_ATTRIB_TYPE> (attribmap.indexedValueHandle);
            auto sampleData = indexValueAttr.getNearestSample(time);
            CREATE_CGAL_ATTRIB(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE, CGAL_ATTRIB_DATA_TYPE());
            if (!created) CARTESIAN_CORE_ERROR("can not create attribute: {0} for simple data type", attribmap.name);
            FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE);
            if (!found) CARTESIAN_CORE_ERROR("can not found attribute: {0} for simple data type", attribmap.name);
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    int vertnum = vertexnumAttrib[v];  // get current vertex number of current point:v
                    CGAL_FOUND_ATTRIB_MAP[v].value = sampleData[vertnum];
                }
            }
        }

        static void createAndSet_Vector2CGALIndexedValue(PRE_TYPE::Mesh& mesh, const VertexNumAttrib& vertexnumAttrib, AttribMap& attribmap, const float time) {
            CARTESIAN_CORE_INFO("map the katana geometry.arbitrary.{0}.indexedvalue", attribmap.name);
            CREATE_CGAL_ATTRIB(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE, CGAL_ATTRIB_DATA_TYPE());
            if (!created) CARTESIAN_CORE_ERROR("can not create attribute: {0} for simple data type: {1}", attribmap.name, "vector2");
            FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE);
            if (!found) CARTESIAN_CORE_ERROR("can not found attribute: {0} for simple data type: {1}", attribmap.name, "vector2");
            // Get katana indexedValue
            KATANA_ATTRIB_TYPE indexValueAttr = static_cast<KATANA_ATTRIB_TYPE> (attribmap.indexedValueHandle);
            auto sampleData = indexValueAttr.getNearestSample(time);

            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    int vertnum = vertexnumAttrib[v];  // get current vertex number of current point:v
                    float x = sampleData[vertnum * 2 + 0];
                    float y = sampleData[vertnum * 2 + 1];
                    CGAL_FOUND_ATTRIB_MAP[v].value.x = x;
                    CGAL_FOUND_ATTRIB_MAP[v].value.y = y;
                }
            }

        }
        static void createAndSet_VectorCGALIndexedValue(PRE_TYPE::Mesh& mesh, const VertexNumAttrib& vertexnumAttrib, AttribMap& attribmap, const float time) {
            CARTESIAN_CORE_INFO("map the katana geometry.arbitrary.{0}.indexedvalue", attribmap.name);
            CREATE_CGAL_ATTRIB(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE, CGAL_ATTRIB_DATA_TYPE());
            if (!created) CARTESIAN_CORE_ERROR("can not create attribute: {0} for simple data type: {1}", attribmap.name, "vector");
            FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE);
            if (!found) CARTESIAN_CORE_ERROR("can not found attribute: {0} for simple data type: {1}", attribmap.name, "vector");
            // Get katana indexedValue
            KATANA_ATTRIB_TYPE indexValueAttr = static_cast<KATANA_ATTRIB_TYPE> (attribmap.indexedValueHandle);
            auto sampleData = indexValueAttr.getNearestSample(time);

            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    int vertnum = vertexnumAttrib[v];  // get current vertex number of current point:v
                    float x = sampleData[vertnum * 3 + 0];
                    float y = sampleData[vertnum * 3 + 1];
                    float z = sampleData[vertnum * 3 + 2];
                    CGAL_FOUND_ATTRIB_MAP[v].value.x = x;
                    CGAL_FOUND_ATTRIB_MAP[v].value.y = y;
                    CGAL_FOUND_ATTRIB_MAP[v].value.z = z;
                }
            }

        }
        static void createAndSet_Vector4CGALIndexedValue(PRE_TYPE::Mesh& mesh, const VertexNumAttrib& vertexnumAttrib, AttribMap& attribmap, const float time) {
            CARTESIAN_CORE_INFO("map the katana geometry.arbitrary.{0}.indexedvalue", attribmap.name);
            CREATE_CGAL_ATTRIB(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE, CGAL_ATTRIB_DATA_TYPE());
            if (!created) CARTESIAN_CORE_ERROR("can not create attribute: {0} for simple data type: {1}", attribmap.name, "vector4");
            FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, PRE_TYPE::Vertex_descriptor, CGAL_ATTRIB_DATA_TYPE);
            if (!found) CARTESIAN_CORE_ERROR("can not found attribute: {0} for simple data type: {1}", attribmap.name, "vector4");
            // Get katana indexedValue
            KATANA_ATTRIB_TYPE indexValueAttr = static_cast<KATANA_ATTRIB_TYPE> (attribmap.indexedValueHandle);
            auto sampleData = indexValueAttr.getNearestSample(time);

            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    int vertnum = vertexnumAttrib[v];  // get current vertex number of current point:v
                    float x = sampleData[vertnum * 4 + 0];
                    float y = sampleData[vertnum * 4 + 1];
                    float z = sampleData[vertnum * 4 + 2];
                    float w = sampleData[vertnum * 4 + 3];
                    CGAL_FOUND_ATTRIB_MAP[v].value.x = x;
                    CGAL_FOUND_ATTRIB_MAP[v].value.y = y;
                    CGAL_FOUND_ATTRIB_MAP[v].value.z = z;
                    CGAL_FOUND_ATTRIB_MAP[v].value.w = w;
                }
            }
        }





    };






    /**
         * @brief build a cgal mesh from katana
         * @param meshToBuild cgal mesh
         * @param iface katana interface
         * @param location location of input index
         * @param index input index
         * @return
    */
    void BuildSurfaceMeshFromKatana(PRE_TYPE::Mesh& meshToBuild,
        Foundry::Katana::GeolibCookInterface& iface,
        const std::string& location = " ",
        const int& index = -1);



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

    /*
    * This class transfer katana attributes to cgal mesh
    */
    template <typename GEO_SCOPE >
    class AttributeFromKatanaGetSetToCGAL {

    public:
        // for int attribute
        static void createAndSet_INT_CGALAttrib(PRE_TYPE::Mesh& mesh, const AttribMap& attribmap, const float& time) {
            auto meshnpts = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);
            FnAttribute::IntAttribute refValueAttrib = static_cast<FnAttribute::IntAttribute> (attribmap.handle);
            auto data = refValueAttrib.getNearestSample(time);

            if (data.size() / 2 == meshnpts)                    // if true, that's a vector attribute 
            {
                CARTESIAN_CORE_INFO("create int attribute , create as vector2, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec2, glm::vec2(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);

                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec2);
                if (found) {
                    for (auto i = 0; i < data.size() / 2; i++) {
                        auto x = data[i * 2 + 0];
                        auto y = data[i * 2 + 1];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec2(x, y);
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }


            if (data.size() / 3 == meshnpts)                    // if true, that's a vector attribute 
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

            if (data.size() / 4 == meshnpts)                    // if true, that's a vector4 attribute 
            {
                CARTESIAN_CORE_INFO("create int attribute , create as vector4, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec4, glm::vec4(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);

                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec4);
                if (found) {
                    for (auto i = 0; i < data.size() / 4; i++) {
                        auto x = data[i * 4 + 0];
                        auto y = data[i * 4 + 1];
                        auto z = data[i * 4 + 2];
                        auto w = data[i * 4 + 3];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec4(x, y, z, w);
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
            if (data.size() / 2 == meshnpts) {
                CARTESIAN_CORE_INFO("create float attribute , create as vector2, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec2, glm::vec2(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec2);
                if (found) {
                    for (auto i = 0; i < data.size() / 2; i++) {
                        auto x = data[i * 2 + 0];
                        auto y = data[i * 2 + 1];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec2(x, y);
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }
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
            if (data.size() / 4 == meshnpts) {
                CARTESIAN_CORE_INFO("create float attribute , create as vector4, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec4, glm::vec4(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec4);
                if (found) {
                    for (auto i = 0; i < data.size() / 4; i++) {
                        auto x = data[i * 4 + 0];
                        auto y = data[i * 4 + 1];
                        auto z = data[i * 4 + 2];
                        auto w = data[i * 4 + 3];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec4(x, y, z, w);
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
            if (data.size() / 2 == meshnpts) {                    // first we check the data length is equal 3 time than our cgal mesh points. if true, that's a vector attribute 
                CARTESIAN_CORE_INFO("create double attribute , create as vector2, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec2, glm::vec2(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec2);
                if (found) {
                    for (auto i = 0; i < data.size() / 2; i++) {
                        auto x = data[i * 2 + 0];
                        auto y = data[i * 2 + 1];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec2(x, y);
                    }
                }
                else {
                    CARTESIAN_CORE_ERROR("cgal can not find attribute: {0} for set value", attribmap.name);
                }
            }
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
            if (data.size() / 4 == meshnpts) {
                CARTESIAN_CORE_INFO("create double attribute , create as vector4, name: {0} on {1}", attribmap.name, attribmap.scope);
                CREATE_CGAL_ATTRIB(mesh, attribmap.name, GEO_SCOPE, glm::vec4, glm::vec4(0));
                if (!created) CARTESIAN_CORE_ERROR("error create attribute:{0}", attribmap.name);
                // find and modifier it
                FIND_CGAL_ATTRIBUTE(mesh, attribmap.name, GEO_SCOPE, glm::vec4);
                if (found) {
                    for (auto i = 0; i < data.size() / 4; i++) {
                        auto x = data[i * 4 + 0];
                        auto y = data[i * 4 + 1];
                        auto z = data[i * 4 + 2];
                        auto w = data[i * 4 + 3];
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec4(x, y, z, w);
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

        // uv attribute also for per face, so we save the indexed attribut for face


    };



    // ------------------------------- For Transfer cgal mesh attribute to katana --------------------------------------------------------------------------------
    template <typename GEO_SCOPE>
    class SetKatanaScope {
    public:
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string& attribName) {

        }
    };
    template <>
    class SetKatanaScope<PRE_TYPE::Face_descriptor> {
    public:
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string& attribName) {
            std::string arbitraryAttName = "geometry.arbitrary";
            arbitraryAttName += "."; arbitraryAttName += attribName;
            arbitraryAttName += ".scope";
            FnAttribute::StringAttribute attr("face");
            iface.setAttr(arbitraryAttName, attr, false);
        }
    };
    template <>
    class SetKatanaScope<PRE_TYPE::Vertex_descriptor> {
    public:
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string& attribName) {
            std::string arbitraryAttName = "geometry.arbitrary";
            arbitraryAttName += "."; arbitraryAttName += attribName;
            arbitraryAttName += ".scope";
            FnAttribute::StringAttribute attr("point");
            iface.setAttr(arbitraryAttName, attr, false);
        }
    };
    class KatanaElementSizeAttrib {
    public:
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string& attribName) {
            // geometry.arbitrary.name.elementSize
            std::string arbitraryAttName = "geometry.arbitrary";
            std::string elementSizeName = arbitraryAttName + "." + attribName + ".elementSize";
            FnAttribute::IntAttribute elementSizeNameAttr(4);
            iface.setAttr(elementSizeName, elementSizeNameAttr, false);
        }
    };


    /*
    * this class transfer cgal mesh attributes to katana
    */
    template <typename GEO_SCOPE>
    class AttributeToKatana {
    public:
        static void setvalues(const PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface) {
            const std::string polyStartIndexAttName = "geometry.poly.startIndex";
            const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
            const std::string pointPosAttName = "geometry.point.P";
            const std::string arbitraryAttName = "geometry.arbitrary";

            // get allocate size for set katana attribute
            int datasize = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);

            // point attribute
            for (auto& name : mesh.properties<GEO_SCOPE>())
            {
                PRE_TYPE::Mesh::Property_map<GEO_SCOPE, int> attribMapInt;
                bool found = false;
                boost::tie(attribMapInt, found) = mesh.property_map<GEO_SCOPE, int>(name);
                if (found) { // int
                    int* rawdata = new int[datasize];
                    for (auto i = 0; i < datasize; i++) {
                        rawdata[i] = attribMapInt[GEO_SCOPE(i)];
                    }
                    std::string intattname = arbitraryAttName + "." + name + ".value";
                    CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", intattname, "int");
                    FnAttribute::IntAttribute intAttr(rawdata, datasize, 1);
                    iface.setAttr(intattname, intAttr, false);
                    delete[]rawdata;

                    // geometry.arbitrary.name.scope
                    SetKatanaScope<GEO_SCOPE>::set(iface, name);
                }


                found = false;
                PRE_TYPE::Mesh::Property_map<GEO_SCOPE, float> attribMapFloat;
                boost::tie(attribMapFloat, found) = mesh.property_map<GEO_SCOPE, float>(name);
                if (found) { // float
                     // geometry.arbitrary.name.scope
                    SetKatanaScope<GEO_SCOPE>::set(iface, name);

                    float* rawdata = new float[datasize];
                    for (auto i = 0; i < datasize; i++) {
                        rawdata[i] = attribMapFloat[GEO_SCOPE(i)];
                    }
                    // geometry.arbitrary.name.value
                    std::string floatattrname = arbitraryAttName + "." + name + ".value";
                    CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", floatattrname, "float");
                    SET_KATANA_FLOAT_ATTRIB(floatattrname, rawdata, datasize, 1);
                    delete[]rawdata;

                }

                found = false;
                PRE_TYPE::Mesh::Property_map<GEO_SCOPE, double> attribMapDouble;
                boost::tie(attribMapDouble, found) = mesh.property_map<GEO_SCOPE, double>(name);
                if (found) { // double
                     // geometry.arbitrary.name.scope
                    SetKatanaScope<GEO_SCOPE>::set(iface, name);

                    double* rawdata = new double[datasize];
                    for (auto i = 0; i < datasize; i++) {
                        rawdata[i] = attribMapDouble[GEO_SCOPE(i)];
                    }
                    // geometry.arbitrary.name.value
                    std::string attribValueName = arbitraryAttName + "." + name + ".value";
                    CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "double");
                    FnAttribute::DoubleAttribute attr(rawdata, datasize, 1);
                    iface.setAttr(attribValueName, attr, false);
                    delete[]rawdata;

                }

                found = false;
                PRE_TYPE::Mesh::Property_map<GEO_SCOPE, std::string> attribMapString;
                boost::tie(attribMapString, found) = mesh.property_map<GEO_SCOPE, std::string>(name);
                if (found) { // string
                    std::string* rawdata = new std::string[datasize];
                    for (auto i = 0; i < datasize; i++) {
                        rawdata[i] = attribMapString[GEO_SCOPE(i)];
                    }
                    // geometry.arbitrary.name.value
                    std::string attribValueName = arbitraryAttName + "." + name + ".value";
                    CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "string");
                    FnAttribute::StringAttribute attr(rawdata, datasize, 1);
                    iface.setAttr(attribValueName, attr, false);
                    delete[]rawdata;

                    // geometry.arbitrary.name.scope
                    SetKatanaScope<GEO_SCOPE>::set(iface, name);
                }


                // vector 
                found = false;
                PRE_TYPE::Mesh::Property_map<GEO_SCOPE, glm::vec3> Vec3AttribMap;
                boost::tie(Vec3AttribMap, found) = mesh.property_map<GEO_SCOPE, glm::vec3>(name);
                if (found) { // vec3

                     // geometry.arbitrary.name.inputType
                    std::string inputTypeName = arbitraryAttName + "." + name + ".inputType";
                    FnAttribute::StringAttribute inputTypeAttr("vector3");
                    iface.setAttr(inputTypeName, inputTypeAttr, false);

                    // geometry.arbitrary.name.scope
                    SetKatanaScope<GEO_SCOPE>::set(iface, name);


                    float* rawdata = new float[datasize * 3];
                    for (auto i = 0; i < datasize; i++) {
                        glm::vec3 value = Vec3AttribMap[GEO_SCOPE(i)];
                        rawdata[i * 3 + 0] = value.x;
                        rawdata[i * 3 + 1] = value.y;
                        rawdata[i * 3 + 2] = value.z;
                    }

                    // geometry.arbitrary.name.value
                    std::string attribValueName = arbitraryAttName + "." + name + ".value";
                    CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "float vector");
                    SET_KATANA_FLOAT_ATTRIB(attribValueName, rawdata, datasize * 3, 3);
                    delete[]rawdata;


                }

                // vector4 
                found = false;
                PRE_TYPE::Mesh::Property_map<GEO_SCOPE, glm::vec4> Vec4AttribMap;
                boost::tie(Vec4AttribMap, found) = mesh.property_map<GEO_SCOPE, glm::vec4>(name);
                if (found) { // vec4

                    // geometry.arbitrary.name.scope
                    SetKatanaScope<GEO_SCOPE>::set(iface, name);

                    KatanaElementSizeAttrib::set(iface, name);

                    float* rawdata = new float[datasize * 4];
                    for (auto i = 0; i < datasize; i++) {
                        glm::vec4 value = Vec4AttribMap[GEO_SCOPE(i)];
                        rawdata[i * 4 + 0] = value.x;
                        rawdata[i * 4 + 1] = value.y;
                        rawdata[i * 4 + 2] = value.z;
                        rawdata[i * 4 + 3] = value.w;
                    }

                    // geometry.arbitrary.name.value
                    std::string attribValueName = arbitraryAttName + "." + name + ".value";
                    CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "float vector4");
                    SET_KATANA_FLOAT_ATTRIB(attribValueName, rawdata, datasize * 4, 1);
                    delete[]rawdata;
                }
            }



        }
    };




    


    /*
    * this class transfer the cgal IndexAttribute to katana
    */
    class IndexedAttribToKatana {
    public:

        using VertexNumPropMap = PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int>;

        static void buildIndex(const PRE_TYPE::Mesh &mesh, std::vector<int> &indexdata, VertexNumPropMap & vertexnumAttrib){
            // build the "geometry.arbitrary.name.index" data
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    indexdata.emplace_back(vertexnumAttrib[v]);
                }
            }
        }
        // build the katana attribute "geometry.arbitrary.name.index"
        static void buildIndexKatanaAttrib(const PRE_TYPE::Mesh& mesh, const std::string &name, VertexNumPropMap& vertexnumAttrib, Foundry::Katana::GeolibCookInterface& iface) {
            const std::string arbitraryAttName = "geometry.arbitrary";
            std::vector <int> datatobuild;
            buildIndex(mesh, datatobuild, vertexnumAttrib);
            std::string indexAttribName = arbitraryAttName + "." + name + ".index";
            FnAttribute::IntAttribute indexAttrb(datatobuild.data(), datatobuild.size(), 1);
            iface.setAttr(indexAttribName, indexAttrb, false);
        }

        // build the katana attribute "geometry.arbitrary.name.scope" , keep to vertex
        static void buildScopeKatanaAttrib( const std::string& name, Foundry::Katana::GeolibCookInterface& iface) {
            const std::string arbitraryAttName = "geometry.arbitrary";
            std::string indexAttribName = arbitraryAttName + "." + name + ".scope";
            FnAttribute::StringAttribute indexAttrb("vertex");
            iface.setAttr(indexAttribName, indexAttrb, false);
        }

        // find this type in https://learn.foundry.com/katana/dev-guide/AttributeConventions/ArbitraryAttributes.html
        static void buildInputTypeKatanaAttrib(const std::string& name, const std::string &typeValue, Foundry::Katana::GeolibCookInterface& iface) {
            const std::string arbitraryAttName = "geometry.arbitrary";
            std::string indexAttribName = arbitraryAttName + "." + name + ".inputType";
            FnAttribute::StringAttribute indexAttrb(typeValue);
            iface.setAttr(indexAttribName, indexAttrb, false);
        }


        /*
        * Get the attribute "CGAL_vertexnum" fo max
        */
        static int getMaxVertexIndexNum(PRE_TYPE::Mesh& mesh)  // not point index(not PRE_TYPE::Vertex_Descriptor)
        {
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> vertexnumAttrib;
            bool foundVertexNumAttrib = false;
            boost::tie(vertexnumAttrib, foundVertexNumAttrib) = mesh.property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum");


            int maxvertxnum = -1;
            for (auto& vd : mesh.vertices()) {
                auto vertnum = vertexnumAttrib[vd];
                if (vertnum > maxvertxnum)
                    maxvertxnum = vertnum;
                
            }
            return maxvertxnum;
        }


        static void setvalues(PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface)
        {
            const std::string polyStartIndexAttName = "geometry.poly.startIndex";
            const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
            const std::string pointPosAttName = "geometry.point.P";
            const std::string arbitraryAttName = "geometry.arbitrary";
            // -------------------------- for indexed value ------------------------------------

            auto maxVertNum = getMaxVertexIndexNum(mesh);

            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> vertexnumAttrib;
            bool foundVertexNumAttrib = false;
            boost::tie(vertexnumAttrib, foundVertexNumAttrib) = mesh.property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum");
            if (!foundVertexNumAttrib)
            {
                CARTESIAN_CORE_ERROR("can not build index attribute from CGAL_vertexnum attribute, line: {0}", __LINE__);
                return;
            }

            for (auto& name : mesh.properties<PRE_TYPE::Vertex_descriptor>())
            {
                // Get CGAL int indexValue
                bool found = false;
                PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<int>> intIndexValueMap;
                boost::tie(intIndexValueMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<int> >(name);

                if (found) {
                    buildScopeKatanaAttrib( name, iface);
                    buildInputTypeKatanaAttrib(name, "int", iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<int> indexedValueData;
                    indexedValueData.resize(maxVertNum + 1);
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum] = intIndexValueMap[vd].value;
                     }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "in");
                    FnAttribute::IntAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 1);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);

                }

                found = false;
                PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<float>> floatIndexValueMap;
                boost::tie(floatIndexValueMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<float> >(name);
                if (found) {
                    buildScopeKatanaAttrib(name, iface);
                    buildInputTypeKatanaAttrib(name, "float", iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<float> indexedValueData;
                    indexedValueData.resize(maxVertNum + 1);
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum] = floatIndexValueMap[vd].value;
                    }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "float");
                    FnAttribute::FloatAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 1);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);
                }


                found = false;
                PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<double>> doubleIndexValueMap;
                boost::tie(doubleIndexValueMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<double> >(name);
                if (found) {
                    buildScopeKatanaAttrib(name, iface);
                    buildInputTypeKatanaAttrib(name, "double", iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<double> indexedValueData;
                    indexedValueData.resize(maxVertNum + 1);
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum] = doubleIndexValueMap[vd].value;
                    }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "double");
                    FnAttribute::DoubleAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 1);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);
                }


                found = false;
                PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<std::string>> stringIndexValueMap;
                boost::tie(stringIndexValueMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<std::string> >(name);
                if (found) {
                    buildScopeKatanaAttrib(name, iface);
                    buildInputTypeKatanaAttrib(name, "string", iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<std::string> indexedValueData;
                    indexedValueData.resize(maxVertNum + 1);
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum] = stringIndexValueMap[vd].value;
                    }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "string");
                    FnAttribute::StringAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 1);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);
                }



                found = false;
                PRE_TYPE::Mesh::Property_map < PRE_TYPE::Vertex_descriptor, IndexedValueType < glm::vec2 >> vec2IndexValueMap;
                boost::tie(vec2IndexValueMap, found) = mesh.property_map < PRE_TYPE::Vertex_descriptor, IndexedValueType<glm::vec2> >(name);
                if (found) {
                    buildInputTypeKatanaAttrib(name, "vector2", iface);
                    buildScopeKatanaAttrib(name, iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<float> indexedValueData;
                    indexedValueData.resize( (maxVertNum + 1) * 2);  // 0-max vertex num * 2, katana require flat raw data
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum * 2 + 0] = vec2IndexValueMap[vd].value.x;
                        indexedValueData[vertnum * 2 + 1] = vec2IndexValueMap[vd].value.y;
                    }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "vector2");
                    FnAttribute::FloatAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 2);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);
                }



                found = false;
                PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<glm::vec3>> vec3IndexValueMap;
                boost::tie(vec3IndexValueMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<glm::vec3> >(name);
                if (found) {
                    buildInputTypeKatanaAttrib(name, "vector3", iface);
                    buildScopeKatanaAttrib(name, iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<float> indexedValueData;
                    indexedValueData.resize((maxVertNum + 1) * 3);  // 0-max vertex num * 2, katana require flat raw data
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum * 3 + 0] = vec3IndexValueMap[vd].value.x;
                        indexedValueData[vertnum * 3 + 1] = vec3IndexValueMap[vd].value.y;
                        indexedValueData[vertnum * 3 + 2] = vec3IndexValueMap[vd].value.y;
                    }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "vector");
                    FnAttribute::FloatAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 3);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);
                }


                found = false;
                PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<glm::vec4>> vec4IndexValueMap;
                boost::tie(vec4IndexValueMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, IndexedValueType<glm::vec4> >(name);
                if (found) {
                    buildInputTypeKatanaAttrib(name, "vector4", iface);
                    buildScopeKatanaAttrib(name, iface);
                    // build the "geometry.arbitrary.name.index"
                    buildIndexKatanaAttrib(mesh, name, vertexnumAttrib, iface);

                    std::vector<float> indexedValueData;
                    indexedValueData.resize((maxVertNum + 1) * 4);  // 0-max vertex num * 2, katana require flat raw data
                    for (auto& vd : mesh.vertices()) {
                        auto vertnum = vertexnumAttrib[vd];
                        indexedValueData[vertnum * 4 + 0] = vec4IndexValueMap[vd].value.x;
                        indexedValueData[vertnum * 4 + 1] = vec4IndexValueMap[vd].value.y;
                        indexedValueData[vertnum * 4 + 2] = vec4IndexValueMap[vd].value.y;
                        indexedValueData[vertnum * 4 + 3] = vec4IndexValueMap[vd].value.z;
                    }
                    // build the "geometry.arbitrary.name.indexedValue"
                    std::string indexedValueAttribName = arbitraryAttName + "." + name + ".indexedValue";
                    CARTESIAN_CORE_WARN("create attribute: {0}, value type: {1}", indexedValueAttribName, "vector4");
                    FnAttribute::FloatAttribute indexedValueAttr(indexedValueData.data(), indexedValueData.size(), 3);
                    iface.setAttr(indexedValueAttribName, indexedValueAttr, false);
                }
            }
        }

    };


   
    class AttributeVertexNFromKatanaToGCGAL {
    public:
        static void build(PRE_TYPE::Mesh& meshToBuild, Foundry::Katana::GeolibCookInterface& iface, const std::string& location,
            const int& index, const float time) {

            const std::string vertexNAttName = "geometry.vertex.N";
            // now parse the geometry.vertex.N, cartesian will only support parse goemetry.vertex.N
            // save the N to face, because face can use CGAL graph iterator 
            FnAttribute::FloatAttribute VertexNAttrib;
            if (index == -1)
                VertexNAttrib = iface.getAttr(vertexNAttName);
            else
                VertexNAttrib = iface.getAttr(vertexNAttName, location, index);
            if (VertexNAttrib.isValid()) {
                CARTESIAN_CORE_WARN("build geometry.vertex.N, save to cgal sope:vertex, named:CGAL_N");
                CREATE_CGAL_ATTRIB(meshToBuild, "CGAL_N", PRE_TYPE::Vertex_descriptor, glm::vec3, glm::vec3(0));
                FIND_CGAL_ATTRIBUTE(meshToBuild, "CGAL_N", PRE_TYPE::Vertex_descriptor,glm::vec3);
                auto vertexNormalSample = VertexNAttrib.getNearestSample(time);

                int iterindex = 0;
                for (PRE_TYPE::Face_descriptor& f : meshToBuild.faces()) {
                    for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(meshToBuild.halfedge(f), meshToBuild)) {
                        auto xindex = iterindex * 3 + 0;
                        auto yindex = iterindex * 3 + 1;
                        auto zindex = iterindex * 3 + 2;
                        auto x = vertexNormalSample[xindex];
                        auto y = vertexNormalSample[yindex];
                        auto z = vertexNormalSample[zindex];
                        CGAL_FOUND_ATTRIB_MAP[v] = glm::vec3(x, y, z);
                        iterindex++;
                    }
                }
            }
            else {
                CARTESIAN_CORE_ERROR("can not find geometry.vertex.N, cartesian will not use katana N attribute");
            }
        }
    };
    


    
    class AttributeVertexNFromGCGALToKatana {
    public:
        static void calNormal(const PRE_TYPE::Mesh &mesh) {

        }



        static void setvalues(const PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface) {
            FIND_CGAL_ATTRIBUTE(mesh, "CGAL_N", PRE_TYPE::Vertex_descriptor, glm::vec3);
            if (!found) {
                CARTESIAN_CORE_ERROR("can not find cgalN to construct geometry.vertex.N, use the CGAL function cal the normal");
                return;
            }
            CARTESIAN_CORE_INFO("build construct geometry.vertex.N from CGAL_N");
            const std::string vertexNAttName = "geometry.vertex.N";
            std::vector<float>datatobuild;
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                auto numedges = mesh.degree(f);
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    auto& vec = CGAL_FOUND_ATTRIB_MAP[v];
                    datatobuild.emplace_back(vec.x);
                    datatobuild.emplace_back(vec.y);
                    datatobuild.emplace_back(vec.z);
                }
            }

            SET_KATANA_FLOAT_ATTRIB(vertexNAttName, datatobuild.data(), datatobuild.size(), 3);
        }
    };
    


    void SurfaceMeshToKatana(PRE_TYPE::Mesh& mesh, bool buildFace, Foundry::Katana::GeolibCookInterface& iface);



}