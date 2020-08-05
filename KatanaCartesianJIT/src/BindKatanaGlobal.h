//
// Created by admin on 2020/8/3.
//

#ifndef BUILDINGCTSLUAJIT_BINDKATANAGLOBAL_H
#define BUILDINGCTSLUAJIT_BINDKATANAGLOBAL_H


#include <FnGeolib/op/FnGeolibOp.h>
#include "sol.hpp"

namespace Cartesian{

    class BindGlobalVars{
    public:
        static void bind(Foundry::Katana::GeolibCookInterface &interface,  const std::shared_ptr<sol::state>& lua){
            auto time = Foundry::Katana::GetCurrentTime(interface);
            lua->set("Time", time);

            auto numsamples  = Foundry::Katana::GetNumSamples(interface);
            lua->set("numSamples", numsamples);

            auto shufferOpen = Foundry::Katana::GetShutterOpen(interface);
            lua->set("shutterOpen", shufferOpen);

            auto shufferClose = Foundry::Katana::GetShutterClose(interface);
            lua->set("shutterClose", shufferClose);


            // Get the attr type
            FnAttribute::StringAttribute attr = interface.getAttr("type");
            std::string type = attr.getValue();
            lua->set("type",type);



            std::string inputLocPath = interface.getInputLocationPath();
            lua->set("locationPath",inputLocPath);

        }



    };
    class BindGlobalFunction{
    public:
        static void bind(Foundry::Katana::GeolibCookInterface &interface,  const std::shared_ptr<sol::state>& ptr){
            // getLocationType -> return string
            auto getLocationtype = [&interface]() ->std::string{return Foundry::Katana::GetInputLocationType(interface); };
            ptr->set_function("getLocationType", getLocationtype );

            //GetAbsOutputLocationPath


            // get attribute

            // add attribute

        }
    };




}






#endif //BUILDINGCTSLUAJIT_BINDKATANAGLOBAL_H
