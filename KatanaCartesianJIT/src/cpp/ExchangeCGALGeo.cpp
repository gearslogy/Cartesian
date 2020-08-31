#include "ExchangeCGALGeo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__




namespace Cartesian {

    int getTupleSize(FnAttribute::Attribute& attrib, const int baseType)  {
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

    void BuildSurfaceMeshFromKatana(PRE_TYPE::Mesh& meshToBuild, 
        Foundry::Katana::GeolibCookInterface& iface,
        const std::string& location, 
        const int& index)
    {

        const std::string polyStartIndexAttName = "geometry.poly.startIndex";
        const std::string polyPolyVertexIndexAttName = "geometry.poly.vertexList";
        const std::string pointPosAttName = "geometry.point.P";
        const std::string arbitraryAttName = "geometry.arbitrary";


        auto time = Foundry::Katana::GetCurrentTime(iface);

        // how many num of per face;
        FnAttribute::IntAttribute faceVerticesNumArrayAttrib;
        if (index == -1) 
            faceVerticesNumArrayAttrib = iface.getAttr(polyStartIndexAttName);
        else 
            faceVerticesNumArrayAttrib = iface.getAttr(polyStartIndexAttName,location,index);

        if (!faceVerticesNumArrayAttrib.isValid()) {
            CARTESIAN_CORE_ERROR("Can not find attribute: {0}", polyStartIndexAttName);
            return;
        }
        auto faceVerticesNumArray = faceVerticesNumArrayAttrib.getNearestSample(time); // first = 0


        // this will construct vertexlist order of the geo;
        FnAttribute::IntAttribute indicesArrayAttrib;
        if(index == -1)
            indicesArrayAttrib = iface.getAttr(polyPolyVertexIndexAttName);
        else
            indicesArrayAttrib = iface.getAttr(polyPolyVertexIndexAttName,location,index);

        if (!indicesArrayAttrib.isValid()) {
            CARTESIAN_CORE_ERROR("Can not find attribute: {0}", polyPolyVertexIndexAttName);
            return;
        }
        auto indicesArray = indicesArrayAttrib.getNearestSample(time);


        // how many points of this geometry
        FnAttribute::FloatAttribute positionArrayAttrib;
        if(index == -1)
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
                    std::string erroratt = arbitraryAttName +"." +attribName + ".value";
                    CARTESIAN_CORE_WARN("can not find the value attribute: {0}, may be texture or uv attribute, cartesian will try to as uv method", erroratt);
                }
                else {
                    int valueType = valueAttrib.getType();
                    int tupleSize =getTupleSize(valueAttrib, valueType);
                    CARTESIAN_CORE_INFO("gen attribute map: {0}, type: {1}, tuple size: {2}, scope: {3}", attribName, "int", tupleSize, scope);
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
                    AttribMap indexMap;                                    // create a indexmap
                    indexMap.name = attribName;
                    indexMap.isIndexed = true;                             // set this map is index
                    indexMap.type = indexedValueAttrib.getType();          // check the indexedValue type
                    indexMap.indexHandle = indexAttrib;                    // set index
                    indexMap.indexedValueHandle = indexedValueAttrib;      // set indexedValue
                    indexMap.tupleSize = getTupleSize(indexedValueAttrib, indexMap.type);
                    indexMap.scope = scope;
                    katanaAttribMaps.emplace_back(indexMap);
                    CARTESIAN_CORE_INFO("gen index&indexed value map for attribute: {0}, value type: {1}, tupleSize: {2}, scope: {3}", attribName, getBaseTypeAsString(indexMap.type), indexMap.tupleSize, indexMap.scope);
                }
            }
        }
        CARTESIAN_CORE_INFO("success parsing attribute num: {0}", katanaAttribMaps.size());


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
    } // end of build surface mesh

    void SurfaceMeshToKatana(PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface)
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

            CARTESIAN_CORE_INFO("num of remove face: {0} ", mesh.number_of_removed_faces());
            CARTESIAN_CORE_INFO("num of remove vertices: {0} ", mesh.number_of_removed_vertices());
            CARTESIAN_CORE_INFO("num of remove hedges: {0} ", mesh.number_of_removed_halfedges());
            CARTESIAN_CORE_INFO("num of remove hedges: {0} ", mesh.number_of_removed_edges());

            std::cout << "has garbage\n";
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
                auto& cgalPos = location[vd];
                posdata.emplace_back(cgalPos.x());
                posdata.emplace_back(cgalPos.y());
                posdata.emplace_back(cgalPos.z());
            }
        }
        FnAttribute::FloatAttribute attr(posdata.data(), posdata.size(), 3);
        iface.setAttr(pointPosAttName, attr, false);


        // for point attribute
        AttributeToKatana<PRE_TYPE::Vertex_descriptor>::setvalues(mesh, iface);
        // for face attribute
        AttributeToKatana<PRE_TYPE::Face_descriptor>::setvalues(mesh, iface);

    }// end of surface mesh to katana

}