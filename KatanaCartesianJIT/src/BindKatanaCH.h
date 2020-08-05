//
// Created by admin on 2020/8/4.
//

#ifndef BUILDINGCTSLUAJIT_BINDKATACH_H
#define BUILDINGCTSLUAJIT_BINDKATACH_H
namespace Cartesian {


    class BindKatanaCH {
    public:
        static void bind(Foundry::Katana::GeolibCookInterface &interface, const std::shared_ptr<sol::state> &lua)
        {

            // ---------------------------- chf()---------------------------------------------------------
            auto chf = [&interface](const std::string &chanName) -> float {
                FnAttribute::GroupAttribute userGroupAttr = interface.getOpArg("user" );
                if(userGroupAttr.isValid()){
                    FnAttribute::FloatAttribute chAttr = userGroupAttr.getChildByName( chanName);
                    if(chAttr.isValid()){
                        return chAttr.getValue();
                    }
                    std::cout << "[KATANA::CARTESIAN]: can not get user." << chanName << std::endl;
                }
                else
                    std::cout << "[KATANA::CARTESIAN]: get param must in user group attrib\n";
                return 0;
            };
            lua->set_function("chf", chf);


            // ---------------------------- chs()---------------------------------------------------------
            auto chs = [&interface](const std::string &chanName) -> std::string {
                FnAttribute::GroupAttribute userGroupAttr = interface.getOpArg("user" );
                if(userGroupAttr.isValid()){
                    FnAttribute::StringAttribute chAttr = userGroupAttr.getChildByName(chanName);
                    if(chAttr.isValid()){
                        return chAttr.getValue();
                    }
                    std::cout << "[KATANA::CARTESIAN]: can not get user." << chanName << std::endl;
                }
                else
                    std::cout << "[KATANA::CARTESIAN]: get param must in user group attrib\n";
                return "";
            };
            lua->set_function("chs", chs);


            // ---------------------------- chs()---------------------------------------------------------
            auto chcolor = [&interface](const std::string &chanName) -> std::string {
                FnAttribute::GroupAttribute userGroupAttr = interface.getOpArg("user" );
                if(userGroupAttr.isValid()){
                    FnAttribute::StringAttribute chAttr = userGroupAttr.getChildByName(chanName);
                    if(chAttr.isValid()){
                        return chAttr.getValue();
                    }
                    std::cout << "[KATANA::CARTESIAN]: can not get user." << chanName << std::endl;
                }
                else
                    std::cout << "[KATANA::CARTESIAN]: get param must in user group attrib\n";
                return "";
            };
            lua->set_function("chcolor", chcolor);

        } // end of bind

    };
}
#endif //BUILDINGCTSLUAJIT_BINDKATACH_H
