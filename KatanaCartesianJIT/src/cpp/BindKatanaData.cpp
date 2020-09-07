#include "BindKatanaData.h"
#include "CartesianLog.h"
#include "ExchangeCGALGeo.h"

#if defined _WIN32 || defined __CYGWIN__
#undef interface
#undef GetCurrentTime
#endif //  __MSVC__



#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>

#include <FnPluginSystem/FnPlugin.h>

#include <FnGeolib/util/Path.h>

#include <FnGeolib/op/FnGeolibOp.h>
#include <FnGeolibServices/FnGeolibCookInterfaceUtilsService.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cassert>





namespace Cartesian {



    void BindKatanaFunction::bind(Foundry::Katana::GeolibCookInterface& iface, const std::shared_ptr<sol::state>& lua) {

  
        // ------------------------------- get/setType() --------------------------------------------------------------------------------
        auto setTypeFunc = [&iface](const std::string& set)->void {
            auto location = iface.getInputLocationPath();
            if (iface.doesLocationExist(location)) {

                iface.setAttr("type", FnAttribute::StringAttribute(set), false);
            }
        };
        lua->set_function("settype", setTypeFunc);


        // return op type
        lua->set_function("getoptype", [&iface]() {return iface.getOpType(); });

        lua->set_function("getinputname", [&iface]() {return iface.getInputName(); });
        lua->set_function("getinputlocationpath", [&iface]() {return iface.getInputLocationPath(); });
        lua->set_function("getouputlocationpath", [&iface]() {return iface.getOutputLocationPath(); });
        // what num node has 

        lua->set_function("ninputs", [&iface]() {return iface.getNumInputs(); });


        auto getTypeFunc1 = [&iface]() {
            FnAttribute::StringAttribute att = iface.getAttr("type");
            if (att.isValid())return att.getValue();
            return std::string("");
        };
        auto getTypeFunc2 = [&iface](const std::string& inputLocation, const int& inputIndex) {
            std::string get;
            try {
                iface.getAttr("type", inputLocation, inputIndex);
            }
            catch (...) {
                CARTESIAN_CORE_WARN("just check gettype() the return value, if existed, ignore this error: runtime_error");
                throw GetAttribException();
            }
            FnAttribute::StringAttribute att = iface.getAttr("type", inputLocation, inputIndex);
            if (att.isValid()) {
                get = att.getValue("", false);
                return get;
            }
            
            return get;
        };
        lua->set_function("gettype", sol::overload(getTypeFunc1, getTypeFunc2));
      

        // -------------------------------- createChild() ------------------------------------------------
        // createChild("name")
        // createChild("name","type")
        std::function<void(const std::string&)> createChild1 = [&iface](const std::string& locName) -> void {
            iface.createChild(locName);
        };

        std::function<void(const std::string&, const std::string&)> createChild2 = [&iface](const std::string& locName, const std::string& type = "group") ->void {
            iface.createChild(locName, type);
        };

        (*lua)["createchild"] = sol::overload(
            createChild1, createChild2
        );
        // -------------------------------- createChild() ------------------------------------------------






        // -------------------------------- delete location () ------------------------------------------------
        // delete all children
        lua->set_function("deletechildren", [&iface]() {iface.deleteChildren(); });
        // delete one child
        lua->set_function("deletechild", [&iface](const std::string& name) {iface.deleteChild(name); });
        lua->set_function("deleteself", [&iface](const std::string& name) {iface.deleteSelf(); });
        // -------------------------------- delete location () ------------------------------------------------

        auto prefetch = [&iface](const std::string& input, const int& index) {
            iface.prefetch(input, index);
        };
        lua->set_function("prefetch", prefetch);


        auto rename = [&iface](const std::string& str, const std::string& dst) {
            //Foundry::Katana::FnGeolibCookInterfaceUtils::cookDaps(iface);
            iface.prefetch();
            Foundry::Katana::RenameChild(iface, str, dst);
        };
        lua->set_function("renamechild", rename);

        // createLocation("/obj/render/")
        auto createLocationFunc = [&iface](const std::string& locpath)->void {
            Foundry::Katana::CreateLocationInfo info;
            Foundry::Katana::CreateLocation(info, iface, locpath);
        };
        lua->set_function("createlocation", createLocationFunc);
        
        



        // copylocation("tochild", "fromSrc", index)
        auto copyLocation = [&iface](const std::string& childname, const std::string& fromLocation, const int& inputIndex)->void {
            iface.copyLocationToChild(childname, fromLocation, inputIndex);
        };
        lua->set_function("copylocation", copyLocation);


        // stop eval child location()
        lua->set_function("stop", [&iface]() {iface.stopChildTraversal(); });



        // delete & delete all attrib
        auto delattr = [&iface](const std::string& attrName) {iface.deleteAttr(attrName); };
        auto deleteall = [&iface]() {iface.deleteAttrs(); };
        lua->set_function("delattrib", delattr);
        lua->set_function("clearattrib", deleteall);

        // get current node xform attrib, return matrix attrib, SRT 
        auto xform = [&iface]() {


            glm::mat4 mat(1.0f);
            glm::mat4 smat(1.0f);
            glm::mat4 rxmat(1.0f);
            glm::mat4 rymat(1.0f);
            glm::mat4 rzmat(1.0f);
            glm::mat4 tmat(1.0f);
            FnAttribute::GroupAttribute transformation = iface.getOpArg("transform");
            if (!transformation.isValid()) {
                CARTESIAN_CORE_ERROR("can not get group attribute: transform");
                return mat;
            }

            FnAttribute::GroupAttribute trans_find = transformation.getChildByName("translate");
            if (!trans_find.isValid()) {
                CARTESIAN_CORE_ERROR("can not get group attribute: transform.translate");
                return mat;
            }
            FnAttribute::GroupAttribute rot_find = transformation.getChildByName("rotate");
            if (!rot_find.isValid()) {
                CARTESIAN_CORE_ERROR("can not get group attribute: transform.rotate");
                return mat;
            }
            FnAttribute::GroupAttribute scale_find = transformation.getChildByName("scale");
            if (!scale_find.isValid()) {
                CARTESIAN_CORE_ERROR("can not get group attribute: transform.scale");
                return mat;
            }



            FnAttribute::FloatAttribute x_trans = trans_find.getChildByName("x");
            FnAttribute::FloatAttribute y_trans = trans_find.getChildByName("y");
            FnAttribute::FloatAttribute z_trans = trans_find.getChildByName("z");
            assert(x_trans.isValid()); assert(y_trans.isValid()); assert(z_trans.isValid());
            glm::vec3 translate(x_trans.getValue(), y_trans.getValue(), z_trans.getValue());

            FnAttribute::FloatAttribute x_rot = rot_find.getChildByName("x");
            FnAttribute::FloatAttribute y_rot = rot_find.getChildByName("y");
            FnAttribute::FloatAttribute z_rot = rot_find.getChildByName("z");
            assert(x_rot.isValid()); assert(y_rot.isValid()); assert(z_rot.isValid());

            FnAttribute::FloatAttribute x_scale = scale_find.getChildByName("x");
            FnAttribute::FloatAttribute y_scale = scale_find.getChildByName("y");
            FnAttribute::FloatAttribute z_scale = scale_find.getChildByName("z");
            assert(x_scale.isValid()); assert(y_scale.isValid()); assert(z_scale.isValid());
            glm::vec3 scale(x_scale.getValue(), y_scale.getValue(), z_scale.getValue());

            smat = glm::scale(smat, scale);
            rxmat = glm::rotate(rxmat, glm::radians(x_rot.getValue()), glm::vec3(1, 0, 0)); // rot x
            rymat = glm::rotate(rymat, glm::radians(y_rot.getValue()), glm::vec3(0, 1, 0)); // rot y
            rzmat = glm::rotate(rzmat, glm::radians(z_rot.getValue()), glm::vec3(0, 0, 1)); // rot z
            //glm::mat4 transform = glm::eulerAngleYXZ( y_rot.getValue(), x_rot.getValue(), z_rot.getValue());
            tmat = glm::translate(tmat, translate);
            mat = tmat * smat * rzmat * rymat * rxmat;
            return mat;
        };
        // get another node location xform attrib
        auto xform_anotherNode = [&iface](const std::string& location, const int& index) {
            //iface.replaceChildren("/root", index);

            glm::mat4 mat(1.0f);

            glm::mat4 smat(1.0f);
            glm::mat4 rxmat(1.0f);
            glm::mat4 rymat(1.0f);
            glm::mat4 rzmat(1.0f);
            glm::mat4 tmat(1.0f);

            try {
                iface.getAttr("xform.interactive.translate", location);
                iface.getAttr("xform.interactive.rotateZ", location);
                iface.getAttr("xform.interactive.rotateY", location);
                iface.getAttr("xform.interactive.rotateX", location);
                iface.getAttr("xform.interactive.scale", location);
            }
            catch (...) {
                CARTESIAN_CORE_WARN("just check getxform() the return value, if existed, ignore this error: runtime_error");
                throw GetAttribException();
            }
            FnAttribute::DoubleAttribute trans = iface.getAttr("xform.interactive.translate", location);
            FnAttribute::DoubleAttribute rotZ = iface.getAttr("xform.interactive.rotateZ", location);
            FnAttribute::DoubleAttribute rotY = iface.getAttr("xform.interactive.rotateY", location);
            FnAttribute::DoubleAttribute rotX = iface.getAttr("xform.interactive.rotateX", location);
            FnAttribute::DoubleAttribute scale = iface.getAttr("xform.interactive.scale", location);

            if (!trans.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.translate for: {0}", location);
                return mat;
            }
            if (!rotZ.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.rotateZ for: {0}", location);
                return mat;
            }
            if (!rotY.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.rotateY for: {0}", location);
                return mat;
            }
            if (!rotX.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.rotateX for: {0}", location);
                return mat;
            }

            if (!scale.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.scale");
                return mat;
            }
            auto time = Foundry::Katana::GetCurrentTime(iface);
            auto sample_trans = trans.getNearestSample(time);
            auto sample_rotZ = rotZ.getNearestSample(time);
            auto sample_rotY = rotY.getNearestSample(time);
            auto sample_rotX = rotX.getNearestSample(time);
            auto sample_scale = scale.getNearestSample(time);

            smat = glm::scale(smat, glm::vec3(sample_scale[0], sample_scale[1], sample_scale[2]));
            rxmat = glm::rotate(rxmat, glm::radians(float(sample_rotX[0])), glm::vec3(1, 0, 0)); // rot x
            rymat = glm::rotate(rymat, glm::radians(float(sample_rotY[0])), glm::vec3(0, 1, 0)); // rot y
            rzmat = glm::rotate(rzmat, glm::radians(float(sample_rotZ[0])), glm::vec3(0, 0, 1)); // rot z
            tmat = glm::translate(tmat, glm::vec3(sample_trans[0], sample_trans[1], sample_trans[2]));

            mat = tmat * smat * rzmat * rymat * rxmat;

            return mat;
        };
        // get another node xform attrib
        lua->set_function("getxform", sol::overload(xform, xform_anotherNode));



        // apply xform
        auto apply_xform = [&iface](const glm::mat4& mat) {
            FnAttribute::DoubleAttribute trans = iface.getAttr("xform.interactive.translate");
            FnAttribute::DoubleAttribute rotZ = iface.getAttr("xform.interactive.rotateZ");
            FnAttribute::DoubleAttribute rotY = iface.getAttr("xform.interactive.rotateY");
            FnAttribute::DoubleAttribute rotX = iface.getAttr("xform.interactive.rotateX");
            FnAttribute::DoubleAttribute scale = iface.getAttr("xform.interactive.scale");

            if (!trans.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.translate");
            }
            if (!rotZ.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.rotateZ");
                return ;
            }
            if (!rotY.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.rotateY");
                return ;
            }
            if (!rotX.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.rotateX");
                return ;
            }

            if (!scale.isValid()) {
                CARTESIAN_CORE_ERROR("can not get xform.interactive.scale");
                return ;
            }


            glm::vec3 dec_scale;
            glm::quat dec_rotation;
            glm::vec3 dec_translation;
            glm::vec3 dec_skew;
            glm::vec4 dec_perspective;
            glm::decompose(mat, dec_scale, dec_rotation, dec_translation, dec_skew, dec_perspective);
            //rotation = glm::conjugate(rotation);
            glm::vec3 euler_rot = glm::eulerAngles(dec_rotation); // rotation return the euler rotation


            double* raw_scale = new double[3];
            double* raw_trans = new double[3];
            double* raw_rotz = new double[4];
            double* raw_roty = new double[4];
            double* raw_rotx = new double[4];

            // raw set dec_scale
            raw_scale[0] = dec_scale.x; raw_scale[1] = dec_scale.y; raw_scale[2] = dec_scale.z;
            // raw set dec_trans
            raw_trans[0] = dec_translation.x; raw_trans[1] = dec_translation.y; raw_trans[2] = dec_translation.z;
            // raw set rotx;
            raw_rotx[0] = euler_rot.x * 180 / glm::pi<double>(); raw_rotx[1] = 1.0; raw_rotx[2] = 0.0; raw_rotx[3] = 0.0;
            // raw set roty;
            raw_roty[0] = euler_rot.y * 180 / glm::pi<double>(); raw_roty[1] = 0.0; raw_roty[2] = 1.0; raw_roty[3] = 0.0;
            // raw set rotz;
            raw_rotz[0] = euler_rot.z * 180 / glm::pi<double>(); raw_rotz[1] = 0.0; raw_rotz[2] = 0.0; raw_rotz[3] = 1.0;

            FnAttribute::DoubleAttribute newTransAttr(raw_trans, 3,
                1);
            FnAttribute::DoubleAttribute newScaleAttr(raw_scale, 3,
                1);
            FnAttribute::DoubleAttribute newXRotAttr(raw_rotx, 4,
                1);
            FnAttribute::DoubleAttribute newYRotAttr(raw_roty, 4,
                1);
            FnAttribute::DoubleAttribute newZRotAttr(raw_rotz, 4,
                1);
            iface.setAttr("xform.interactive.translate", newTransAttr, false);
            iface.setAttr("xform.interactive.scale", newScaleAttr, false);
            iface.setAttr("xform.interactive.rotateX", newXRotAttr, false);
            iface.setAttr("xform.interactive.rotateY", newYRotAttr, false);
            iface.setAttr("xform.interactive.rotateZ", newZRotAttr, false);

            delete[]raw_roty;
            delete[]raw_rotz;
            delete[]raw_rotx;
            delete[]raw_scale;
            delete[]raw_trans;
        };
        lua->set_function("setxform", apply_xform);



        // Geometry exchange
        auto geoself = [&iface](PRE_TYPE::Mesh &mesh) {
            BuildSurfaceMeshFromKatana(mesh, iface);
        };

        auto geoself_fromotherinput = [&iface](PRE_TYPE::Mesh& mesh , const std::string &location, const int &inputindex) {
            try {
                BuildSurfaceMeshFromKatana(mesh, iface, location, inputindex);
            }
            catch (...) {
                CARTESIAN_CORE_WARN("just check inputmesh() , ignore this error: runtime_error");
                throw GetAttribException();
            }
        };
        lua->set_function("inputmesh", sol::overload(geoself,geoself_fromotherinput));


        // attach mesh
        auto meshend = [&iface](PRE_TYPE::Mesh &mesh) {
            SurfaceMeshToKatana(mesh, 1, iface);
        };

        auto pointsend = [&iface](PRE_TYPE::Mesh& mesh) {
            SurfaceMeshToKatana(mesh, 0, iface);
        };

        lua->set_function("attachmesh", meshend);
        lua->set_function("attachpoints", pointsend);


        // for quickly instance
        auto setinstance_source = [&iface](const std::string &path) {
            iface.setAttr("geometry.instanceSource", FnAttribute::StringAttribute(path));
        };
        lua->set_function("setinstance_source", setinstance_source);
        
        // for instance translate

        
        auto set_instance_translate2 = [&iface](float x, float y, float z) {
            std::string name = "xform.group.translate"; // double attribute
            double values[3] = { x, y, z};
            iface.setAttr(name, FnAttribute::DoubleAttribute(values, 3, 1));
        };
        auto set_instance_translate = [&iface](const glm::vec3& trans) {
            std::string name = "xform.group.translate"; // double attribute
            double values[3] = { trans.x, trans.y,trans.z };
            iface.setAttr(name, FnAttribute::DoubleAttribute(values, 3, 1));
        };
        lua->set_function("setinstance_translate", sol::overload(set_instance_translate2, set_instance_translate));

        auto set_instance_rotate = [&iface](const glm::vec3& rot) {
            std::string nameX = "xform.group.rotateX"; // double attribute
            std::string nameY = "xform.group.rotateY"; // double attribute
            std::string nameZ = "xform.group.rotateZ"; // double attribute
            double valuesX[4] = { rot.x, 1, 0, 0 };
            double valuesY[4] = { rot.y, 0, 1, 0 };
            double valuesZ[4] = { rot.z, 0, 0, 1 };
            iface.setAttr(nameX, FnAttribute::DoubleAttribute(valuesX, 4, 1));
            iface.setAttr(nameY, FnAttribute::DoubleAttribute(valuesY, 4, 1));
            iface.setAttr(nameZ, FnAttribute::DoubleAttribute(valuesZ, 4, 1));
        };
        auto set_instance_rotate2 = [&iface](double x, double y, double z) {
            std::string nameX = "xform.group.rotateX"; // double attribute
            std::string nameY = "xform.group.rotateY"; // double attribute
            std::string nameZ = "xform.group.rotateZ"; // double attribute
            double valuesX[4] = { x, 1, 0, 0 };
            double valuesY[4] = { y, 0, 1, 0 };
            double valuesZ[4] = { z, 0, 0, 1 };
            iface.setAttr(nameX, FnAttribute::DoubleAttribute(valuesX, 4, 1));
            iface.setAttr(nameY, FnAttribute::DoubleAttribute(valuesY, 4, 1));
            iface.setAttr(nameZ, FnAttribute::DoubleAttribute(valuesZ, 4, 1));
        };
        lua->set_function("setinstance_rotate", sol::overload(set_instance_rotate,set_instance_rotate2) );

        auto set_instance_scale = [&iface](const glm::vec3& scale) {
            std::string name = "xform.group.scale"; // double attribute
            double values[3] = { scale.x, scale.y,scale.z };
            iface.setAttr(name, FnAttribute::DoubleAttribute(values, 3, 1));
        };
        auto set_instance_scale2 = [&iface](float x, float y, float z) {
            std::string name = "xform.group.scale"; // double attribute
            double values[3] = {x,y,z };
            iface.setAttr(name, FnAttribute::DoubleAttribute(values, 3, 1));
        };
        lua->set_function("setinstance_scale", sol::overload(set_instance_scale, set_instance_scale2));



        // for get bound box, return tables
        auto getbbox = [&iface]() {
            auto time = Foundry::Katana::GetCurrentTime(iface);
            std::vector<double> values;  // minx maxx miny maxy minz maxz
            FnAttribute::DoubleAttribute att =   Foundry::Katana::GetBoundAttr(iface);
            if (!att.isValid()) {
                CARTESIAN_CORE_ERROR("can not get bound attribute");
            }
            else {
                auto samples = att.getNearestSample(time);
                values.emplace_back(samples[0]); // minx 
                values.emplace_back(samples[2]); // miny 
                values.emplace_back(samples[4]); // minz 
                values.emplace_back(samples[1]); // maxx 
                values.emplace_back(samples[3]); // maxy 
                values.emplace_back(samples[5]); // maxz
            }
  
            return sol::as_table(values);
        };
        auto getAnotherbbox = [&iface](const std::string &location, const int index) {
            try {
                Foundry::Katana::GetBoundAttr(iface,location, index);
            }
            catch (...) {
                CARTESIAN_CORE_WARN("just check getboundattr() , ignore this error: runtime_error");
                throw GetAttribException();
            }

            auto time = Foundry::Katana::GetCurrentTime(iface);
            std::vector<double> values;  // minx maxx miny maxy minz maxz
            FnAttribute::DoubleAttribute att = Foundry::Katana::GetBoundAttr(iface,location, index);
            if (!att.isValid()) {
                CARTESIAN_CORE_ERROR("can not get bound attribute");
            }
            for (auto& val : att.getNearestSample(time)) {
                values.emplace_back(val);// minx maxx miny maxy minz maxz
            }
            return sol::as_table(values);
        };
        lua->set_function("getboundattr", sol::overload(getbbox, getAnotherbbox));
        
        
        
        // Direct Get Attribute of current node from katana, only return table
        auto getPointFloatTuple = [&iface](const std::string &attname) {
            auto time = Foundry::Katana::GetCurrentTime(iface);
            std::string PAttName = "geometry.point.P";
            std::string arbitraryAttName = "geometry.arbitrary";
            std::string valueAttribName = arbitraryAttName;
            valueAttribName += ".";
            valueAttribName += attname;
            valueAttribName += ".";
            valueAttribName += "value";

            std::vector<float> values;
            if (attname == "P") {
                FnAttribute::FloatAttribute posAttr = iface.getAttr(PAttName);
                if (posAttr.isValid()) 
                {
                    auto samples = posAttr.getNearestSample(time);
                    for (auto& val : samples) 
                        values.emplace_back(val)
;
                    return sol::as_table(values);
                }
            }
            FnAttribute::FloatAttribute valueAttr = iface.getAttr(valueAttribName);

           
            if(valueAttr.isValid()){
                    auto samples = valueAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);
          
            }
            else {
                CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
            }
            return sol::as_table(values);
        };

