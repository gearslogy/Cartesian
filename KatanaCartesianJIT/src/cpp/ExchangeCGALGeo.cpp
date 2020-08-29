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
                    CARTESIAN_CORE_INFO("gen attribute map: {0}, type: {1}, tuple size: {2}, scope: {3}", attribName, "int", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 1, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
                if (valueType == 2) { // it's float attribute
                    FnAttribute::FloatAttribute refValueAttrib = static_cast<FnAttribute::FloatAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map: {0}, type: {1}, tuple size: {2}, scope: {3}", attribName, "float", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 2, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
                if (valueType == 3) { // it's double attribute
                    FnAttribute::DoubleAttribute refValueAttrib = static_cast<FnAttribute::DoubleAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map: {0}, type: {1}, tuple size: {2}, scope: {3}", attribName, "double", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, 3, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }
                if (valueType == 4) { // it's string attribute
                    FnAttribute::StringAttribute refValueAttrib = static_cast<FnAttribute::StringAttribute> (valueAttrib);
                    auto tupleSize = refValueAttrib.getTupleSize();
                    CARTESIAN_CORE_INFO("gen attribute map: {0}, type: {1}, tuple size: {2}, scope: {3}", attribName, "string", tupleSize, scope);
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

    void SurfaceMeshToKatana( PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface)
    {
     
        
        

        const std::string polyStartIndexAttName = "geometry.poly.startIndex";
        const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
        const std::string pointPosAttName = "geometry.point.P";
        const std::string arbitraryAttName = "geometry.arbitrary";

        



        // ------------------ construct geometry.poly.startIndex attribute -----------------------------
        // this actually is likely primpoints()
        std::vector <int> startIndexData; 
        std::vector <int> vertexListData;
        startIndexData.push_back(0); // katana require the first data is 0
        /*
        for (PRE_TYPE::face_iterator faceIter = mesh.faces_begin(); faceIter != mesh.faces_end(); faceIter++) {
            // in this case we list around this face's faces
            PRE_TYPE::Face_descriptor faceid = *faceIter;  // get iter face id
            if (!faceid.is_valid()) {
                std::cout << "in-valid face:" << faceid << std::endl;
                continue;
            }
            CGAL::Vertex_around_face_circulator<PRE_TYPE::Mesh> vRoundBegin(mesh.halfedge(faceid), mesh), vRoundEnd(vRoundBegin);
            int verticesNum = 0;
            while (1) {
                PRE_TYPE::Vertex_descriptor vd = *vRoundBegin;  // per face vertex
                if (!vd.is_valid() || mesh.is_removed(vd)) {
                    std::cout << "in-valid vert:" << vd << std::endl;
                    break;
                }
                vertexListData.emplace_back(vd.idx()); 
                verticesNum++;
                //std::cout << " " << vd.idx();
                if (++vRoundBegin == vRoundEnd)break;
            }
            if (verticesNum != 0) {
                startIndexData.emplace_back(startIndexData[startIndexData.size() - 1] + verticesNum);
            }
           
        }
        */

        // remove unused prims
        for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
            for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                if (!v.is_valid() || mesh.is_removed(v))
                {
                    mesh.remove_face(f);
                    break;
                }
            }
        }
        if (mesh.has_garbage()) {
            /*
            std::cout << "num of remove face: " << mesh.number_of_removed_faces() << std::endl;
            std::cout << "num of remove vertices: " << mesh.number_of_removed_vertices() << std::endl;
            std::cout << "num of remove hedges: " << mesh.number_of_removed_halfedges() << std::endl;
            std::cout << "num of remove hedges: " << mesh.number_of_removed_edges() << std::endl;

            std::cout << "has garbage\n";*/
            mesh.collect_garbage();
        }

        // ------------------ construct geometry.poly.startIndex attribute -----------------------------
        for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
            int numedges = mesh.degree(f);
            startIndexData.emplace_back(startIndexData[startIndexData.size() - 1] + numedges);
            // face vertices
            for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                vertexListData.push_back(v.idx());
            }
        }


        CARTESIAN_CORE_INFO("set the PolyStartIndexAttr: {0}", polyStartIndexAttName);
        FnAttribute::IntAttribute startIndexAttr(startIndexData.data(), startIndexData.size(), 1);
        iface.setAttr(polyStartIndexAttName, startIndexAttr, false);

        CARTESIAN_CORE_INFO("set the PolyVertexIndexAttr: {0}", polyPolyVertexIndexAttName);
        FnAttribute::IntAttribute vertexListAttr(vertexListData.data(), vertexListData.size(), 1);
        iface.setAttr(polyPolyVertexIndexAttName, vertexListAttr, false);

        // ------------------ construct geometry.point.P attribute -----------------------------
        CARTESIAN_CORE_INFO("construct the geometry.point.P attribute");
        PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, PRE_TYPE::K::Point_3> location = mesh.points();
        std::vector<float> posdata;
        for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices()) {
            if (vd.is_valid()) {
                auto cgalPos = location[vd];
                posdata.emplace_back(cgalPos.x());
                posdata.emplace_back(cgalPos.y());
                posdata.emplace_back(cgalPos.z());
            }
        }
        FnAttribute::FloatAttribute attr(posdata.data(), posdata.size(), 3);
        iface.setAttr(pointPosAttName, attr, false);
 


        int numVertices = posdata.size();
        // point attribute
        for (auto& name : mesh.properties<PRE_TYPE::Vertex_descriptor>())
        {
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> attribMapInt;
            bool found = false;
            boost::tie(attribMapInt, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, int>(name);
            if (found) { // int
                int* rawdata = new int[numVertices];
                for (auto i = 0; i < numVertices; i++) {
                    rawdata[i] = attribMapInt[PRE_TYPE::Vertex_descriptor(i)];
                }
                std::string attribValueName = arbitraryAttName + "." +name + ".value";
                CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "int");
                FnAttribute::IntAttribute attr(rawdata, numVertices, 1);
                iface.setAttr(attribValueName, attr, false);
                delete[]rawdata;
            }


            found = false;
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, float> attribMapFloat;
            boost::tie(attribMapFloat, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, float>(name);
            if (found) { // float
                float* rawdata = new float[numVertices];
                for (auto i = 0; i < numVertices; i++) {
                    rawdata[i] = attribMapFloat[PRE_TYPE::Vertex_descriptor(i)];
                }
                std::string attribValueName = arbitraryAttName + "." + name + ".value";
                CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "float");
                FnAttribute::FloatAttribute attr(rawdata, numVertices, 1);
                iface.setAttr(attribValueName, attr, false);
                delete[]rawdata;
            }

            found = false;
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, double> attribMapDouble;
            boost::tie(attribMapDouble, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, double>(name);
            if (found) { // double
                double* rawdata = new double[numVertices];
                for (auto i = 0; i < numVertices; i++) {
                    rawdata[i] = attribMapDouble[PRE_TYPE::Vertex_descriptor(i)];
                }
                std::string attribValueName = arbitraryAttName + "." + name + ".value";
                CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "double");
                FnAttribute::DoubleAttribute attr(rawdata, numVertices, 1);
                iface.setAttr(attribValueName, attr, false);
                delete[]rawdata;
            }

            found = false;
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, std::string> attribMapString;
            boost::tie(attribMapString, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, std::string>(name);
            if (found) { // string
                std::string* rawdata = new std::string[numVertices];
                for (auto i = 0; i < numVertices; i++) {
                    rawdata[i] = attribMapString[PRE_TYPE::Vertex_descriptor(i)];
                }
                std::string attribValueName = arbitraryAttName + "." + name + ".value";
                CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "string");
                FnAttribute::StringAttribute attr(rawdata, numVertices, 1);
                iface.setAttr(attribValueName, attr, false);
                delete[]rawdata;
            }


            found = false;
            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, glm::vec3> Vec3AttribMap;
            boost::tie(Vec3AttribMap, found) = mesh.property_map<PRE_TYPE::Vertex_descriptor, glm::vec3>(name);
            if (found) { // vec3
                float* rawdata = new float[numVertices * 3];
                for (auto i = 0; i < numVertices; i++) {
                    glm::vec3 value = Vec3AttribMap[PRE_TYPE::Vertex_descriptor(i)];
                    rawdata[i * 3 + 0] = value.x;
                    rawdata[i * 3 + 1] = value.y;
                    rawdata[i * 3 + 2] = value.z;
                }
                std::string attribValueName = arbitraryAttName + "." + name + ".value";
                CARTESIAN_CORE_INFO("set attribute for katana: {0}, type: {1}", attribValueName, "float vector");
                FnAttribute::FloatAttribute attr(rawdata, numVertices, 1);
                iface.setAttr(attribValueName, attr, false);
                delete[]rawdata;
            }
        }



    }// end of surface mesh to katana

}