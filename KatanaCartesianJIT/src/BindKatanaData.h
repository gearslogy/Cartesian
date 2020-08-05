//
// Created by admin on 2020/8/4.
//

#ifndef BUILDINGCTSLUAJIT_BINDKATANADATA_H
#define BUILDINGCTSLUAJIT_BINDKATANADATA_H
namespace Cartesian{

    /*
     *  This function should not set to lua statck per cook !
     *
     */

    class BindKatanaFunction{
    public:
        static void bind(Foundry::Katana::GeolibCookInterface &interface,  const std::shared_ptr<sol::state>& lua){

            // ------------------------------- get setType() --------------------------------------------------------------------------------
            auto setTypeFunc = [&interface](const std::string &set)->void{interface.setAttr("type",FnAttribute::StringAttribute(set));};
            lua->set_function("setType",setTypeFunc);

            auto getTypeFunc = [&interface]()
            {
                FnAttribute::StringAttribute att= interface.getAttr("type");
                return att.getValue();
            };
            lua->set_function("getType", getTypeFunc);
            // ------------------------------- get setType() --------------------------------------------------------------------------------




            // -------------------------------- createChild() ------------------------------------------------
            // createChild("name")
            // createChild("name","type")
            std::function<void(const std::string &)> createChild1 = [&interface](const std::string &locName) -> void {
                interface.createChild(locName);
            };

            std::function<void(const std::string &,const std::string &)> createChild2 = [&interface](const std::string &locName,const std::string &type = "group") ->void {
                interface.createChild(locName,type);
            };

            (*lua)["createChild"]= sol::overload(
                    createChild1,createChild2
                    );
            // -------------------------------- createChild() ------------------------------------------------


            // -------------------------------- delete location () ------------------------------------------------
            // delete all children
            lua->set_function("deleteChildren",[&interface] (){interface.deleteChildren();}) ;
            // delete one child
            lua->set_function("deleteChild",[&interface] (const std::string&name){interface.deleteChild(name);}) ;
            lua->set_function("deleteSelf",[&interface](const std::string &name){interface.deleteSelf();});
            // -------------------------------- delete location () ------------------------------------------------




            // createLocation("/obj/render/")
            auto createLocationFunc = [&interface](const std::string&locpath)->void{
                Foundry::Katana::CreateLocationInfo info;
                Foundry::Katana::CreateLocation(info,interface,locpath);
            };
            lua->set_function("createLocation",createLocationFunc);



            // Get Attrib
            // pointattrib()


            // stop eval child location()
            lua->set_function("stop",[&interface] (){interface.stopChildTraversal();} );



        }

    };




}





#endif //BUILDINGCTSLUAJIT_BINDKATANADATA_H
