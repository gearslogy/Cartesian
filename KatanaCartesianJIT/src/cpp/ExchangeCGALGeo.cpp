#include "ExchangeCGALGeo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__




namespace Cartesian {



    void BuildSurfaceMeshFromKatana(PRE_TYPE::Mesh& meshToBuild, Foundry::Katana::GeolibCookInterface& iface)
    {

        const std::string polyStartIndexAttName = "geometry.poly.startIndex";
        const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
        const std::string pointPosAttName = "geometry.point.P";
        const std::string arbitraryAttName = "geometry.arbitrary";


        auto time = Foundry::Katana::GetCurrentTime(iface);

        // how many num of per face;
        FnAttribute::IntAttribute faceVerticesNumArrayAttrib = iface.getAttr(polyStartIndexAttName);
        if (!faceVerticesNumArrayAttrib.isValid()) {
            CARTESIAN_CORE_ERROR("Can not find attribute: {0}", polyStartIndexAttName);
            return;
        }
        auto faceVerticesNumArray = faceVerticesNumArrayAttrib.getNearestSample(time); // first = 0


        // this will construct vertexlist order of the geo;
        FnAttribute::IntAttribute indicesArrayAttrib = iface.getAttr(polyPolyVertexIndexAttName);
        if (!indicesArrayAttrib.isValid()) {
            CARTESIAN_CORE_ERROR("Can not find attribute: {0}", polyPolyVertexIndexAttName);
            return;
        }
        auto indicesArray = indicesArrayAttrib.getNearestSample(time);


        // how many points of this geometry
        FnAttribute::FloatAttribute positionArrayAttrib = iface.getAttr(pointPosAttName);
        if (!positionArrayAttrib.isValid()) {
            CARTESIAN_CORE_ERROR("Can not find attribute: {0}", pointPosAttName);
            return;
        }

        std::vector<AttribMap> katanaAttribMaps;
        FnAttribute::GroupAttribute arbitraryAttrib = iface.getAttr(arbitraryAttName);
        int numOfArbitraryAttribs = 0;   // count of arbitrary attribute;
        if (arbitraryAttrib.isValid()) {
            numOfArbitraryAttribs = arbitraryAttrib.getNumberOfChildren();
            for (int i = 0; i < numOfArbitraryAttribs; i++) {
                std::string attribName = arbitraryAttrib.getChildName(i);

                FnAttribute::GroupAttribute attrib = arbitraryAttrib.getChildByIndex(i);  // emp: geometry.arbitrary.Cd
                /* Katana API
                #define kFnKatAttributeTypeNull 0
                #define kFnKatAttributeTypeInt 1
                #define kFnKatAttributeTypeFloat 2
                #define kFnKatAttributeTypeDouble 3
                #define kFnKatAttributeTypeString 4
                #define kFnKatAttributeTypeGroup 5
                #define kFnKatAttributeTypeError -1
                */
                FnAttribute::StringAttribute scopeAttrib = attrib.getChildByName("scope"); // geometry.arbitrary.Cd.scope
                if (!scopeAttrib.isValid()) {
                    std::string erroratt = arbitraryAttName + attribName + ".scope";
                    CARTESIAN_CORE_ERROR("can not find the scope attribute :{0}", erroratt);
                    continue;
                }
                std::string scope = scopeAttrib.getValue();                                // point or face 

                FnAttribute::Attribute valueAttrib = attrib.getChildByName("value"); // geometry.arbitrary.Cd.value
                if (!valueAttrib.isValid()) {
                    std::string erroratt = arbitraryAttName + attribName + ".value";
                    CARTESIAN_CORE_ERROR("can not find the value attribute :{0}", erroratt);
                    continue;
                }
                int valueType = valueAttrib.getType();
                if (valueType == 1) { // it's int attribute
                    FnAttribute::IntAttribute refValueAttrib = static_cast<FnAttribute::IntAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map :{0}, type: {1}, tuple size: {2}, scope:{3}", attribName, "int", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 1, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
                if (valueType == 2) { // it's float attribute
                    FnAttribute::FloatAttribute refValueAttrib = static_cast<FnAttribute::FloatAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map:{0}, type: {1}, tuple size: {2}, scope:{3}", attribName, "float", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 2, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
                if (valueType == 3) { // it's double attribute
                    FnAttribute::DoubleAttribute refValueAttrib = static_cast<FnAttribute::DoubleAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map :{0}, type: {1}, tuple size: {2}, scope:{3}", attribName, "double", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 3, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
                if (valueType == 4) { // it's string attribute
                    FnAttribute::StringAttribute refValueAttrib = static_cast<FnAttribute::StringAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map :{0}, type: {1}, tuple size: {2}, scope:{3}", attribName, "string", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 4, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
            }
        }
        CARTESIAN_CORE_INFO("success parsing attribute num:{0}", katanaAttribMaps.size());


        auto positionArray = positionArrayAttrib.getNearestSample(time);
        auto numpts = positionArray.size() / 3; // per point has x y z, in katana it's a flat array
        // ------------------- Create Geometry points ----------------------------------
        for (auto i = 0; i < numpts; i++) {
            auto xindex = i * 3 + 0;
            auto yindex = i * 3 + 1;
            auto zindex = i * 3 + 2;
            auto x = positionArray[xindex];
            auto y = positionArray[yindex];
            auto z = positionArray[zindex];
            auto vd = meshToBuild.add_vertex(PRE_TYPE::K::Point_3(x, y, z));
        }
        // ------------------- Create Geometry points ----------------------------------


        // ------------------- Create Geometry Face ----------------------------------
        int indicesArrayIndex = 0;
        for (int f = 1; f < faceVerticesNumArray.size(); f++) {
            int faceNum = faceVerticesNumArray[f] - faceVerticesNumArray[f - 1];
            // run loop on per face 
            std::vector <PRE_TYPE::Vertex_descriptor> vertices;
            for (int i = 0; i < faceNum; i++) {
                int ptindex = indicesArray[indicesArrayIndex];
                PRE_TYPE::Vertex_descriptor CGAL_VertexID(ptindex);
                vertices.emplace_back(CGAL_VertexID);
                indicesArrayIndex++;
            }
            // use range function construct a face
            meshToBuild.add_face(vertices);
        }
        // ------------------- Create Geometry Face ----------------------------------

        int meshnpts = meshToBuild.num_vertices();
        // attribute building: create attribute
        for (auto& attribmap : katanaAttribMaps) {
            if (attribmap.scope == "point")
            {
                switch (attribmap.type)
                {
                case 1:  // ----------------------------- int for point ---------------------------------------------------
                {
                    AttributeGetSet<PRE_TYPE::Vertex_descriptor>::createAndSet_INT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 2: // ----------------------------- float for point ---------------------------------------------------
                {
                    AttributeGetSet<PRE_TYPE::Vertex_descriptor>::createAndSet_FLT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 3: // ----------------------------- double for point ---------------------------------------------------
                {
                    AttributeGetSet<PRE_TYPE::Vertex_descriptor>::createAndSet_DOUBLE_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 4: { // ----------------------------- string for point ---------------------------------------------------
                    AttributeGetSet<PRE_TYPE::Vertex_descriptor>::createAndSet_STR_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }

                default:
                    break;

                }


            }


            if (attribmap.scope == "face")
            {
               
                switch (attribmap.type)
                {
                case 1:  // ----------------------------- int for face ---------------------------------------------------
                {
                    AttributeGetSet<PRE_TYPE::Face_descriptor>::createAndSet_INT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 2: // ----------------------------- float for face ---------------------------------------------------
                {
                    AttributeGetSet<PRE_TYPE::Face_descriptor>::createAndSet_FLT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 3: // ----------------------------- double for face ---------------------------------------------------
                {
                    AttributeGetSet<PRE_TYPE::Face_descriptor>::createAndSet_DOUBLE_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 4: { // ----------------------------- string for face ---------------------------------------------------
                    AttributeGetSet<PRE_TYPE::Face_descriptor>::createAndSet_STR_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }

                default:
                    break;

                }

            }

        } // end of create attribute;



    } // end of build surface mesh

    void SurfaceMeshToKatana(const PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface)
    {

    }// end of surface mesh to katana

}