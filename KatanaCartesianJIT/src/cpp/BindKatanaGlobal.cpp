#include "BindKatanaGlobal.h"

namespace Cartesian {

    void BindGlobalVars::bind(Foundry::Katana::GeolibCookInterface& interface, sol::state * lua) {
        auto time = Foundry::Katana::GetCurrentTime(interface);
        lua->set("Time", time);

        auto numsamples = Foundry::Katana::GetNumSamples(interface);
        lua->set("numSamples", numsamples);

        auto shufferOpen = Foundry::Katana::GetShutterOpen(interface);
        lua->set("shutterOpen", shufferOpen);

        auto shufferClose = Foundry::Katana::GetShutterClose(interface);
        lua->set("shutterClose", shufferClose);


        std::string inputLocPath = interface.getInputLocationPath();
        lua->set("locationPath", inputLocPath);

        auto getvarable = [&interface](const std::string& varname) {
            FnAttribute::StringAttribute ret = Foundry::Katana::GetGraphStateVariable(interface, varname);
            return ret.getValue();
        };
        lua->set_function("getgraphvariable",getvarable);
    }

}