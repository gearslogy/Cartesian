#include "BindKatanaData.h"
#include "CartesianLog.h"
#define SOL_ALL_SAFETIES_ON 0
#define SOL_NO_EXCEPTIONS 1

#undef interface
#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>

#include <FnPluginSystem/FnPlugin.h>

#include <FnGeolib/util/Path.h>

#include <FnGeolib/op/FnGeolibOp.h>
#include <FnGeolibServices/FnGeolibCookInterfaceUtilsService.h>
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



        // Get Attrib
        // pointattrib()


        // stop eval child location()
        lua->set_function("stop", [&iface](){iface.stopChildTraversal(); });





    }

}