        auto getPointFloatTuple2 = [&iface](const std::string& attname, const std::string &inputLocation, const int inputIndex) {
            auto time = Foundry::Katana::GetCurrentTime(iface);
            std::string arbitraryAttName = "geometry.arbitrary";
            std::string valueAttribName = arbitraryAttName;
            std::string PAttName = "geometry.point.P";
            valueAttribName += ".";
            valueAttribName += attname;
            valueAttribName += ".";
            valueAttribName += "value";


            try {
                if(attname == "P")
                    iface.getAttr(PAttName, inputLocation, inputIndex);
                iface.getAttr(valueAttribName,inputLocation, inputIndex);
            }
            catch (...) {
                CARTESIAN_CORE_WARN("just check getfloattuple() , ignore this error: runtime_error");
                throw GetAttribException();
            }

            std::vector<float> values;
            if (attname == "P") {
                FnAttribute::FloatAttribute posAttr = iface.getAttr(PAttName, inputLocation, inputIndex);
                if (posAttr.isValid()) {
                    auto samples = posAttr.getNearestSample(time);
                    for (auto& val : samples) values.emplace_back(val);
                    return sol::as_table(values);
                }
            }


            FnAttribute::FloatAttribute valueAttr = iface.getAttr(valueAttribName, inputLocation, inputIndex);
            if (valueAttr.isValid()) {
                auto samples = valueAttr.getNearestSample(time);
                for (auto& val : samples) values.emplace_back(val);

            }
            else {
                CARTESIAN_CORE_ERROR("query attribute no (value) attribute: {0}", valueAttribName);;
            }
            return sol::as_table(values);
        };

