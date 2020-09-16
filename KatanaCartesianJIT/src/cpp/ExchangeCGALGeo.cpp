#include "ExchangeCGALGeo.h"

#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CGAL/Polygon_mesh_processing/bbox.h>


namespace Cartesian {

    int getTupleSize(FnAttribute::Attribute& attrib, const int baseType) {
        if (baseType == 1) // int
        {
            FnAttribute::IntAttribute rhs = static_cast<FnAttribute::IntAttribute> (attrib);
            return rhs.getTupleSize();
        }
        if (baseType == 2) // float
        {
            FnAttribute::FloatAttribute rhs = static_cast<FnAttribute::FloatAttribute> (attrib);
            return rhs.getTupleSize();
        }
        if (baseType == 3) // double
        {
            FnAttribute::DoubleAttribute rhs = static_cast<FnAttribute::DoubleAttribute> (attrib);
            return rhs.getTupleSize();
        }
        if (baseType == 4) // string
        {
            FnAttribute::StringAttribute rhs = static_cast<FnAttribute::StringAttribute> (attrib);
            return rhs.getTupleSize();
        }

        return -1;
    }

    std::string getBaseTypeAsString(const int baseType) {
        if (baseType == 1) // int
            return "int";
        if (baseType == 2) // float
            return "float";
        if (baseType == 3) // double
            return "double";
        if (baseType == 4) // string
            return "string";
        return "ERROR BASE TYPE";
    }

