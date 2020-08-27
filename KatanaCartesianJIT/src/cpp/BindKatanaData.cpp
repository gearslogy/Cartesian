#include "BindKatanaData.h"
#include "CartesianLog.h"


#undef interface
#undef GetCurrentTime
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
        auto setTypeFunc = [&iface](const std::string& set)->void {iface.setAttr("type", FnAttribute::StringAttribute(set)); };
        lua->set_function("settype", setTypeFunc);

      
        // return op type
        lua->set_function("getoptype", [&iface](){return iface.getOpType(); } );
       
        lua->set_function("getinputname", [&iface](){return iface.getInputName(); });
        lua->set_function("getinputlocationpath", [&iface](){return iface.getInputLocationPath(); });
        lua->set_function("getouputlocationpath", [&iface](){return iface.getOutputLocationPath(); });
        // what num node has 
        
        lua->set_function("ninputs", [&iface]() {return iface.getNumInputs(); } );
        

        auto getTypeFunc1 = [&iface]() {
            FnAttribute::StringAttribute att = iface.getAttr("type");
            if (att.isValid())return att.getValue();
            return std::string("");
        };
        auto getTypeFunc2 = [&iface](const std::string& inputLocation, const int& inputIndex) {
            std::string get;
            FnAttribute::StringAttribute att = iface.getAttr("type", inputLocation, inputIndex);
            if (att.isValid()) {
                get = att.getValue("", false);
                return get;
            }
            else {
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
        lua->set_function("deletechildren", [&iface](){iface.deleteChildren(); });
        // delete one child
        lua->set_function("deletechild", [&iface](const std::string& name){iface.deleteChild(name); });
        lua->set_function("deleteself", [&iface](const std::string& name){iface.deleteSelf(); });
        // -------------------------------- delete location () ------------------------------------------------
        
        auto prefetch = [&iface](const std::string& input, const int& index) {
            iface.prefetch(input, index);
        };
        lua->set_function("prefetch", prefetch);


        auto rename = [&iface](const std::string &str, const std::string &dst) {
            //Foundry::Katana::FnGeolibCookInterfaceUtils::cookDaps(iface);
            iface.prefetch();
            Foundry::Katana::RenameChild(iface, str,dst ); 
        };
        lua->set_function("renamechild", rename);

        // createLocation("/obj/render/")
        auto createLocationFunc = [&iface](const std::string& locpath)->void{
            Foundry::Katana::CreateLocationInfo info;
            Foundry::Katana::CreateLocation(info, iface, locpath);
        };
        lua->set_function("createlocation", createLocationFunc);



        // copylocation("tochild", "fromSrc", index)
        auto copyLocation = [&iface](const std::string& childname, const std::string &fromLocation , const int &inputIndex)->void {
            iface.copyLocationToChild(childname, fromLocation, inputIndex);
        };
        lua->set_function("copylocation", copyLocation);


        // stop eval child location()
        lua->set_function("stop", [&iface](){iface.stopChildTraversal(); });

        

        // delete & delete all attrib
        auto delattr = [&iface](const std::string& attrName) {iface.deleteAttr(attrName);};
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
            rxmat = glm::rotate(rxmat, glm::radians(x_rot.getValue() ), glm::vec3(1, 0, 0)); // rot x
            rymat = glm::rotate(rymat, glm::radians(y_rot.getValue() ), glm::vec3(0, 1, 0)); // rot y
            rzmat = glm::rotate(rzmat, glm::radians(z_rot.getValue() ), glm::vec3(0, 0, 1)); // rot z
            //glm::mat4 transform = glm::eulerAngleYXZ( y_rot.getValue(), x_rot.getValue(), z_rot.getValue());
            tmat = glm::translate(tmat, translate);
            mat = tmat * smat * rzmat* rymat * rxmat;
            return mat;
        };
        // get another node location xform attrib
        auto xform_anotherNode = [&iface](const std::string &location, const int &index) {
            glm::mat4 mat(1.0f);

            glm::mat4 smat(1.0f);
            glm::mat4 rxmat(1.0f);
            glm::mat4 rymat(1.0f);
            glm::mat4 rzmat(1.0f);
            glm::mat4 tmat(1.0f);
            FnAttribute::DoubleAttribute trans = iface.getAttr("xform.interactive.translate", location, index);
            FnAttribute::DoubleAttribute rotZ = iface.getAttr("xform.interactive.rotateZ", location, index);
            FnAttribute::DoubleAttribute rotY = iface.getAttr("xform.interactive.rotateY", location, index);
            FnAttribute::DoubleAttribute rotX = iface.getAttr("xform.interactive.rotateX", location, index);
            FnAttribute::DoubleAttribute scale = iface.getAttr("xform.interactive.scale", location, index);

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
                CARTESIAN_CORE_ERROR("can not get transform.scale");
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
        lua->set_function("xform", sol::overload(xform,xform_anotherNode));
        
    }

}