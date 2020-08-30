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


// katana float attribute set
#define SET_KATANA_FLOAT_ATTRIB(ATTRIB_NAME,RAW_DATA,DATA_SIZE,TUPLESIZE)\
FnAttribute::FloatAttribute attr(RAW_DATA, DATA_SIZE, TUPLESIZE);\
iface.setAttr(ATTRIB_NAME, attr, false);




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

    /*
    * This class transfer katana attributes to cgal mesh
    */
    template <typename GEO_SCOPE >
    class AttributeGetSet {

    public:
        // for int attribute
        static void createAndSet_INT_CGALAttrib(PRE_TYPE::Mesh& mesh, const AttribMap& attribmap, const float& time) {
            auto meshnpts = GetCGALScopeNum<GEO_SCOPE>::getNum(mesh);
            FnAttribute::IntAttribute refValueAttrib = static_cast<FnAttribute::IntAttribute> (attribmap.handle);
            auto data = refValueAttrib.getNearestSample(time);
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
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec4(x, y, z,w);
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
                        CGAL_FOUND_ATTRIB_MAP[GEO_SCOPE(i)] = glm::vec4(x, y, z,w);
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



    };


    
    // ------------------------------- For Transfer cgal mesh attribute to katana --------------------------------------------------------------------------------
    template <typename GEO_SCOPE>
    class SetKatanaScope {
    public:
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string &attribName){
           
        }
    };
    template <>
    class SetKatanaScope<PRE_TYPE::Face_descriptor> {
    public:
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string& attribName){
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
        static void set(Foundry::Katana::GeolibCookInterface& iface, const std::string& attribName){
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
                if (found) { // vec3

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
                    SET_KATANA_FLOAT_ATTRIB(attribValueName,rawdata, datasize * 4, 1);
                    delete[]rawdata;


                }
            }



        }
    };

    void SurfaceMeshToKatana(PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface);



}