        lua->set_function("getfloattuple", sol::overload(getPointFloatTuple, getPointFloatTuple2));




        // setboundattr()
        // set bounding box for current location
        // table : {minx,miny,minz,maxx,maxy,maxz}
        auto setbbox = [&iface](const sol::lua_table& table) {
            if (table.size() != 6) {
                CARTESIAN_CORE_ERROR("set bounding box attribute for katana , table need six elements");
                return;
            }
            double values[6];
            values[0] = table.get<double>(1);  // minx
            values[1] = table.get<double>(4);  // maxx

            values[2] = table.get<double>(2);  // miny
            values[3] = table.get<double>(5);  // maxy

            values[4] = table.get<double>(3);  // minz
            values[5] = table.get<double>(6);  // maxz

            FnAttribute::DoubleAttribute boundAttr(values, 6, 2);
            iface.setAttr("bound", boundAttr, false);

        };
        auto setbbox2 = [&iface](const glm::vec3& bbmin, const glm::vec3& bbmax) {

            double values[6];
            values[0] = bbmin.x;  // minx
            values[1] = bbmax.x;  // maxx

            values[2] = bbmin.y;  // miny
            values[3] = bbmax.y;  // maxy

            values[4] = bbmin.z;  // minz
            values[5] = bbmax.z;  // maxz

            FnAttribute::DoubleAttribute boundAttr(values, 6, 2);
            iface.setAttr("bound", boundAttr, false);

        };
        lua->set_function("setboundattr", sol::overload(setbbox, setbbox2) );

        
        
       
    }

}