    void CreateAndBuildCGAL_vertexnum(PRE_TYPE::Mesh& mesh) {
        CARTESIAN_CORE_INFO("Build CGAL_vertexnum attribute");
        PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> vertexnumAttrib;
        bool createdVertexNumAttrib = false;
        boost::tie(vertexnumAttrib, createdVertexNumAttrib) = mesh.add_property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum", -1);
        if (!createdVertexNumAttrib) CARTESIAN_CORE_ERROR("can not create attribute:CGAL_vertexnum");

        int curlVertexNum = 0;
        for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
            auto roundRange = CGAL::vertices_around_face(mesh.halfedge(f), mesh);
            for (PRE_TYPE::Vertex_descriptor v : roundRange) {
                if (vertexnumAttrib[v] == -1)
                {
                    vertexnumAttrib[v] = curlVertexNum;
                    curlVertexNum++;
                }
            }
        }
    }
    void ReBuildCGAL_vertexnum(PRE_TYPE::Mesh& mesh) {
        CARTESIAN_CORE_INFO("rebuild CGAL_vertexnum attribute");
        PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> vertexnumAttrib;
        bool foundVertexNumAttrib = false;
        boost::tie(vertexnumAttrib, foundVertexNumAttrib) = mesh.property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum");
        if (foundVertexNumAttrib) {
            // set to -1
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    vertexnumAttrib[v] = -1;
                }
            }

            int curlVertexNum = 0;
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                // face vertices
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    if (vertexnumAttrib[v] == -1)
                        vertexnumAttrib[v] = curlVertexNum;
                    curlVertexNum++;
                }
            }
        }
        else CARTESIAN_CORE_ERROR("rebuild vertex order error, can not find the attribute: {0}", "CGAL_vertexnum");
    }

    void TryToBuildCGAL_vertexnum(PRE_TYPE::Mesh& mesh) {
        CARTESIAN_CORE_INFO("Try to Build CGAL_vertexnum attribute");
        PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> trytofind_VertexnumAttribMap;
        bool foundVertexNumAttrib = false;
        boost::tie(trytofind_VertexnumAttribMap, foundVertexNumAttrib) = mesh.property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum");
        if (!foundVertexNumAttrib) { // if not ready the vertex num attribmap

            PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> rebulid_vertexnumAttrib; // Rebuild attribute
            bool createdVertexNumAttrib = false;
            boost::tie(rebulid_vertexnumAttrib, createdVertexNumAttrib) = mesh.add_property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum", -1); // default give the -1
            if (!createdVertexNumAttrib) CARTESIAN_CORE_ERROR("can not rebuild attribute:CGAL_vertexnum {0},{1}", __LINE__, __FUNCTION__);
            int curlVertexNum = 0;
            for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
                for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                    if (rebulid_vertexnumAttrib[v] == -1)
                    {
                        rebulid_vertexnumAttrib[v] = curlVertexNum;  // change the order
                        curlVertexNum++;
                    }
                }
            }

        }
    }



    void BuildSurfaceMeshFromKatana(PRE_TYPE::Mesh& meshToBuild,
        Foundry::Katana::GeolibCookInterface& iface,
        const std::string& location,
        const int& index)
    {

        const std::string polyStartIndexAttName = "geometry.poly.startIndex";
        const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
        const std::string pointPosAttName = "geometry.point.P";
        const std::string arbitraryAttName = "geometry.arbitrary";
        const std::string vertexNAttName = "geometry.vertex.N";

        auto time = Foundry::Katana::GetCurrentTime(iface);

        // how many num of per face;
        bool haveFace = true;
        FnAttribute::IntAttribute faceVerticesNumArrayAttrib;

        if (index == -1)
        {
            faceVerticesNumArrayAttrib = iface.getAttr(polyStartIndexAttName);
        }
        else {

            faceVerticesNumArrayAttrib = iface.getAttr(polyStartIndexAttName, location, index);

        }

        if (!faceVerticesNumArrayAttrib.isValid()) {
            //CARTESIAN_CORE_ERROR("can not find attribute: {0}", polyStartIndexAttName);
            haveFace = false;
        }

        auto faceVerticesNumArray = faceVerticesNumArrayAttrib.getNearestSample(time); // first = 0
        // this will construct vertexlist order of the geo;
        FnAttribute::IntAttribute indicesArrayAttrib;
        if (index == -1)
            indicesArrayAttrib = iface.getAttr(polyPolyVertexIndexAttName);
        else
            indicesArrayAttrib = iface.getAttr(polyPolyVertexIndexAttName, location, index);

        if (!indicesArrayAttrib.isValid()) {
            haveFace = false;
            //CARTESIAN_CORE_ERROR("can not find attribute: {0}", polyPolyVertexIndexAttName);
        }
        auto indicesArray = indicesArrayAttrib.getNearestSample(time);


        // how many points of this geometry
        FnAttribute::FloatAttribute positionArrayAttrib;
        if (index == -1)
            positionArrayAttrib = iface.getAttr(pointPosAttName);
        else
        {
            positionArrayAttrib = iface.getAttr(pointPosAttName, location, index);
        }

        if (!positionArrayAttrib.isValid()) {
            CARTESIAN_CORE_ERROR("Can not find attribute: {0}", pointPosAttName);
            return;
        }

        std::vector<AttribMap> katanaAttribMaps;
        FnAttribute::GroupAttribute arbitraryAttrib;
        if (index == -1)
            arbitraryAttrib = iface.getAttr(arbitraryAttName);
        else
            arbitraryAttrib = iface.getAttr(arbitraryAttName, location, index);

        int numOfArbitraryAttribs = arbitraryAttrib.getNumberOfChildren();   // count of arbitrary attribute;
        if (arbitraryAttrib.isValid()) {
            for (int i = 0; i < numOfArbitraryAttribs; i++) {

                // get current ATTRIB_NAME of geometry.arbitrary.ATTRIB_NAME 
                std::string attribName = arbitraryAttrib.getChildName(i);

                // just pass the Texture attribute, because the "geometry.arbitrary.st" same as Texture attribute;
                if (attribName == "Texture") continue;


                FnAttribute::GroupAttribute attrib = arbitraryAttrib.getChildByIndex(i);  // emp: geometry.arbitrary.Cd/st/other*

                FnAttribute::StringAttribute scopeAttrib = attrib.getChildByName("scope"); // geometry.arbitrary.Cd.scope
                if (!scopeAttrib.isValid()) {
                    std::string erroratt = arbitraryAttName + attribName + ".scope";
                    CARTESIAN_CORE_ERROR("can not find the scope attribute :{0}", erroratt);
                    continue;
                }
                std::string scope = scopeAttrib.getValue();                                // point or face 

                FnAttribute::Attribute valueAttrib = attrib.getChildByName("value"); // geometry.arbitrary.Cd.value
                if (!valueAttrib.isValid()) {
                    std::string erroratt = arbitraryAttName + "." + attribName + ".value";
                    CARTESIAN_CORE_WARN("can not find the value attribute: {0}, may be texture or uv attribute, cartesian will try to as uv method", erroratt);
                }
                else {
                    int valueType = valueAttrib.getType();
                    int tupleSize = getTupleSize(valueAttrib, valueType);
                    //CARTESIAN_CORE_INFO("gen attribute map: {0}, type: {1}, tuple size: {2}, scope: {3}", attribName, "int", tupleSize, scope);
                    MAKE_ATTRIB_MAP(attribName, scope, valueType, valueAttrib, tupleSize);
                    katanaAttribMaps.emplace_back(ATTRIB_MAP);
                }


                // try to parse this: geometry.arbitrary.Cd.st
                FnAttribute::IntAttribute indexAttrib = attrib.getChildByName("index"); // geometry.arbitrary.st.value  // int 
                FnAttribute::Attribute indexedValueAttrib = attrib.getChildByName("indexedValue"); // geometry.arbitrary.st.indexedValue
                if (!indexAttrib.isValid() && !indexedValueAttrib.isValid()) {
                    continue;
                }
                else {

                    // struct for our 
                    AttribMap indexMap;                                                                      // create a indexmap
                    indexMap.name = attribName;
                    indexMap.isIndexed = true;                                                               // set this map is index
                    indexMap.type = indexedValueAttrib.getType();                                            // check the indexedValue type
                    indexMap.indexHandle = indexAttrib;                                                      // set index
                    indexMap.indexedValueHandle = indexedValueAttrib;                                        // set indexedValue
                    indexMap.tupleSize = getTupleSize(indexedValueAttrib, indexMap.type);                    // very import for indexed value, only this item can distinguish what type(simple/vector2/vector3/vector4)
                    indexMap.scope = scope;
                    katanaAttribMaps.emplace_back(indexMap);
                    CARTESIAN_CORE_INFO("gen index&indexed value map for attribute: {0}, value type: {1}, tuple size: {2}, scope: {3}", attribName, getBaseTypeAsString(indexMap.type), indexMap.tupleSize, indexMap.scope);
                }
            }
        }

        //CARTESIAN_CORE_INFO("success parsing attribute num: {0}", katanaAttribMaps.size());


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
        if (haveFace) {

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
        }
        // ------------------- Create Geometry Face ----------------------------------




        // 
        // final we create the vertex order, THIS ORDER like houdini vertex, not houdini point. here for katana, so the order should match the katana order.
        // save this order to CGAL Vertex property map

        // if this figure is point number order(4 face, 9 points)
        //      0---1---2 
        //      |   |   |
        //      3---4---5
        //      |   |   |
        //      6---7---8

        //  this figure is vertex number order in katana (order for per face)
        //      0---1---4 
        //      |   |   |
        //      3---2---5
        //      |   |   |
        //      6---7---8
        //  in katana index attribute will be:    0 1 2 3     1 4 5 2     3 2 7 6   2 5 7 8
        // store it cgal int attribute, katana will repr in geometry.arbitrary.CGAL_vertexnum.value
        if(haveFace)
            CreateAndBuildCGAL_vertexnum(meshToBuild);

        PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, int> vertexnumAttrib;
        bool found_vertexnum_attrib;
        boost::tie(vertexnumAttrib, found_vertexnum_attrib) = meshToBuild.property_map<PRE_TYPE::Vertex_descriptor, int>("CGAL_vertexnum");


        // only build st attribute
        if (haveFace) {
            for (auto& attribmap : katanaAttribMaps) {
                if (!attribmap.isIndexed) continue;
                if (attribmap.name != "st") continue;

                //meshToBuild.katana_stIndexValues.clear();
                //meshToBuild.katana_stIndexedValues.clear();

                FnAttribute::IntAttribute indexAttr = static_cast<FnAttribute::IntAttribute> (attribmap.indexHandle);
                auto sampleIndexData = indexAttr.getNearestSample(time);
                for (auto& v : sampleIndexData)
                    meshToBuild.katana_stIndexValues.emplace_back(v);


                FnAttribute::FloatAttribute indexedValueAttr = static_cast<FnAttribute::FloatAttribute> (attribmap.indexedValueHandle);
                auto sampleIndexedValueData = indexedValueAttr.getNearestSample(time);
                for (auto& v : sampleIndexedValueData) {
                    meshToBuild.katana_stIndexedValues.emplace_back(v);
                }


            }
        }
       

        /*
        // only process the indexedValue attribute, we still store it in CGAL vertex attribute
        for (auto& attribmap : katanaAttribMaps) {
            if (!attribmap.isIndexed)continue;
            if (attribmap.tupleSize == 1) { // simple attribute
                if (attribmap.type == 1) { // simple int
                    IndexedValueHelper<FnAttribute::IntAttribute, IndexedValueType<int>>::createAndSet_simpleCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 2) { // simple float
                    IndexedValueHelper<FnAttribute::FloatAttribute, IndexedValueType<float>>::createAndSet_simpleCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 3) { // simple double
                    IndexedValueHelper<FnAttribute::DoubleAttribute, IndexedValueType<double>>::createAndSet_simpleCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 4) { // simple string
                    IndexedValueHelper<FnAttribute::StringAttribute, IndexedValueType<std::string>>::createAndSet_simpleCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }

            }
            if (attribmap.tupleSize == 2) { // vector2 attribute, no matter what type(int/float/double) ,  do not support vector string
                if (attribmap.type == 4) continue;
                if(attribmap.type == 1) {
                    IndexedValueHelper<FnAttribute::IntAttribute, IndexedValueType<glm::vec2>>::createAndSet_Vector2CGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 2) {
                    IndexedValueHelper<FnAttribute::FloatAttribute, IndexedValueType<glm::vec2>>::createAndSet_Vector2CGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 3) {
                    IndexedValueHelper<FnAttribute::DoubleAttribute, IndexedValueType<glm::vec2>>::createAndSet_Vector2CGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
            }
            if (attribmap.tupleSize == 3) { // vector attribute, no matter what type(int/float/double) ,  do not support vector string
                if (attribmap.type == 4) continue;
                if (attribmap.type == 1) {
                    IndexedValueHelper<FnAttribute::IntAttribute, IndexedValueType<glm::vec3>>::createAndSet_VectorCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 2) {
                    IndexedValueHelper<FnAttribute::FloatAttribute, IndexedValueType<glm::vec3>>::createAndSet_VectorCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 3) {
                    IndexedValueHelper<FnAttribute::DoubleAttribute, IndexedValueType<glm::vec3>>::createAndSet_VectorCGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }

            }
            if (attribmap.tupleSize == 4) { // vector4 attribute, no matter what type(int/float/double) ,  do not support vector string
                if (attribmap.type == 4) continue;
                if (attribmap.type == 1) {
                    IndexedValueHelper<FnAttribute::IntAttribute, IndexedValueType<glm::vec4>>::createAndSet_Vector4CGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 2) {
                    IndexedValueHelper<FnAttribute::FloatAttribute, IndexedValueType<glm::vec4>>::createAndSet_Vector4CGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }
                if (attribmap.type == 3) {
                    IndexedValueHelper<FnAttribute::DoubleAttribute, IndexedValueType<glm::vec4>>::createAndSet_Vector4CGALIndexedValue(meshToBuild, vertexnumAttrib, attribmap, time);
                }

            }
        }
   */



   // attribute building: create attribute, except the indexedValue attribute
        for (auto& attribmap : katanaAttribMaps) {
            if (attribmap.isIndexed) continue;

            if (attribmap.scope == "point")
            {
                switch (attribmap.type)
                {
                case 1:  // ----------------------------- int for point ---------------------------------------------------
                {
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Vertex_descriptor>::createAndSet_INT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 2: // ----------------------------- float for point ---------------------------------------------------
                {
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Vertex_descriptor>::createAndSet_FLT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 3: // ----------------------------- double for point ---------------------------------------------------
                {
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Vertex_descriptor>::createAndSet_DOUBLE_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 4: { // ----------------------------- string for point ---------------------------------------------------
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Vertex_descriptor>::createAndSet_STR_CGALAttrib(meshToBuild, attribmap, time);
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
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Face_descriptor>::createAndSet_INT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 2: // ----------------------------- float for face ---------------------------------------------------
                {
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Face_descriptor>::createAndSet_FLT_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 3: // ----------------------------- double for face ---------------------------------------------------
                {
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Face_descriptor>::createAndSet_DOUBLE_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }
                case 4: { // ----------------------------- string for face ---------------------------------------------------
                    AttributeFromKatanaGetSetToCGAL<PRE_TYPE::Face_descriptor>::createAndSet_STR_CGALAttrib(meshToBuild, attribmap, time);
                    break;
                }

                default:
                    break;
                }
            }
        } // end of create attribute;


        // in the last, we build the VertexN
        if(haveFace)
            AttributeVertexNFromKatanaToGCGAL::build(meshToBuild, iface, location, index, time);

        //CARTESIAN_CORE_INFO("geometry build end");

    } // end of build surface mesh

    void SurfaceMeshToKatana(PRE_TYPE::Mesh& mesh, bool buildFace, Foundry::Katana::GeolibCookInterface& iface)
    {



        // clear katana current attribute
        iface.deleteAttrs();
        // set to polymesh
        if(buildFace)
            iface.setAttr("type", FnAttribute::StringAttribute("polymesh"), false);
        else
            iface.setAttr("type", FnAttribute::StringAttribute("pointcloud"), false);

        const std::string polyStartIndexAttName = "geometry.poly.startIndex";
        const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
        const std::string pointPosAttName = "geometry.point.P";
        const std::string arbitraryAttName = "geometry.arbitrary";





        // ------------------ construct geometry.poly.startIndex attribute -----------------------------
        // this actually is likely primpoints()
        std::vector <int> startIndexData;
        std::vector <int> vertexListData;
        startIndexData.push_back(0);


        // remove unused prims
        for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
            for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                if (!v.is_valid() || mesh.is_removed(v)) {
                    mesh.remove_face(f);
                    break;
                }
            }
        }

        if (mesh.has_garbage()) {

            CARTESIAN_CORE_INFO("num of remove face: {0} ", mesh.number_of_removed_faces());
            CARTESIAN_CORE_INFO("num of remove vertices: {0} ", mesh.number_of_removed_vertices());
            CARTESIAN_CORE_INFO("num of remove hedges: {0} ", mesh.number_of_removed_halfedges());
            CARTESIAN_CORE_INFO("num of remove hedges: {0} ", mesh.number_of_removed_edges());

            std::cout << "has garbage\n";
            mesh.collect_garbage();


            //------------------ Rebuild the vertex order, may be use removepoint() removeprim() function-------------------------------------
            ReBuildCGAL_vertexnum(mesh);
        }

        // ------------------ If read mesh from disk , the mesh have not ready for CGAL_vertexnum attribute
        // need try to find, if not, rebuild the CGAL_vertexnum attrib
        if(buildFace)
            TryToBuildCGAL_vertexnum(mesh);



        // ------------------ construct geometry.poly.startIndex attribute -----------------------------
        for (PRE_TYPE::Face_descriptor& f : mesh.faces()) {
            int numedges = mesh.degree(f);
            startIndexData.emplace_back(startIndexData[startIndexData.size() - 1] + numedges);
            // face vertices
            for (PRE_TYPE::Vertex_descriptor v : CGAL::vertices_around_face(mesh.halfedge(f), mesh)) {
                vertexListData.push_back(v.idx());
            }
        }

        if (buildFace) {
            CARTESIAN_CORE_INFO("set the PolyStartIndexAttr: {0}", polyStartIndexAttName);
            FnAttribute::IntAttribute startIndexAttr(startIndexData.data(), startIndexData.size(), 1);
            iface.setAttr(polyStartIndexAttName, startIndexAttr, false);
            CARTESIAN_CORE_INFO("set the PolyVertexIndexAttr: {0}", polyPolyVertexIndexAttName);
            FnAttribute::IntAttribute vertexListAttr(vertexListData.data(), vertexListData.size(), 1);
            iface.setAttr(polyPolyVertexIndexAttName, vertexListAttr, false);
        }

        // ------------------ construct geometry.point.P attribute -----------------------------
        CARTESIAN_CORE_INFO("construct the geometry.point.P attribute");
        PRE_TYPE::Mesh::Property_map<PRE_TYPE::Vertex_descriptor, PRE_TYPE::K::Point_3> location = mesh.points();
        std::vector<float> posdata;
        for (PRE_TYPE::Vertex_descriptor vd : mesh.vertices()) {
            if (vd.is_valid()) {
                auto& cgalPos = location[vd];
                posdata.emplace_back(cgalPos.x());
                posdata.emplace_back(cgalPos.y());
                posdata.emplace_back(cgalPos.z());
            }
        }
        FnAttribute::FloatAttribute attr(posdata.data(), posdata.size(), 3);
        iface.setAttr(pointPosAttName, attr, false);







        // restore the st attribute
        if (buildFace) {
            if (mesh.katana_stIndexedValues.empty() || mesh.katana_stIndexValues.empty()) return;
            IndexedAttribToKatana::buildScopeKatanaAttrib("st", iface);
            IndexedAttribToKatana::buildInputTypeKatanaAttrib("st", "vector2", iface);
            // build the "geometry.arbitrary.st.index"
            std::string stIndexAttribName = arbitraryAttName + "." + "st" + ".index";
            FnAttribute::IntAttribute indexAttr(mesh.katana_stIndexValues.data(), mesh.katana_stIndexValues.size(), 1);
            iface.setAttr(stIndexAttribName, indexAttr, false);

            // build the "geometry.arbitrary.st.indexedValue"
            std::string stIndexedValueAttribName = arbitraryAttName + "." + "st" + ".indexedValue";
            FnAttribute::FloatAttribute indexedValueAttr(mesh.katana_stIndexedValues.data(), mesh.katana_stIndexedValues.size(), 2);
            iface.setAttr(stIndexedValueAttribName, indexedValueAttr, false);
        }

        // for point attribute
        CARTESIAN_CORE_TRACE("---SET POINT ATTRIBS---");
        AttributeToKatana<PRE_TYPE::Vertex_descriptor>::setvalues(mesh, iface);
        // for face attribute
        if (buildFace) {
            CARTESIAN_CORE_TRACE("---SET FACE ATTRIBS---");
            AttributeToKatana<PRE_TYPE::Face_descriptor>::setvalues(mesh, iface);
        }

        // geometry.vertex.N
        if (buildFace) {
            CARTESIAN_CORE_TRACE("---SET N ATTRIBS---");
            AttributeVertexNFromGCGALToKatana::setvalues(mesh, iface);
        }

        // geometry bound
        std::vector<double> boundValues; // xmin xmax ymin ymax zmin zmax
        auto bbox = CGAL::Polygon_mesh_processing::bbox(mesh);
        boundValues.emplace_back(bbox.xmin());
        boundValues.emplace_back(bbox.xmax());
        boundValues.emplace_back(bbox.ymin());
        boundValues.emplace_back(bbox.ymax());
        boundValues.emplace_back(bbox.zmin());
        boundValues.emplace_back(bbox.zmax());

        FnAttribute::DoubleAttribute boundAttr(boundValues.data(), boundValues.size(), 2);
        iface.setAttr("bound", boundAttr, false);

    }// end of surface mesh to katana

}