#include "BindKatanaData.h"

namespace Cartesian {



    void BindKatanaFunction::bind(Foundry::Katana::GeolibCookInterface& interface, const std::shared_ptr<sol::state>& lua) {



        // ------------------------------- get/setType() --------------------------------------------------------------------------------
        auto setTypeFunc = [&interface](const std::string& set)->void{interface.setAttr("type", FnAttribute::StringAttribute(set)); };
        lua->set_function("settype", setTypeFunc);

        // gettype
        auto getTypeFunc = [&interface]()
        {
            FnAttribute::StringAttribute att = interface.getAttr("type");
            return att.getValue();
        };
        // overload gettype
        auto getTypeFunc2 = [&interface](const std::string& loc, const int& index){
            interface.prefetch(loc,index);
            FnAttribute::StringAttribute type =  interface.getAttr("type", loc, index);
            return type.getValue();

        };

        lua->set_function("gettype", sol::overload(getTypeFunc, getTypeFunc2));

        // return op type
        lua->set_function("getoptype", [&interface](){return interface.getOpType(); } );
       
        lua->set_function("getinputname", [&interface](){return interface.getInputName(); });
        lua->set_function("getinputlocationpath", [&interface](){return interface.getInputLocationPath(); });
        lua->set_function("getouputlocationpath", [&interface](){return interface.getOutputLocationPath(); });
        // what num node has 
        
        lua->set_function("ninputs", [&interface]() {return interface.getNumInputs(); } );
        



        // -------------------------------- createChild() ------------------------------------------------
        // createChild("name")
        // createChild("name","type")
        std::function<void(const std::string&)> createChild1 = [&interface](const std::string& locName) -> void {
            interface.createChild(locName);
        };

        std::function<void(const std::string&, const std::string&)> createChild2 = [&interface](const std::string& locName, const std::string& type = "group") ->void {
            interface.createChild(locName, type);
        };

        (*lua)["createchild"] = sol::overload(
            createChild1, createChild2
        );
        // -------------------------------- createChild() ------------------------------------------------


        // -------------------------------- delete location () ------------------------------------------------
        // delete all children
        lua->set_function("deletechildren", [&interface](){interface.deleteChildren(); });
        // delete one child
        lua->set_function("deletechild", [&interface](const std::string& name){interface.deleteChild(name); });
        lua->set_function("deleteself", [&interface](const std::string& name){interface.deleteSelf(); });
        // -------------------------------- delete location () ------------------------------------------------




        // createLocation("/obj/render/")
        auto createLocationFunc = [&interface](const std::string& locpath)->void{
            Foundry::Katana::CreateLocationInfo info;
            Foundry::Katana::CreateLocation(info, interface, locpath);
        };
        lua->set_function("createlocation", createLocationFunc);



        // Get Attrib
        // pointattrib()


        // stop eval child location()
        lua->set_function("stop", [&interface](){interface.stopChildTraversal(); });





    }

}